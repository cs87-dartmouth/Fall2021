/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/factory.h>
#include <darts/image.h>
#include <darts/material.h>
#include <darts/surface.h>
#include <darts/surface_group.h>
#include <darts/test.h>

#include <algorithm>

struct SurfaceSampleTest : public SampleTest
{
    SurfaceSampleTest(const json &j);

    bool  sample(Vec3f &dir, const Vec2f &rv) override;
    float pdf(Vec3f &dir) override;

    shared_ptr<Surface> surface;
    Vec3f               normal;
};

SurfaceSampleTest::SurfaceSampleTest(const json &j) : SampleTest(j)
{
    if (j.contains("surface"))
        surface = DartsFactory<Surface>::create(j.at("surface"));
    else if (j.contains("surfaces"))
    {
        json j2    = j["surfaces"];
        auto group = DartsFactory<SurfaceGroup>::create(j2);
        //
        // parse the children
        //
        if (j2.contains("children"))
        {
            for (auto &s : j2["children"])
            {
                auto child = DartsFactory<Surface>::create(s);
                child->add_to_parent(group.get(), child, j2);
            }
        }

        group->build();

        surface = group;
    }
    else
        throw DartsException("Invalid sample_surface file. No 'surface' or 'surfaces' field found.");
}

bool SurfaceSampleTest::sample(Vec3f &dir, const Vec2f &rv)
{
    // Sample geometry
    EmitterRecord rec;
    rec.o = Vec3f(0.f);
    surface->sample(rec, rv);
    dir = normalize(rec.wi);

    return true;
}

float SurfaceSampleTest::pdf(Vec3f &dir)
{
    return surface->pdf(Vec3f(0.f), dir);
}

DARTS_REGISTER_CLASS_IN_FACTORY(Test, SurfaceSampleTest, "sample_surface")
