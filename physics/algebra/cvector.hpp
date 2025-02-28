#pragma once

#include <complex>

namespace Plteen {
    #define _X(c) c.real()
    #define _Y(c) c.imag()
    #define set_X(c, v) c.real(v)
    #define set_Y(c, v) c.imag(v)

    template<class T> using Point  = std::complex<T>;
    template<class T> using Vector = std::complex<T>;

    typedef Plteen::Point<float>  cPoint;
    typedef Plteen::Vector<float> cVector;

    static const std::complex<float> cO(0.0F, 0.0F);
}
