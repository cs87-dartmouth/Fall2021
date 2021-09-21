/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <cstdio> // for size_t
#include <darts/fwd.h>
#include <darts/vec.h>
#include <filesystem/fwd.h>
#include <map>       // for map
#include <memory>    // for shared_ptr and make_shared
#include <set>       // for set
#include <stdexcept> // for runtime_error
#include <string>    // for operator+, basic_string
#include <utility>   // for make_pair, pair
#include <vector>    // for vector

#include <nlohmann/json.hpp>

// #define FMT_HEADER_ONLY
#include <fmt/color.h>
#include <fmt/format.h>

#include <spdlog/spdlog.h>

// bringing standard library objects in scope
using std::clamp;
using std::make_pair;
using std::make_shared;
using std::make_unique;
using std::map;
using std::max;
using std::min;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

// A few useful constants
#undef M_PI

#define M_PI         3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

/// Error signaling unimplemented features
#define put_your_code_here(txt)                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        static bool been_here = false;                                                                                 \
        if (!been_here)                                                                                                \
        {                                                                                                              \
            been_here = true;                                                                                          \
            spdlog::warn("{}() not (fully) implemented at {}:{}.\n    msg: {}", __FUNCTION__, __FILE__, __LINE__,      \
                         txt);                                                                                         \
        }                                                                                                              \
    } while (0);

/// Initialize darts before parsing or rendering a scene
void darts_init(int verbosity = spdlog::level::info);

/// Prints a success message (fmt::print style) and flushes the output
template <typename... Args>
inline void success(const char *msg, const Args &...args)
{
    fmt::print(fg(fmt::color::green), msg, args...);
    fflush(stdout);
}

/*!
    Python-style range: iterates from min to max in range-based for loops

    To use:

    \code{.cpp}
    for(int i = 0; i < 100; i++) { ... }             // old way
    for(auto i : range(100))     { ... }             // new way

    for(int i = 10; i < 100; i+=2)  { ... }          // old way
    for(auto i : range(10, 100, 2)) { ... }          // new way

    for(float i = 3.5f; i > 1.5f; i-=0.01f) { ... } // old way
    for(auto i : range(3.5f, 1.5f, -0.01f)) { ... } // new way
    \endcode
*/
template <typename T>
class Range
{
public:
    /// Standard iterator support for #Range
    class Iterator
    {
    public:
        Iterator(T pos, T step) : m_pos(pos), m_step(step)
        {
        }

        bool operator!=(const Iterator &o) const
        {
            return (o.m_pos - m_pos) * m_step > T(0);
        }
        Iterator &operator++()
        {
            m_pos += m_step;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator copy(*this);
                     operator++();
            return copy;
        }
        T operator*() const
        {
            return m_pos;
        }

    private:
        T m_pos, m_step;
    };

    /// Construct an iterable range from \p start to \p end in increments of \p step
    Range(T start, T end, T step = T(1)) : m_start(start), m_end(end), m_step(step)
    {
    }

    Iterator begin() const
    {
        return Iterator(m_start, m_step);
    }
    Iterator end() const
    {
        return Iterator(m_end, m_step);
    }

private:
    T m_start, m_end, m_step;
};

/// Construct a Python-style range from a single parameter \p end
template <typename T>
Range<T> range(T end)
{
    return Range<T>(T(0), end, T(1));
}

/// Construct a Python-style range from \p start, up to but excluding \p end, in incremenents of \p step
template <typename T>
Range<T> range(T start, T end, T step = T(1))
{
    return Range<T>(start, end, step);
}

/*!
    Linear interpolation.

    Linearly interpolates between \a a and \a b, using parameter \a t.

    \tparam T    type for start and end points, and return value
    \tparam S    type for interpolation parameter
    \param a     Start point
    \param b     End point
    \param t     A blending factor of \a a and \a b.
    \return      Linear interpolation of \a a and \a b -
                 a value between \a a and \a b if \a t is between 0 and 1.
*/
template <typename T, typename S>
inline T lerp(T a, T b, S t)
{
    return T((S(1) - t) * a + t * b);
}

/// Always-positive modulo operation
template <typename T>
inline T mod(T a, T b)
{
    int n = (int)(a / b);
    a -= n * b;
    if (a < 0)
        a += b;
    return a;
}

/// Convert radians to degrees
inline float rad2deg(float value)
{
    return value * (180.0f / M_PI);
}

/// Convert degrees to radians
inline float deg2rad(float value)
{
    return value * (M_PI / 180.0f);
}

/*!
    Return the sine and cosine in a single function call.

    In C++17 you can unpack the result using `auto [s,c] = sincos(theta)`.
*/
template <typename T>
std::pair<T, T> sincos(T arg)
{
    return {std::sin(arg), std::cos(arg)};
}

/*!
    Convert spherical (phi,theta) coordinates to a unit direction in Cartesian coordinates

    \param phi_theta    The spherical angles with \f$\phi \in [0,2\pi)\f$ and \f$\theta \in [0, \pi]\f$.
                        \f$\theta = 0 \mapsto (0,0,1)\f$ and \f$\theta = 0 \mapsto (0,0,-1)\f$.
    \return             The corresponding unit-length direction vector in Cartesian coordinates.
 */
Vec3f spherical_coordinates_to_direction(const Vec2f &phi_theta);

/*!
    Convert a unit direction from Cartesian coordinates to spherical (phi,theta) coordinates

    \param dir  The direction vector in Cartesian coordinates (assumed to be unit length)
    \return     The spherical angles with \f$\phi \in [0,2\pi)\f$ and \f$\theta \in [0, \pi]\f$.
                \f$\theta = 0 \mapsto (0,0,1)\f$ and \f$\theta = 0 \mapsto (0,0,-1)\f$.
*/
Vec2f direction_to_spherical_coordinates(const Vec3f &dir);

/// Converts a unit direction to a UV coordinate using #direction_to_spherical_coordinates
Vec2f direction_to_spherical_uv(const Vec3f &p);

/// Converts a spherical UV coordinate to a unit direction using #spherical_coordinates_to_direction
Vec3f spherical_uv_to_direction(const Vec2f &uv);

/*!
    Calculates the unpolarized fresnel reflection coefficient for a dielectric material. Handles incidence from either
    side (i.e. `cos_theta_i<0` is allowed).

    \param cos_theta_i  Cosine of the angle between the normal and the incident ray
    \param ext_ior      Refractive index of the side that contains the surface normal
    \param int_ior      Refractive index of the interior
*/
float fresnel(float cos_theta_i, float ext_ior, float int_ior);

/// Convert a time value in milliseconds into a human-readable string
std::string time_string(double time, int precision = 2);

/// Indent a string by the specified number of spaces
std::string indent(const std::string &string, int amount = 2);

/// Simple exception class, which stores a human-readable error description
class DartsException : public std::runtime_error
{
public:
    /// Variadic template constructor to support fmt::format-style arguments
    template <typename... Args>
    DartsException(const char *fmt, const Args &...args) : std::runtime_error(fmt::format(fmt, args...))
    {
    }
};

/*!
    Return the global file resolver instance.

    This class is used to locate resource files (e.g. mesh or texture files) referenced by a scene being loaded.
*/
filesystem::resolver &get_file_resolver();

// Ray tracing statistics
extern std::atomic<uint64_t> intersection_tests; ///< Global counter of the number of ray intersection tests
extern std::atomic<uint64_t> rays_traced;        ///< Global counter of the number of rays traced

#define INCREMENT_INTERSECTION_TESTS intersection_tests++
#define INCREMENT_TRACED_RAYS        rays_traced++

// Matplotlib-style false-color maps
Color3f viridis(float t);
Color3f inferno(float t);
Color3f magma(float t);
Color3f plasma(float t);

/*!
    \file
    \brief Common include files and various utility functions.
*/