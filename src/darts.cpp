/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <CLI/CLI.hpp>
#include <darts/scene.h>
#include <filesystem/resolver.h>
#include <fmt/chrono.h>
#include <darts/test.h>

// runs the raytrace over all tests and saves the corresponding images
int main(int argc, char **argv)
{
    int verbosity = spdlog::get_level();

    string   outfile;
    string   format = "png";
    string   scenefile;
    uint32_t seed = 53;

    CLI::App app{"Dartmouth Academic Ray Tracing Skeleton", "darts"};

    string save_formats = fmt::format("{}", fmt::join(Image3f::savable_formats(), ", "));

    app.add_option(
        "-o,--outfile", outfile,
        fmt::format("Specify the output image filename (extension must be one accepted by -f)", save_formats));
    app.add_option("-f,--format", format, fmt::format("Specify just the output image format; default: png."))
        ->check(CLI::IsMember(Image3f::savable_formats()));
    app.add_option("-s,--seed", seed,
                   fmt::format("Seed for the random number generator (e.g. the node id on a compute cluster)."))
        ->check(CLI::PositiveNumber);
    app.add_option("-v,--verbosity", verbosity,
                   R"(Set verbosity threshold T with lower values meaning more verbose
and higher values removing low-priority messages. All messages with
severity >= T are displayed, where the severities are:
    trace    = 0
    debug    = 1
    info     = 2
    warn     = 3
    err      = 4
    critical = 5
    off      = 6
The default is 2 (info).)")
        ->check(CLI::Range(0, 6));
    app.add_option(
           "scene", scenefile,
           "The filename of the JSON scenefile to load (or the string \"example_sceneN\", where N is 0, 1, 2, or 3).")
        ->required();

    try
    {
        CLI11_PARSE(app, argc, argv);

        darts_init(verbosity);

        // generate/load scene either by creating one of the hardcoded test scenes or loading from json file
        json j;
        int  scene_number = 0;
        if (sscanf(scenefile.c_str(), "example_scene%d", &scene_number) == 1)
            j = create_example_scene(scene_number);
        else
        {
            filesystem::path path(scenefile);

            // Add the parent directory of the scene file to the file resolver. That way, the scene file can reference
            // resources (OBJ files, textures) using relative paths
            get_file_resolver().prepend(path.parent_path());

            // open file
            std::ifstream stream(scenefile, std::ifstream::in);
            if (!stream.good())
                throw DartsException("Cannot open file: {}.", scenefile);

            stream >> j;
        }

        run_tests(j);

        auto scene = make_shared<Scene>(j);

        // use the outfile if specified, otherwise take the basename from the scene file and append the time.
        string outfile_hdr;
        if (outfile.empty())
        {
            std::time_t t           = std::time(nullptr);
            string      time_string = fmt::format("{:%Y-%m-%d-%H-%M-%S}", fmt::localtime(t));

            auto base   = scenefile.substr(0, scenefile.find_last_of('.')) + "-" + time_string + ".";
            outfile     = base + format;
            outfile_hdr = base + "exr";
        }

        spdlog::info("Will save rendered image to \"{}\"", outfile);

        auto image = scene->raytrace();

        spdlog::info("Number of intersection tests: {}", intersection_tests);
        spdlog::info("Number of rays traced: {}", rays_traced);
        spdlog::info("Average number of intersection tests per ray: {}",
                     float(intersection_tests) / float(rays_traced));
        spdlog::info("Writing rendered image to file \"{}\"...", outfile);

        image.save(outfile);

        // if the outfile wasn't specified, also save the rendering in .hdr format
        if (!outfile_hdr.empty())
            image.save(outfile_hdr);

        spdlog::info("done!");
    }
    catch (const std::exception &e)
    {
        spdlog::critical("{}", e.what());
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
