#pragma once

#include "tuple.hpp"

#include <complex>

namespace Plteen {
    template<typename T>
    class __lambda__ Point : public Plteen::Tuple<Plteen::Point, T> {
    public:
        static const Plteen::Point<T> O;

    public:
        Point() = default;
        Point(T x, T y) noexcept : Plteen::Tuple<Plteen::Point, T>(x, y) {}
        Point(const std::complex<T>& c) noexcept : Plteen::Tuple<Plteen::Point, T>(T(c.real()), T(c.imag())) {}

        template <typename U>
        explicit Point(const std::complex<U>& c) noexcept
            : Plteen::Tuple<Plteen::Point, T>(T(c.real()), T(c.imag())) {}

        template<typename U>
        explicit Point(const Plteen::Point<U>& v) noexcept
            : Plteen::Tuple<Plteen::Point, T>(T(v.x), T(v.y)) {}

        template<typename U>
        explicit Point(const Plteen::Point<U>& v, T sx, T sy) noexcept
            : Plteen::Tuple<Plteen::Point, T>(T(v.x * sx), T(v.y * sy)) {}

        ~Point() noexcept {}
    };

    typedef Plteen::Point<float> Dot;
    template<typename T> const Plteen::Point<T> Plteen::Point<T>::O = {};
}
