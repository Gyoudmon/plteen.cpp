#pragma once

#include "../algebra/point.hpp"
#include "../algebra/vector.hpp"
#include "port.hpp"

#include "../../datum/flonum.hpp"
#include "../mathematics.hpp"

namespace Plteen {
    template<typename T>
    class __lambda__ AABox {
    public:
        AABox() { this->invalidate(); }
        AABox(T w, T h) : ltdot({ T(), T() }), rbdot({ w, h }) {}
        AABox(T x, T y, T w, T h) : ltdot({ x, y }), rbdot({ x + w, y + h }) {}
        
        template<typename U>
        AABox(const Plteen::Point<U>& ltdot, const Plteen::Point<U>& rbdot) : ltdot(ltdot), rbdot(rbdot) {}

        template<typename U>
        explicit AABox(const Plteen::AABox<U>& box) : ltdot(box.ltdot), rbdot(box.rbdot) {}
        
        template<typename U>
        explicit AABox(const Plteen::AABox<U>& box, T sx, T sy) : ltdot{box.ltdot, sx, sy}, rbdot{box.rbdot, sx, sy} {}
        
        Plteen::AABox<T>& operator=(const Plteen::AABox<T>& box) { this->ltdot = box.ltdot; this->rbdot = box.rbdot; return (*this); }

        ~AABox() noexcept {}

    public:
        T x() const { return this->ltdot.x; }
        T y() const { return this->ltdot.y; }
        T width() const { return this->rbdot.x - this->ltdot.x; }
        T height() const { return this->rbdot.y - this->ltdot.y; }
        
        Plteen::Point<T> point_at(const Plteen::Port& port) const { return this->point_at(port.fx, port.fy); }

        template<typename Fl>
        Plteen::Point<Fl> point_at(Fl fx, Fl fy) const {
            return { Fl(this->ltdot.x) + Fl(this->rbdot.x - this->ltdot.x) * fx,
                     Fl(this->ltdot.y) + Fl(this->rbdot.y - this->ltdot.y) * fy };
        }

    public:
        bool is_empty() const {
            return (this->ltdot.x >= this->rbdot.x) && (this->ltdot.y >= this->rbdot.y);
        }

        bool contain(const Plteen::Point<T>& pt) {
            return rectangle_contain(this->ltdot.x, this->ltdot.y, this->rbdot.x, this->rbdot.y, pt.x, pt.y);
        }

        bool overlay(const Plteen::AABox<T>& box) {
            return rectangle_overlay(this->ltdot.x, this->ltdot.y, this->rbdot.x, this->rbdot.y,
                                        box.ltdot.x, box.ltdot.y, box.rbdot.x, box.rbdot.y);
        }

        void invalidate() {
            T vmin = std::numeric_limits<T>::lowest();
            T vmax = std::numeric_limits<T>::max();

            this->ltdot = { vmax, vmax };
            this->rbdot = { vmin, vmin };
        }

    public:
        bool operator==(const AABox<T>& box) const { return (this->ltdot == box.ltdot) && (this->rbdot == box.rbdot); }
        bool operator!=(const AABox<T>& box) const { return (this->ltdot != box.ltdot) || (this->rbdot != box.rbdot); }

        Plteen::AABox<T> operator+(const Plteen::Point<T>& p) const { Plteen::AABox<T> box(*this); box += p; return box; }
        Plteen::AABox<T> operator-(const Plteen::Point<T>& p) const { Plteen::AABox<T> box(*this); box -= p; return box; }
        Plteen::AABox<T> operator+(const Plteen::EuclideanVector<T>& v) const { Plteen::AABox<T> box(*this); box += v; return box; }
        Plteen::AABox<T> operator-(const Plteen::EuclideanVector<T>& v) const { Plteen::AABox<T> box(*this); box -= v; return box; }
        Plteen::AABox<T> operator+(const Plteen::AABox<T>& b) const { Plteen::AABox<T> box(*this); box += b; return box; }
        Plteen::AABox<T> operator*(const Plteen::AABox<T>& b) const { Plteen::AABox<T> box(*this); box *= b; return box; }
        Plteen::AABox<T> operator*(T s) const { Plteen::AABox<T> box(*this); box *= s; return box; }
        Plteen::AABox<T> operator/(T d) const { Plteen::AABox<T> box(*this); box /= d; return box; }

        friend inline Plteen::AABox<T> operator+(const Plteen::Point<T>& lhs, Plteen::AABox<T> rhs) { return rhs += lhs; }
        friend inline Plteen::AABox<T> operator*(T lhs, Plteen::AABox<T> rhs) { return rhs *= lhs; }
       
        Plteen::AABox<T>& operator+=(const Plteen::Point<T>& p) { this->ltdot += p; this->rbdot += p; return (*this); }
        Plteen::AABox<T>& operator-=(const Plteen::Point<T>& p) { this->ltdot -= p; this->rbdot -= p; return (*this); }
        Plteen::AABox<T>& operator*=(T s) { this->ltdot *= s; this->rbdot *= s; return (*this); }
        Plteen::AABox<T>& operator/=(T d) { this->ltdot /= d; this->rbdot /= d; return (*this); }

        Plteen::AABox<T>& operator+=(const Plteen::EuclideanVector<T>& v) {
            this->ltdot.x += v.x;
            this->ltdot.y += v.y;
            this->rbdot.x += v.x;
            this->rbdot.y += v.y;
            
            return (*this);
        }

        Plteen::AABox<T>& operator-=(const Plteen::EuclideanVector<T>& v) {
            this->ltdot.x -= v.x;
            this->ltdot.y -= v.y;
            this->rbdot.x -= v.x;
            this->rbdot.y -= v.y;
            
            return (*this);
        }

    public:
        Plteen::AABox<T>& operator+=(const Plteen::AABox<T>& b) {
            if (b.ltdot.x < this->ltdot.x) this->ltdot.x = b.ltdot.x;
            if (b.ltdot.y < this->ltdot.y) this->ltdot.y = b.ltdot.y;
            if (b.rbdot.x > this->rbdot.x) this->rbdot.x = b.rbdot.x;
            if (b.rbdot.y > this->rbdot.y) this->rbdot.y = b.rbdot.y;
            
            return (*this);
        }
        
        Plteen::AABox<T>& operator*=(const Plteen::AABox<T>& b) {
            if (b.ltdot.x > this->ltdot.x) this->ltdot.x = b.ltdot.x;
            if (b.ltdot.y > this->ltdot.y) this->ltdot.y = b.ltdot.y;
            if (b.rbdot.x < this->rbdot.x) this->rbdot.x = b.rbdot.x;
            if (b.rbdot.y < this->rbdot.y) this->rbdot.y = b.rbdot.y;
            
            return (*this);
        }

    public:
        std::string desc() const { return "[" + this->ltdot.desc() + " - " + this->rbdot.desc() + "]"; }

    public:
        Plteen::Point<T> ltdot;
        Plteen::Point<T> rbdot;
    };

    typedef AABox<float> Box;
}
