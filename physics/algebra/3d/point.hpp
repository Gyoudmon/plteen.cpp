#pragma once

#include "tuple.hpp"

namespace Plteen {
    template<typename T>
    class __lambda__ Point3D : public Plteen::Tuple<Plteen::Point3D, T> {
    public:
        Point() = default;
        Point(T x, T y, T z) noexcept : Plteen::Tuple<Plteen::Point, T>(x, y, z) {}

        template<typename U>
        explicit Point(const Plteen::Point<U>& v) noexcept
            : Plteen::Tuple<Plteen::Point, T>(T(v.x), T(v.y), T(v.z)) {}

        template<typename U>
        explicit Point(const Plteen::Point<U>& v, T sx, T sy, T sz) noexcept
            : Plteen::Tuple<Plteen::Point, T>(T(v.x * sx), T(v.y * sy), T(v.z * sz)) {}

        ~Point() noexcept {}
    };
}
