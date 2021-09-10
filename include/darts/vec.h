/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include "linalg.h"
#include <fmt/format.h>

/** \addtogroup group_math
 *  @{
 */

template <int N, class T>
using Vec = linalg::vec<T, N>; ///< Generic \p N dimensional vector
template <class T, int M, int N>
using Mat = linalg::mat<T, M, N>; ///< Generic \p M x \p N matrix

template <class T>
using Vec2 = Vec<2, T>; ///< 2D vector of type T
template <class T>
using Vec3 = Vec<3, T>; ///< 3D vector of type T
template <class T>
using Vec4 = Vec<4, T>; ///< 4D vector of type T

template <class T>
using Color3 = Vec<3, T>; ///< RGB color of type T
template <class T>
using Color4 = Vec<4, T>; ///< RGBA color of type T

using Vec2f = Vec2<float>;         ///< 2D float vector
using Vec2d = Vec2<double>;        ///< 2D double vector
using Vec2i = Vec2<std::int32_t>;  ///< 2D integer vector (32-bit)
using Vec2u = Vec2<std::uint32_t>; ///< 2D unsigned vector (32-bit)
using Vec2c = Vec2<std::uint8_t>;  ///< 2D character vector (8-bit unsigned integers)

using Vec3f   = Vec3<float>;         ///< 3D float vector
using Vec3d   = Vec3<double>;        ///< 3D double vector
using Vec3i   = Vec3<std::int32_t>;  ///< 3D integer vector (32-bit)
using Vec3u   = Vec3<std::uint32_t>; ///< 3D unsigned vector (32-bit)
using Vec3c   = Vec3<std::uint8_t>;  ///< 3D character vector (8-bit unsigned integers)
using Color3f = Vec3<float>;         ///< RGB float color
using Color3d = Vec3<double>;        ///< RGB double color
using Color3u = Vec3<std::uint32_t>; ///< RGB 32-bit unsigned integer color
using Color3c = Vec3<std::uint8_t>;  ///< RGB 8-bit unsigned integer

using Vec4f = Vec4<float>;         ///< 4D float vector
using Vec4d = Vec4<double>;        ///< 4D double vector
using Vec4i = Vec4<std::int32_t>;  ///< 4D integer vector (32-bit)
using Vec4u = Vec4<std::uint32_t>; ///< 4D unsigned vector (32-bit)
using Vec4c = Vec4<std::uint8_t>;  ///< 4D character vector (8-bit unsigned integers)

using Color4f = Vec4<float>;         ///< RGBA float color
using Color4d = Vec4<double>;        ///< RGBA double color
using Color4u = Vec4<std::uint32_t>; ///< RGBA 32-bit unsigned integer color
using Color4c = Vec4<std::uint8_t>;  ///< RGBA 8-bit unsigned integer

template <class T>
using Mat22 = linalg::mat<T, 2, 2>; ///< A generic 2x2 matrix
template <class T>
using Mat33 = linalg::mat<T, 3, 3>; ///< A generic 3x3 matrix
template <class T>
using Mat44 = linalg::mat<T, 4, 4>; ///< A generic 4x4 matrix

using Mat22f = Mat22<float>;  ///< 2x2 float matrix
using Mat22d = Mat22<double>; ///< 2x2 double matrix
using Mat33f = Mat33<float>;  ///< 3x3 float matrix
using Mat33d = Mat33<double>; ///< 3x3 double matrix
using Mat44f = Mat44<float>;  ///< 4x4 float matrix
using Mat44d = Mat44<double>; ///< 4x4 double matrix

/**
 * @brief  Construct an orthonormal coordinate system given one vector \p a.
 *
 * @param a [in]     The coordinate system's local z axis direction.
 * @param b [out]    A local x-axis orthogonal to a.
 * @param c [out]    A local y-axis orthogonal to both a and b.
 */
template <typename T>
void coordinate_system(const Vec3<T> &a, Vec3<T> &b, Vec3<T> &c)
{
    if (std::abs(a.x) > std::abs(a.y))
    {
        T inv_len = T(1) / std::sqrt(a.x * a.x + a.z * a.z);
        c         = Vec3<T>(a.z * inv_len, 0, -a.x * inv_len);
    }
    else
    {
        T inv_len = T(1) / std::sqrt(a.y * a.y + a.z * a.z);
        c         = Vec3<T>(0, a.z * inv_len, -a.y * inv_len);
    }
    b = cross(c, a);
}

/// Convert from linear RGB to sRGB
inline Color3f to_sRGB(const Color3f &c)
{
    Color3f result;

    for (int i = 0; i < 3; ++i)
    {
        float value = c[i];
        if (value <= 0.0031308f)
            result[i] = 12.92f * value;
        else
            result[i] = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
    }

    return result;
}

/// Convert from sRGB to linear RGB
inline Color3f to_linear_RGB(const Color3f &c)
{
    Color3f result;

    for (int i = 0; i < 3; ++i)
    {
        float value = c[i];

        if (value <= 0.04045f)
            result[i] = value * (1.0f / 12.92f);
        else
            result[i] = std::pow((value + 0.055f) * (1.0f / 1.055f), 2.4f);
    }

    return result;
}

/// Check if the color vector contains a NaN/Inf/negative value
inline bool is_valid_color(const Color3f &c)
{
    for (int i = 0; i < 3; ++i)
    {
        float value = c[i];
        if (value < 0 || !std::isfinite(value))
            return false;
    }
    return true;
}

/// Return the associated luminance
inline float luminance(const Color3f &c)
{
    return dot(c, {0.212671f, 0.715160f, 0.072169f});
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//
// Base class for both vec and mat fmtlib formatters.
//
// Based on the great blog tutorial: https://wgml.pl/blog/formatting-user-defined-types-fmt.html
//
template <typename V, typename T, bool Newline>
struct vecmat_formatter
{
    using underlying_formatter_type = fmt::formatter<T>;

    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return underlying_formatter.parse(ctx);
    }

    template <typename FormatContext>
    auto format(const V &v, FormatContext &ctx)
    {
        fmt::format_to(ctx.out(), "{{");
        auto it = begin(v);
        while (true)
        {
            ctx.advance_to(underlying_formatter.format(*it, ctx));
            if (++it == end(v))
            {
                fmt::format_to(ctx.out(), "}}");
                break;
            }
            else
                fmt::format_to(ctx.out(), ",{} ", Newline ? "\n" : "");
        }
        return ctx.out();
    }

protected:
    underlying_formatter_type underlying_formatter;
};

template <typename T, int N>
struct fmt::formatter<linalg::vec<T, N>> : public vecmat_formatter<linalg::vec<T, N>, T, false>
{
};

template <typename T, int M, int N>
struct fmt::formatter<linalg::mat<T, M, N>> : public vecmat_formatter<linalg::mat<T, M, N>, linalg::vec<T, N>, true>
{
};

#ifdef DARTS_IOSTREAMS
#include <iomanip>
#include <iostream>
template <class C, int N, class T>
std::basic_ostream<C> &operator<<(std::basic_ostream<C> &out, const Vec<N, T> &v)
{
    std::ios_base::fmtflags oldFlags = out.flags();
    auto                    width    = out.precision() + 2;

    out.setf(std::ios_base::right);
    if (!(out.flags() & std::ios_base::scientific))
        out.setf(std::ios_base::fixed);
    width += 5;

    out << '{';
    for (size_t i = 0; i < N - 1; ++i)
        out << std::setw(width) << v[i] << ',';
    out << std::setw(width) << v[N - 1] << '}';

    out.flags(oldFlags);
    return out;
}

template <class C, class T>
std::basic_ostream<C> &operator<<(std::basic_ostream<C> &s, const Mat44<T> &m)
{
    return s << "{" << m[0] << ",\n " << m[1] << ",\n " << m[2] << ",\n " << m[3] << "}";
}
#endif

#endif

/** @}*/

/**
    \file
    \brief Contains various classes for linear algebra: vectors, matrices, rays, axis-aligned bounding boxes.
*/
