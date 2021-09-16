/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/surface_group.h>

void SurfaceGroup::add_child(shared_ptr<Surface> surface)
{
    m_surfaces.push_back(surface);
}

bool SurfaceGroup::intersect(const Ray3f &_ray, HitInfo &hit) const
{
    // copy the ray so we can modify the tmax values as we traverse
    Ray3f ray          = _ray;
    bool  hit_anything = false;

    // This is a linear intersection test that iterates over all primitives
    // within the scene. It's the most naive intersection test and hence very
    // slow if you have many primitives.

    // foreach primitive
    for (auto surface : m_surfaces)
    {
        if (surface->intersect(ray, hit))
        {
            hit_anything = true;
            ray.maxt     = hit.t;
        }
    }

    // record closest intersection
    return hit_anything;
}



DARTS_REGISTER_CLASS_IN_FACTORY(SurfaceGroup, SurfaceGroup, "group")