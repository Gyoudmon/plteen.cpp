#pragma once

#include "port.hpp"

#include "../mathematics.hpp"
#include "../../datum/flonum.hpp"

namespace Plteen {
    template<typename T>
    class __lambda__ AABox {
    public:
        AABox() { this->invalidate(); }
        AABox(T w, T h) : ltdot(T(), T()), rbdot(w, h) {}
        AABox(T x, T y, T w, T h) : ltdot(x, y), rbdot(x + w, y + h) {}
        
        AABox(const std::complex<T>& pt, T w, T h) : ltdot(pt), rbdot(pt.real() + w, pt.imag() + h) {}
        AABox(const std::complex<T>& ltdot, const std::complex<T>& rbdot) : ltdot(ltdot), rbdot(rbdot) {}

        template<typename U>
        explicit AABox(const Plteen::AABox<U>& box) : ltdot(box.ltdot), rbdot(box.rbdot) {}
        
        template<typename U>
        explicit AABox(const Plteen::AABox<U>& box, T sx, T sy) : AABox(box) { this->scale(sx, sy); }
        
        Plteen::AABox<T>& operator=(const Plteen::AABox<T>& box) { this->ltdot = box.ltdot; this->rbdot = box.rbdot; return (*this); }

        ~AABox() noexcept {}

    public:
        T x() const { return this->ltdot.real(); }
        T y() const { return this->ltdot.imag(); }
        T rx() const { return this->rbdot.real(); }
        T by() const { return this->rbdot.imag(); }
        T width() const  { return this->rbdot.real() - this->ltdot.real(); }
        T height() const { return this->rbdot.imag() - this->ltdot.imag(); }

        void scale(T sx, T sy) {
            this->ltdot = complex_scale(this->ltdot, sx, sy);
            this->rbdot = complex_scale(this->rbdot, sx, sy);
        }
        
        std::complex<T> point_at(const Plteen::Port& port) const { return this->point_at(port.fx, port.fy); }

        template<typename Fl>
        std::complex<Fl> point_at(Fl fx, Fl fy) const {
            auto size = this->rbdot - this->ltdot;

            return { Fl(this->ltdot.real()) + Fl(size.real()) * fx,
                     Fl(this->ltdot.imag()) + Fl(size.imag()) * fy };
        }

    public:
        bool is_empty() const { return (this->ltdot.real() >= this->rbdot.real()) && (this->ltdot.imag() >= this->rbdot.imag()); }
        bool contain(const std::complex<T>& pt) { return rectangle_contain(this->ltdot, this->rbdot, pt); }
        bool overlay(const Plteen::AABox<T>& box) { return rectangle_overlay(this->ltdot, this->rbdot, box.ltdot, box.rbdot); }

        void invalidate() {
            T vmin = std::numeric_limits<T>::lowest();
            T vmax = std::numeric_limits<T>::max();

            this->ltdot = { vmax, vmax };
            this->rbdot = { vmin, vmin };
        }

    public:
        bool operator==(const AABox<T>& box) const { return (this->ltdot == box.ltdot) && (this->rbdot == box.rbdot); }
        bool operator!=(const AABox<T>& box) const { return (this->ltdot != box.ltdot) || (this->rbdot != box.rbdot); }

        Plteen::AABox<T> operator+(const std::complex<T>& p) const { Plteen::AABox<T> box(*this); box += p; return box; }
        Plteen::AABox<T> operator-(const std::complex<T>& p) const { Plteen::AABox<T> box(*this); box -= p; return box; }
        Plteen::AABox<T> operator+(const Plteen::AABox<T>& b) const { Plteen::AABox<T> box(*this); box += b; return box; }
        Plteen::AABox<T> operator*(const Plteen::AABox<T>& b) const { Plteen::AABox<T> box(*this); box *= b; return box; }
        Plteen::AABox<T> operator*(T s) const { Plteen::AABox<T> box(*this); box *= s; return box; }
        Plteen::AABox<T> operator/(T d) const { Plteen::AABox<T> box(*this); box /= d; return box; }

        friend inline Plteen::AABox<T> operator+(const std::complex<T>& lhs, Plteen::AABox<T> rhs) { return rhs += lhs; }
        friend inline Plteen::AABox<T> operator*(T lhs, Plteen::AABox<T> rhs) { return rhs *= lhs; }
       
        Plteen::AABox<T>& operator+=(const std::complex<T>& p) { this->ltdot += p; this->rbdot += p; return (*this); }
        Plteen::AABox<T>& operator-=(const std::complex<T>& p) { this->ltdot -= p; this->rbdot -= p; return (*this); }
        Plteen::AABox<T>& operator*=(T s) { this->ltdot *= s; this->rbdot *= s; return (*this); }
        Plteen::AABox<T>& operator/=(T d) { this->ltdot /= d; this->rbdot /= d; return (*this); }

    public:
        Plteen::AABox<T>& operator+=(const Plteen::AABox<T>& b) {
            if (b.ltdot.real() < this->ltdot.real()) this->ltdot.real(b.ltdot.real());
            if (b.ltdot.imag() < this->ltdot.imag()) this->ltdot.imag(b.ltdot.imag());
            if (b.rbdot.real() > this->rbdot.real()) this->rbdot.real(b.rbdot.real());
            if (b.rbdot.imag() > this->rbdot.imag()) this->rbdot.imag(b.rbdot.imag());
            
            return (*this);
        }
        
        Plteen::AABox<T>& operator*=(const Plteen::AABox<T>& b) {
            if (b.ltdot.real() > this->ltdot.real()) this->ltdot.real(b.ltdot.real());
            if (b.ltdot.imag() > this->ltdot.imag()) this->ltdot.imag(b.ltdot.imag());
            if (b.rbdot.real() < this->rbdot.real()) this->rbdot.real(b.rbdot.real());
            if (b.rbdot.imag() < this->rbdot.imag()) this->rbdot.imag(b.rbdot.imag());
            
            return (*this);
        }

    public:
        std::string desc() const {
            return "[(" + std::to_string(this->ltdot.real()) + ", " + std::to_string(this->ltdot.imag()) + ") - "
                  + "(" + std::to_string(this->rbdot.real()) + ", " + std::to_string(this->rbdot.imag()) + ")]";
        }

    public:
        std::complex<T> ltdot;
        std::complex<T> rbdot;
    };

    typedef AABox<float> Box;
}
