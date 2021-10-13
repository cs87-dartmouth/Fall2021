/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/common.h>
#include <pcg32.h>

/*! \addtogroup group_sampling
    @{
*/

/// Global random number generator that produces floats between <tt>[0,1)</tt>
inline float randf()
{
    static pcg32 rng = pcg32();
    return rng.nextFloat();
}

/// Sample a random point uniformly within a unit sphere (uses the global randf() RNG and rejection sampling)
inline Vec3f random_in_unit_sphere()
{
    Vec3f p;
    do
    {
        float a = randf();
        float b = randf();
        float c = randf();
        p       = 2.0f * Vec3f(a, b, c) - Vec3f(1);
    } while (length2(p) >= 1.0f);

    return p;
}

/// Sample a random point uniformly within a unit disk (uses the global randf() RNG and rejection sampling)
inline Vec2f random_in_unit_disk()
{
    Vec2f p;
    do
    {
        float a = randf();
        float b = randf();
        p       = 2.0f * Vec2f(a, b) - Vec2f(1);
    } while (length2(p) >= 1.0f);

    return p;
}

/// Uniformly sample a vector on a 2D disk with radius 1, centered around the origin
inline Vec2f sample_disk(const Vec2f &rv)
{
    float r                 = std::sqrt(rv.y);
    auto [sin_phi, cos_phi] = sincos(2.0f * M_PI * rv.x);

    return Vec2f(cos_phi * r, sin_phi * r);
}


/// Probability density of #sample_disk()
inline float sample_disk_pdf(const Vec2f &p)
{
    return length2(p) <= 1 ? INV_PI : 0.0f;
}

/// Uniformly sample a vector on the unit sphere with respect to solid angles
inline Vec3f sample_sphere(const Vec2f &rv)
{
    return Vec3f{0.f}; // CHANGEME
}

/// Probability density of #sample_sphere()
inline float sample_sphere_pdf()
{
    return 0.f; // CHANGEME
}


/// Uniformly sample a vector on the unit hemisphere around the pole (0,0,1) with respect to solid angles
inline Vec3f sample_hemisphere(const Vec2f &rv)
{
    return Vec3f{0.f}; // CHANGEME
}

/// Probability density of #sample_hemisphere()
inline float sample_hemisphere_pdf(const Vec3f &v)
{
    return 0.f; // CHANGEME
}

/// Uniformly sample a vector on the unit hemisphere around the pole (0,0,1) with respect to projected solid
/// angles
inline Vec3f sample_hemisphere_cosine(const Vec2f &rv)
{
    return Vec3f{0.f}; // CHANGEME
}

/// Probability density of #sample_hemisphere_cosine()
inline float sample_hemisphere_cosine_pdf(const Vec3f &v)
{
    return 0.f; // CHANGEME
}

/// Sample a vector on the unit hemisphere with a cosine-power density about the pole (0,0,1)
inline Vec3f sample_hemisphere_cosine_power(float exponent, const Vec2f &rv)
{
    return Vec3f{0.f}; // CHANGEME
}

/// Probability density of #sample_hemisphere_cosine_power()
inline float sample_hemisphere_cosine_power_pdf(float exponent, float cosine)
{
    return 0.f; // CHANGEME
}




/*! @}*/

/*!
    \file
    \brief Random sampling on various domains
*/