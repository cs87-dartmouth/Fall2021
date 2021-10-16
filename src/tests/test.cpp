/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/factory.h>
#include <darts/image.h>
#include <darts/parallel_for.h>
#include <darts/progress.h>
#include <darts/sampling.h>
#include <darts/test.h>
#include <filesystem/resolver.h>

void run_tests(const json &j)
{
    // check if this is a scene to render, or a test to execute
    if (j.contains("type") && j["type"] == "tests")
    {
        int count      = j["tests"].size();
        int num_passed = 0;
        for (auto &t : j["tests"])
        {
            try
            {
                auto test = DartsFactory<Test>::create(t);
                test->run();
                num_passed++;
            }
            catch (const std::exception &e)
            {
                spdlog::error("Test failed: {}", e.what());
            }
        }
        if (num_passed == count)
        {
            spdlog::info("Passed all {}/{} tests!", num_passed, count);
            exit(EXIT_SUCCESS);
        }
        else
        {
            spdlog::error("Failed {}/{} tests.", count - num_passed, count);
            exit(EXIT_FAILURE);
        }
    }
}

SampleTest::SampleTest(const json &j)
{
    name         = j.at("name");
    image_width  = j.value("image_width", 512);
    image_height = j.value("image_height", 256);
    num_samples  = j.value("num_samples", 50) * image_width * image_height;
}

Vec2i SampleTest::direction_to_pixel(const Vec3f &dir) const
{
    return Vec2i(direction_to_spherical_coordinates(dir) * Vec2f(image_width * INV_TWOPI, image_height * M_1_PI));
}

Vec3f SampleTest::pixel_to_direction(const Vec2i &pixel) const
{
    return spherical_coordinates_to_direction((Vec2f(pixel.x, pixel.y) + 0.5f) *
                                              Vec2f(2 * M_PI / image_width, M_PI / image_height));
}

Image3f SampleTest::generate_heatmap(const Array2d<float> &density, float max_value)
{
    Image3f result(density.width(), density.height());

    for (int y = 0; y < density.height(); ++y)
        for (int x = 0; x < density.width(); ++x)
            result(x, y) = inferno(density(x, y) / max_value);

    return result;
}

void SampleTest::run()
{
    fmt::print("---------------------------------------------------------------------------\n");
    fmt::print("Running sample test for \"{}\"\n", name);

    // Merge adjacent pixels to decrease noise in the histogram
    constexpr int histo_subsample = 4;

    // Step 1: Evaluate pdf over the sphere and compute its integral
    double         integral = 0.0f;
    Array2d<float> pdf(image_width / histo_subsample, image_height / histo_subsample);
    Progress       progress1("Evaluating analytic PDF", pdf.height());
    for (int y = 0; y < pdf.height(); ++y, ++progress1)
        for(int x=0;x<pdf.width();x++)
                    {
                         float accum = 0.f;
                         for (int sx = 0; sx < histo_subsample; ++sx)
                             for (int sy = 0; sy < histo_subsample; ++sy)
                             {
                                 Vec3f dir =
                                     pixel_to_direction(Vec2i(histo_subsample * x + sx, histo_subsample * y + sy));
                                 float sin_theta  = std::sqrt(max(1.0f - dir.z * dir.z, 0.0f));
                                 float pixel_area = (M_PI / image_width) * (M_PI * 2.0f / image_height) * sin_theta;
                                 float value      = this->pdf(dir);
                                 accum += value;
                                 integral += pixel_area * value;
                             }
                         pdf(x, y) = accum / (histo_subsample * histo_subsample);
                     }
    progress1.set_done();

    // Step 2: Generate histogram of samples
    Array2d<float> histogram(image_width / histo_subsample, image_height / histo_subsample);

    int      valid_samples = 0;
    bool     nan_or_inf    = false;
    pcg32    rng;
    Progress progress2(fmt::format("Generating samples {}", num_samples), num_samples);
    for (int i = 0; i < num_samples; ++i, ++progress2)
    {
        Vec3f dir;
        if (!sample(dir, Vec2f(rng.nextFloat(), rng.nextFloat())))
            continue;

        if (std::isnan(dir.x + dir.y + dir.z) || std::isinf(dir.x + dir.y + dir.z))
        {
            nan_or_inf = true;
            continue;
        }

        // Map scattered direction to pixel in our sample histogram
        Vec2i pixel = direction_to_pixel(dir) / histo_subsample;
        if (pixel.x < 0 || pixel.y < 0 || pixel.x >= histogram.width() || pixel.y >= histogram.height())
            continue;

        // Incorporate Jacobian of spherical mapping and bin area into the sample weight
        float sin_theta = std::sqrt(max(1.0f - dir.z * dir.z, 0.0f));
        float weight    = (histogram.width() * histogram.height()) / (M_PI * (2.0f * M_PI) * num_samples * sin_theta);
        // Accumulate into histogram
        histogram(pixel.x, pixel.y) += weight;
        valid_samples++;
    }

    progress2.set_done();

    // Now upscale our histogram and pdf
    Array2d<float> histo_fullres(image_width, image_height);
    for (int y = 0; y < histo_fullres.height(); ++y)
        for (int x = 0; x < histo_fullres.width(); ++x)
            histo_fullres(x, y) = histogram(x / histo_subsample, y / histo_subsample);

    Array2d<float> pdf_fullres(image_width, image_height);
    for (int y = 0; y < pdf_fullres.height(); ++y)
        for (int x = 0; x < pdf_fullres.width(); ++x)
            pdf_fullres(x, y) = pdf(x / histo_subsample, y / histo_subsample);

    // Step 3: For auto-exposure, compute 99.95th percentile instead of maximum for increased robustness
    std::vector<float> values(&pdf(0, 0), &pdf(0, 0) + pdf.size());
    std::sort(values.begin(), values.end());
    float max_value = values[int((pdf.size() - 1) * 0.9995)];
    for (int i = 0; i < pdf.size(); ++i)
        if (std::isnan(pdf(i)) || std::isinf(pdf(i)))
            nan_or_inf = true;

    // Generate heat maps
    // NOTE: we use get_file_resolver()[0] here to refer to the parent directory of the scene file.
    // This assumes that the calling code has prepended this directory to the front of the global resolver list
    generate_heatmap(pdf_fullres, max_value).save((get_file_resolver()[0] / (name + "-pdf.png")).str());
    generate_heatmap(histo_fullres, max_value).save((get_file_resolver()[0] / (name + "-sampled.png")).str());

    // Output statistics
    fmt::print("Integral of PDF (should be close to 1): {}\n", integral);
    fmt::print("{}% of samples were valid (this should be close to 100%)\n", (valid_samples * 100) / num_samples);
    if (nan_or_inf)
        spdlog::warn("Some directions/PDFs contained invalid values (NaN or infinity). This should not "
                     "happen. Make sure you catch all corner cases in your code.");
    print_more_statistics();
}
