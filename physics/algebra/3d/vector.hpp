#pragma once

#include "tuple.hpp"
#include "point.hpp"

namespace Plteen {
    template<typename T>
    class __lambda__ Vector3D : public Plteen::Tuple<Plteen::Vector3D, T> {
    public:
        Vector3D() = default;
        Vector3D(T x, T y, T z) noexcept : Tuple<Plteen::Vector3D, T>(x, y, z) {}
        Vector3D(const Plteen::Point3D<T>& pt) noexcept : Tuple<Plteen::Vector3D, T>(pt.x, pt.y, pt.z) {}
        Vector3D(const Plteen::Point3D<T>& sp, const Plteen::Point3D<T>& ep) noexcept
            : Tuple<Plteen::Vector3D, T>(ep.x - sp.x, ep.y - sp.y, ep.z - sp.z) {}

        template <typename U>
        explicit Vector3D(const Plteen::Vector3D<U>& v) noexcept
            : Plteen::Tuple<Plteen::Vector3D, U>(T(v.x), T(v.y), T(v.z)) {}

        template<typename U>
        explicit Vector3D(const Plteen::Vector3D<U>& v, T sx, T sy, T sz) noexcept
            : Plteen::Tuple<Plteen::Vector3D, T>(T(v.x * sx), T(v.y * sy), T(v.z * sz)) {}

        ~Vector3D() noexcept {}
    };
}
