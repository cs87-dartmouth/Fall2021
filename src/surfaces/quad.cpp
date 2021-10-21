/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#include <darts/scene.h>

/** \addtogroup group_surfaces
 *  @{
 */

/// A quad spanning from -\ref m_size / 2 to \ref m_size / 2 in the (x,y)-plane at z=0.
class Quad : public XformedSurfaceWithMaterial
{
public:
    Quad(const json &j = json::object());

    bool intersect(const Ray3f &ray, HitInfo &hit) const override;
    Box3f local_bounds() const override;
    Color3f sample(EmitterRecord &rec, const Vec2f &rv) const override;
    float   pdf(const Vec3f &o, const Vec3f &v) const override;

protected:
    Vec2f m_size = Vec2f(1.f); ///< The extent of the quad in the (x,y) plane
};

Quad::Quad(const json &j) : XformedSurfaceWithMaterial(j)
{
    m_size = j.value("size", m_size);
    m_size /= 2.f;

}

bool Quad::intersect(const Ray3f &ray, HitInfo &hit) const
{
    INCREMENT_INTERSECTION_TESTS;

    // compute ray intersection (and ray parameter), continue if not hit
    auto tray = m_xform.inverse().ray(ray);
    if (tray.d.z == 0)
        return false;
    auto t = -tray.o.z / tray.d.z;
    auto p = tray(t);

    if (m_size.x < std::abs(p.x) || m_size.y < std::abs(p.y))
        return false;

    // check if computed param is within ray.mint and ray.maxt
    if (t < tray.mint || t > tray.maxt)
        return false;

    // project hitpoint onto plane to reduce floating-point error
    p.z = 0;

    // if hit, set intersection record values
    hit.t  = t;
    hit.p  = m_xform.point(p);
    hit.gn = hit.sn = normalize(m_xform.normal({0, 0, 1}));
    hit.mat         = m_material.get();
    hit.uv = Vec2f(0.0f, 0.0f); /* TODO: Compute proper UV coordinates */

    return true;
}

Box3f Quad::local_bounds() const
{
    return Box3f(-Vec3f(m_size.x, m_size.y, 0) - Vec3f(1e-4f), Vec3f(m_size.x, m_size.y, 0) + Vec3f(1e-4f));
}

Color3f Quad::sample(EmitterRecord &rec, const Vec2f &rv) const
{
    rec.hit.p   = m_xform.point({(2 * rv.x - 1) * m_size.x, (2 * rv.y - 1) * m_size.y, 0});
    rec.wi      = rec.hit.p - rec.o;
    float dist2 = length2(rec.wi);
    rec.hit.t   = std::sqrt(dist2);
    rec.hit.mat = m_material.get();
    rec.hit.gn = rec.hit.sn = m_xform.normal({0, 0, 1});
    rec.wi /= rec.hit.t;

    // convert to solid angle measure
    float area   = 4 * length(cross(m_xform.vector({m_size.x, 0, 0}), m_xform.vector({0, m_size.y, 0})));
    float cosine = std::abs(dot(rec.hit.gn, rec.wi));
    rec.pdf      = dist2 / (cosine * area);

    return rec.hit.mat->emitted(Ray3f(rec.o, rec.wi), rec.hit) / rec.pdf;
}

float Quad::pdf(const Vec3f &o, const Vec3f &v) const
{
    HitInfo rec;
    if (this->intersect(Ray3f(o, v), rec))
    {
        float area             = 4 * length(cross(m_xform.vector({m_size.x, 0, 0}), m_xform.vector({0, m_size.y, 0})));
        float distance_squared = rec.t * rec.t * length2(v);
        float cosine           = std::abs(dot(v, rec.gn) / length(v));
        return distance_squared / (cosine * area);
    }
    else
        return 0;
}

DARTS_REGISTER_CLASS_IN_FACTORY(Surface, Quad, "quad")

/**
    \file
    \brief Class #Quad
*/

/** @}*/
