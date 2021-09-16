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







/*! @}*/

/*!
    \file
    \brief Random sampling on various domains
*/