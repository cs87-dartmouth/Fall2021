/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/sampler.h>
#include <darts/sampling.h>

/**
    %Independent sampling - returns independent uniformly distributed random numbers on \f$[0, 1)^2\f$.

    This class is essentially just a wrapper around the pcg32 pseudorandom number generator. For more details on what
    sample generators do in general, refer to the \ref Sampler class.
 */
class Independent : public Sampler
{
public:
    Independent(const json &j)
    {
        m_sample_count = j.at("samples").get<int>();
    }

    virtual ~Independent()
    {
    }

    /**
        Create an exact clone of the current instance

        This is useful if you want to duplicate a sampler to use in multiple threads
     */
    std::unique_ptr<Sampler> clone() const override
    {
        std::unique_ptr<Independent> cloned(new Independent());
        cloned->m_sample_count      = m_sample_count;
        cloned->m_base_seed         = m_base_seed;
        cloned->m_sample_count      = m_sample_count;
        cloned->m_current_sample    = m_current_sample;
        cloned->m_current_dimension = m_current_dimension;

        cloned->m_rng = m_rng;
        return std::move(cloned);
    }

    void seed(int seedx, int seedy) override
    {
        Sampler::seed(seedx, seedy);
        m_rng.seed(m_base_seed + seedx, m_base_seed + seedy);
    }

    void start_pixel(int x, int y) override
    {
        // No-op for this sampler
    }

    float next1f() override
    {
        m_current_dimension++;
        return m_rng.nextFloat();
    }

    Vec2f next2f() override
    {
        m_current_dimension += 2;
        return {m_rng.nextFloat(), m_rng.nextFloat()};
    }

protected:
    Independent()
    {
    }

    pcg32 m_rng;
};

DARTS_REGISTER_CLASS_IN_FACTORY(Sampler, Independent, "independent")
