/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/factory.h>
#include <darts/fwd.h>
#include <stdlib.h>

/*! \addtogroup group_materials
    @{
*/


/// A base class used to represent surface material properties.
class Material
{
public:
    /// Default constructor which accepts a #json object of named parameters
    Material(const json &j = json::object());

    /// Free all memory
    virtual ~Material() = default;

    /*!
       \brief Compute the scattered direction scattered at a surface hitpoint.

       The base Material does not scatter any light, so it simply returns false.

       \param  ray          incoming ray
       \param  hit          the ray's intersection with the surface
       \param  attenuation  how much the light should be attenuated
       \param  scattered    the direction light should be scattered
       \return bool         True if the surface scatters light
     */
    virtual bool scatter(const Ray3f &ray, const HitInfo &hit, Color3f &attenuation, Ray3f &scattered) const
    {
        return false;
    }

    /*!
       Compute the amount of emitted light at the surface hitpoint.

       The base Material class does not emit light, so it simply returns black.

       \param  ray		the incoming ray
       \param  hit		the ray's intersection with the surface
       \return			the emitted color
     */
    virtual Color3f emitted(const Ray3f &ray, const HitInfo &hit) const
    {
        return Color3f(0, 0, 0);
    }

    /*!
        Return whether or not this Material is emissive.
        This is primarily used to create a global list of emitters for sampling.
    */
    virtual bool is_emissive() const
    {
        return false;
    }

};

/// A perfectly diffuse (Lambertian) material
class Lambertian : public Material
{
public:
    Lambertian(const json &j = json::object());

    bool scatter(const Ray3f &ray, const HitInfo &hit, Color3f &attenuation, Ray3f &scattered) const override;


    Color3f albedo = Color3f(0.8f); ///< The diffuse color (fraction of light that is reflected per color channel).
};

/*! @}*/

/*!
    \file
    \brief Class #Material
*/