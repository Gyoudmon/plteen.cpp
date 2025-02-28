#pragma once

#include "tuple3d.hpp"

namespace Plteen {
    template<typename T>
    class __lambda__ Point3D : public Plteen::Tuple3D<Plteen::Point3D, T> {
    public:
        static const Plteen::Point3D<T> O;

    public:
        Point3D() = default;
        Point3D(T x, T y, T z) noexcept : Plteen::Tuple<Plteen::Point3D, T>(x, y, z) {}

        template<typename U>
        explicit Point3D(const Plteen::Point3D<U>& v) noexcept
            : Plteen::Tuple<Plteen::Point3D, T>(T(v.x), T(v.y), T(v.z)) {}

        template<typename U>
        explicit Point3D(const Plteen::Point3D<U>& v, T sx, T sy, T sz) noexcept
            : Plteen::Tuple<Plteen::Point3D, T>(T(v.x * sx), T(v.y * sy), T(v.z * sz)) {}

        ~Point3D() noexcept {}
    };

    template<typename T> const Plteen::Point3D<T> Plteen::Point3D<T>::O = {};
}
