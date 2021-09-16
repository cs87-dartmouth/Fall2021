/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/surface.h>

/*! \addtogroup group_surfaces
    @{
*/

/*!
    A collection of Surfaces grouped together.

    Provides an interface for treating a collection of Surfaces as a single Surface. This base class implements a naive
    linear-time intersection routine which simply intersects each ray with every child Surface.

    This class also serves as the superclass for acceleration structures (such as BVHs, KD-Trees) which are responsible
    for performing ray-surface intersection tests against a collection of Surfaces.
*/
class SurfaceGroup : public Surface
{
public:
    /// Create a new and empty naive accelerator
    SurfaceGroup(const json &j = json::object())
    {
    }

    virtual void add_child(shared_ptr<Surface> surface) override;

    /*!
        Intersect a ray against all surfaces registered with the Accelerator.

        Detailed information about the intersection, if any, will be stored in the provided \p hit data record.

        \return true If an intersection was found
    */
    bool intersect(const Ray3f &ray, HitInfo &hit) const override;



protected:
    vector<shared_ptr<Surface>> m_surfaces; ///< All children
};

/*! @}*/

/*!
    \file
    \brief Class #SurfaceGroup
*/
