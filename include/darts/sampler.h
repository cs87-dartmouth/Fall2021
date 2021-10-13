/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/factory.h>
#include <darts/vec.h>
#include <memory>

/*!
    Abstract sample generator.

    A sample generator is responsible for generating the random number stream that will be passed to an #Integrator
    implementation as it computes the radiance incident along a specified ray.
*/
class Sampler
{
public:
    Sampler() : m_sample_count(1u), m_current_sample(0u), m_current_dimension(0u)
    {
    }

    virtual ~Sampler()
    {
    }

    /// Create an exact copy of this Sampler instance
    virtual std::unique_ptr<Sampler> clone() const = 0;

    /// Deterministically seed the underlying RNG (to produce identical results between runs)
    virtual void seed(int seedx, int seedy)
    {
        m_current_dimension = 0u;
        m_current_sample    = 0u;
    }

    /*!
        Prepare to generate samples for pixel (x,y).

        This function is called every time the integrator starts rendering a new pixel.
    */
    virtual void start_pixel(int x, int y) = 0;

    /// Advance to the next sample
    virtual void advance()
    {
        m_current_dimension = 0u;
        m_current_sample++;
    }

    /// Retrieve the next float value (dimension) from the current sample
    virtual float next1f() = 0;

    /// Retrieve the next two float values (dimensions) from the current sample
    virtual Vec2f next2f() = 0;

    /// Return the number of configured pixel samples
    virtual uint32_t sample_count() const
    {
        return m_sample_count;
    }

    void set_base_seed(uint32_t base)
    {
        m_base_seed = base;
    }

    uint32_t current_sample() const
    {
        return m_current_sample;
    }

    uint32_t current_dimension() const
    {
        return m_current_dimension;
    }

protected:
    uint32_t m_base_seed;
    uint32_t m_sample_count;
    uint32_t m_current_sample;
    uint32_t m_current_dimension;
};

/*!
    \file
    \brief Class #Sampler
*/
