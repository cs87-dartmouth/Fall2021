/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <cmath>
#include <darts/common.h>
#include <filesystem/resolver.h>

#include <darts/factory.h>

#include <spdlog/sinks/stdout_color_sinks.h>

std::atomic<uint64_t> intersection_tests = 0u;
std::atomic<uint64_t> rays_traced        = 0u;

void darts_init(int verbosity)
{
    // changing the default logger requires calling this twice since the default empty name is already taken
    // here we force the default logger to always output in color
    spdlog::set_default_logger(spdlog::stdout_color_mt("some_arbitrary_name"));
    spdlog::set_default_logger(spdlog::stdout_color_mt("", spdlog::color_mode::always));

    spdlog::set_pattern("%^%v%$");
    spdlog::set_level(spdlog::level::level_enum(verbosity));

    spdlog::debug("Available materials: {}.", fmt::join(DartsFactory<Material>::registered_types(), ", "));
    spdlog::debug("Available surfaces: {}.", fmt::join(DartsFactory<Surface>::registered_types(), ", "));
}

Vec3f spherical_coordinates_to_direction(const Vec2f &phi_theta)
{
    auto [sin_theta, cos_theta] = sincos(phi_theta.y);
    auto [sin_phi, cos_phi]     = sincos(phi_theta.x);

    return Vec3f(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
}

Vec2f direction_to_spherical_coordinates(const Vec3f &v)
{
    return {std::atan2(-v.y, -v.x) + M_PI, std::acos(v.z)};
}

Vec2f direction_to_spherical_uv(const Vec3f &p)
{
    auto sph = direction_to_spherical_coordinates(p);
    return {mod(sph.x * INV_TWOPI - 0.5f, 1.f), 1.f - sph.y * INV_PI};
}

Vec3f spherical_uv_to_direction(const Vec2f &uv)
{
    return spherical_coordinates_to_direction(Vec2f{(uv.x - 0.5f) * 2.0f * M_PI, (1.f - uv.y) * M_PI});
}

/*
    Taken from Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
float fresnel(float cos_theta_i, float ext_ior, float int_ior)
{
    float etaI = ext_ior, etaT = int_ior;

    if (ext_ior == int_ior)
        return 0.0f;

    // Swap the indices of refraction if the interaction starts at the inside of the object
    if (cos_theta_i < 0.0f)
    {
        std::swap(etaI, etaT);
        cos_theta_i = -cos_theta_i;
    }

    // Using Snell's law, calculate the squared sine of the angle between the normal and the transmitted ray
    float eta = etaI / etaT, sinThetaTSqr = eta * eta * (1 - cos_theta_i * cos_theta_i);

    if (sinThetaTSqr > 1.0f)
        return 1.0f; // Total internal reflection!

    float cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

    float Rs = (etaI * cos_theta_i - etaT * cosThetaT) / (etaI * cos_theta_i + etaT * cosThetaT);
    float Rp = (etaT * cos_theta_i - etaI * cosThetaT) / (etaT * cos_theta_i + etaI * cosThetaT);

    return (Rs * Rs + Rp * Rp) / 2.0f;
}

std::string time_string(double time, int precision)
{
    if (std::isnan(time) || std::isinf(time))
        return "inf";

    int seconds = time / 1000;
    int minutes = seconds / 60;
    int hours   = minutes / 60;
    int days    = hours / 24;

    if (days > 0)
        return fmt::format("{}d:{:0>2}h:{:0>2}m:{:0>2}s", days, hours % 24, minutes % 60, seconds % 60);
    else if (hours > 0)
        return fmt::format("{}h:{:0>2}m:{:0>2}s", hours % 24, minutes % 60, seconds % 60);
    else if (minutes > 0)
        return fmt::format("{}m:{:0>2}s", minutes % 60, seconds % 60);
    else if (seconds > 0)
        return fmt::format("{:.3f}s", time / 1000);
    else
        return fmt::format("{}ms", int(time));
}

std::string indent(const std::string &string, int amount)
{
    // This could probably be done faster (it's not really speed-critical though)
    std::istringstream iss(string);
    std::string        output;
    std::string        spacer(amount, ' ');
    bool               first_line = true;
    for (std::string line; std::getline(iss, line);)
    {
        if (!first_line)
            output += spacer;
        output += line;
        if (!iss.eof())
            output += '\n';
        first_line = false;
    }
    return output;
}

filesystem::resolver &get_file_resolver()
{
    static filesystem::resolver resolver;
    return resolver;
}

//
// Converted to C++ by Wojciech Jarosz 2021
// Original copyright info from https://www.shadertoy.com/view/WlfXRN
// fitting polynomials to matplotlib colormaps
//
// License CC0 (public domain)
//   https://creativecommons.org/share-your-work/public-domain/cc0/
//
// feel free to use these in your own work!
//
// similar to https://www.shadertoy.com/view/XtGGzG but with a couple small differences:
//
//  - use degree 6 instead of degree 5 polynomials
//  - use nested horner representation for polynomials
//  - polynomials were fitted to minimize maximum error (as opposed to least squares)
//
// data fitted from https://github.com/BIDS/colormap/blob/master/colormaps.py
// (which is licensed CC0)

Color3f viridis(float t)
{
    constexpr Color3f c0(0.2777273272234177, 0.005407344544966578, 0.3340998053353061);
    constexpr Color3f c1(0.1050930431085774, 1.404613529898575, 1.384590162594685);
    constexpr Color3f c2(-0.3308618287255563, 0.214847559468213, 0.09509516302823659);
    constexpr Color3f c3(-4.634230498983486, -5.799100973351585, -19.33244095627987);
    constexpr Color3f c4(6.228269936347081, 14.17993336680509, 56.69055260068105);
    constexpr Color3f c5(4.776384997670288, -13.74514537774601, -65.35303263337234);
    constexpr Color3f c6(-5.435455855934631, 4.645852612178535, 26.3124352495832);

    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}

Color3f inferno(float t)
{
    constexpr Color3f c0(0.0002189403691192265, 0.001651004631001012, -0.01948089843709184);
    constexpr Color3f c1(0.1065134194856116, 0.5639564367884091, 3.932712388889277);
    constexpr Color3f c2(11.60249308247187, -3.972853965665698, -15.9423941062914);
    constexpr Color3f c3(-41.70399613139459, 17.43639888205313, 44.35414519872813);
    constexpr Color3f c4(77.162935699427, -33.40235894210092, -81.80730925738993);
    constexpr Color3f c5(-71.31942824499214, 32.62606426397723, 73.20951985803202);
    constexpr Color3f c6(25.13112622477341, -12.24266895238567, -23.07032500287172);

    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}

Color3f magma(float t)
{
    constexpr Color3f c0(-0.002136485053939582, -0.000749655052795221, -0.005386127855323933);
    constexpr Color3f c1(0.2516605407371642, 0.6775232436837668, 2.494026599312351);
    constexpr Color3f c2(8.353717279216625, -3.577719514958484, 0.3144679030132573);
    constexpr Color3f c3(-27.66873308576866, 14.26473078096533, -13.64921318813922);
    constexpr Color3f c4(52.17613981234068, -27.94360607168351, 12.94416944238394);
    constexpr Color3f c5(-50.76852536473588, 29.04658282127291, 4.23415299384598);
    constexpr Color3f c6(18.65570506591883, -11.48977351997711, -5.601961508734096);

    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}

Color3f plasma(float t)
{
    constexpr Color3f c0(0.05873234392399702, 0.02333670892565664, 0.5433401826748754);
    constexpr Color3f c1(2.176514634195958, 0.2383834171260182, 0.7539604599784036);
    constexpr Color3f c2(-2.689460476458034, -7.455851135738909, 3.110799939717086);
    constexpr Color3f c3(6.130348345893603, 42.3461881477227, -28.51885465332158);
    constexpr Color3f c4(-11.10743619062271, -82.66631109428045, 60.13984767418263);
    constexpr Color3f c5(10.02306557647065, 71.41361770095349, -54.07218655560067);
    constexpr Color3f c6(-3.658713842777788, -22.93153465461149, 18.19190778539828);

    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}
