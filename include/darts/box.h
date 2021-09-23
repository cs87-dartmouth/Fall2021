/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/common.h>
#include <darts/ray.h>

/*! \addtogroup group_math
    @{
*/

/// An N-D axis-aligned bounding box consisting of two N-D points min and max
template <int N, typename T>
struct Box
{
    Vec<N, T> min; ///< The lower-bound of the interval
    Vec<N, T> max; ///< The upper-bound of the interval

    /// Create an empty box by default.
    Box() : min(std::numeric_limits<T>::max()), max(std::numeric_limits<T>::lowest())
    {
    }

    /// Create a box containing a single point in space
    explicit Box(const Vec<N, T> &p) : min(p), max(p)
    {
    }

    /// Create a box enclosing two points
    Box(const Vec<N, T> &p1, const Vec<N, T> &p2) : min(linalg::min(p1, p2)), max(linalg::max(p1, p2))
    {
    }

    bool is_empty() const
    {
        for (size_t i = 0; i < N; ++i)
            if (min[i] > max[i])
                return true;
        return false;
    }

    void enclose(const Box &box2)
    {
        min = linalg::min(min, box2.min);
        max = linalg::max(max, box2.max);
    }

    void enclose(const Vec<N, T> &p)
    {
        min = linalg::min(min, p);
        max = linalg::max(max, p);
    }

    bool is_finite() const
    {
        return !(std::numeric_limits<T>::has_infinity &&
                 (linalg::any(linalg::equal(std::numeric_limits<T>::infinity(), min)) ||
                  linalg::any(linalg::equal(std::numeric_limits<T>::infinity(), max))));
    }

    Vec<N, T> center() const
    {
        if (!is_finite())
            return Vec<N, T>();
        else
            return (min + max) / T(2);
    }
    Vec<N, T> diagonal() const
    {
        return max - min;
    }

    /// Calculate the N-dimensional volume of the bounding box
    T volume() const
    {
        return product(diagonal());
    }

    /// Calculate the N-1 dimensional volume of the boundary
    T area() const
    {
        auto d = diagonal();
        T    result(0);
        for (int i = 0; i < N; ++i)
        {
            T term(1);
            for (int j = 0; j < N; ++j)
            {
                if (i == j)
                    continue;
                term *= d[j];
            }
            result += term;
        }
        return 2 * result;
    }

    /*!
        Check whether a #Ray intersects this #Box

        \param ray 		The ray along which to check for intersection
        \return 		\c true if there is an intersection
    */
    bool intersect(const Ray<N, T> &ray) const
    {
        // return true;
        T minT = ray.mint;
        T maxT = ray.maxt;

        for (auto i : range(N))
        {
            T invD = T(1) / ray.d[i];
            T t0   = (min[i] - ray.o[i]) * invD;
            T t1   = (max[i] - ray.o[i]) * invD;
            if (invD < T(0))
                std::swap(t0, t1);

            minT = t0 > minT ? t0 : minT;
            maxT = t1 < maxT ? t1 : maxT;
            if (maxT < minT)
                return false;
        }
        return true;
    }
};

template <typename T>
using Box2 = Box<2, T>; ///< 2D axis-aligned box
template <typename T>
using Box3 = Box<3, T>; ///< 3D axis-aligned box
template <typename T>
using Box4 = Box<4, T>; ///< 4D axis-aligned box

using Box2f = Box2<float>;    ///< 2D axis-aligned box (of floats)
using Box2d = Box2<double>;   ///< 2D axis-aligned box (of doubles)
using Box2i = Box2<int32_t>;  ///< 2D axis-aligned box (of signed 32-bit integers)
using Box2u = Box2<uint32_t>; ///< 2D axis-aligned box (of unsigned 32-bit integers
using Box2c = Box2<uint8_t>;  ///< 2D axis-aligned box (of unsigned 8-bit integers

using Box3f = Box3<float>;    ///< 3D axis-aligned box (of floats)
using Box3d = Box3<double>;   ///< 3D axis-aligned box (of doubles)
using Box3i = Box3<int32_t>;  ///< 3D axis-aligned box (of signed 32-bit integers)
using Box3u = Box3<uint32_t>; ///< 3D axis-aligned box (of unsigned 32-bit integers
using Box3c = Box3<uint8_t>;  ///< 3D axis-aligned box (of unsigned 8-bit integers

using Box4f = Box4<float>;    ///< 4D axis-aligned box (of floats)
using Box4d = Box4<double>;   ///< 4D axis-aligned box (of doubles)
using Box4i = Box4<int32_t>;  ///< 4D axis-aligned box (of signed 32-bit integers)
using Box4u = Box4<uint32_t>; ///< 4D axis-aligned box (of unsigned 32-bit integers
using Box4c = Box4<uint8_t>;  ///< 4D axis-aligned box (of unsigned 8-bit integers

/*! @}*/

/*!
    \file
    \brief Contains the implementation of a generic, N-dimensional axis-aligned #Box class.
*/