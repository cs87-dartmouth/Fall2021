/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/surface.h>

/*! \addtogroup group_surfaces
    @{
*/

/// A sphere centered at the origin with radius \ref m_radius
class Sphere : public XformedSurfaceWithMaterial
{
public:
    Sphere(float radius, shared_ptr<const Material> material, const Transform &xform = Transform());
    Sphere(const json &j = json::object());

    bool intersect(const Ray3f &ray, HitInfo &hit) const override;
    Box3f local_bounds() const override;

protected:
    float m_radius = 1.0f;  ///< The radius of the sphere
};

/*! @}*/

/*!
    \file
    \brief Class #Sphere
*/
