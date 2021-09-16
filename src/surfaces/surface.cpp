/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/scene.h>
#include <darts/surface.h>

XformedSurfaceWithMaterial::XformedSurfaceWithMaterial(const json &j)
{
    m_xform    = j.value("transform", m_xform);
    m_material = DartsFactory<Material>::find(j);
}

