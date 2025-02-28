#pragma once

#include "tuple.hpp"
#include "point.hpp"

#include <complex>

namespace Plteen {
    template<typename T>
    class __lambda__ EuclideanVector : public Plteen::Tuple<Plteen::EuclideanVector, T> {
    public:
        static const Plteen::EuclideanVector<T> O;

    public:
        EuclideanVector() = default;
        EuclideanVector(T x, T y) noexcept : Tuple<Plteen::EuclideanVector, T>(x, y) {}
        EuclideanVector(const std::complex<T>& c) noexcept : Plteen::Tuple<Plteen::EuclideanVector, T>(T(c.real()), T(c.imag())) {}
        EuclideanVector(const Plteen::Point<T>& pt) noexcept : Tuple<Plteen::EuclideanVector, T>(pt.x, pt.y) {}
        EuclideanVector(const Plteen::Point<T>& sp, const Plteen::Point<T>& ep) noexcept
            : Tuple<Plteen::EuclideanVector, T>(ep.x - sp.x, ep.y - sp.y) {}

        template <typename U>
        explicit EuclideanVector(const std::complex<U>& c) noexcept
            : Plteen::Tuple<Plteen::EuclideanVector, T>(T(c.real()), T(c.imag())) {}

        template <typename U>
        explicit EuclideanVector(const Plteen::EuclideanVector<U>& v) noexcept
            : Plteen::Tuple<Plteen::EuclideanVector, T>(T(v.x), T(v.y)) {}

        template<typename U>
        explicit EuclideanVector(const Plteen::EuclideanVector<U>& v, T sx, T sy) noexcept
            : Plteen::Tuple<Plteen::EuclideanVector, T>(T(v.x * sx), T(v.y * sy)) {}

        ~EuclideanVector() noexcept {}
    };

    typedef Plteen::EuclideanVector<float> Vector;
    template<typename T> const Plteen::EuclideanVector<T> Plteen::EuclideanVector<T>::O = {};
}
