/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/mesh.h>

/// An instance of a triangle for a given face in a mesh
class Triangle : public Surface
{
public:
    /// Parse and construct a single triangle
    Triangle(const json &j);

    /// Construct a single triangle of an existing mesh
    Triangle(const json &j, shared_ptr<const Mesh> mesh, uint32_t tri_number);

    bool intersect(const Ray3f &ray, HitInfo &hit) const override;

    Box3f bounds() const override;


    bool is_emissive() const override
    {
        return m_mesh && m_mesh->materials[m_mesh->Fm[m_face_idx]] &&
               m_mesh->materials[m_mesh->Fm[m_face_idx]]->is_emissive();
    }

protected:
    // convenience function to access the i-th vertex (i must be 0, 1, or 2)
    Vec3f vertex(size_t i) const
    {
        return m_mesh->vs[m_mesh->Fv[m_face_idx][i]];
    }

    shared_ptr<const Mesh> m_mesh;
    uint32_t               m_face_idx;
};

/// ray - single triangle intersection routine
bool single_triangle_intersect(const Ray3f &ray, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec3f *n0,
                               const Vec3f *n1, const Vec3f *n2, const Vec2f *t0, const Vec2f *t1, const Vec2f *t2,
                               HitInfo &isect, const Material *material = nullptr);

/*!
    \file
    \brief Class #Triangle
*/