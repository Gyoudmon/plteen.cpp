#pragma once

#include <string>

#include "../../../datum/flonum.hpp"
#include "../../../datum/except.hpp"

namespace Plteen {
    template<template<typename> class Child, typename T>
    class __lambda__ Tuple3D {
    public:
        Tuple3D() noexcept : Tuple3D(T(), T(), T()) {}
        Tuple3D(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
        Tuple3D(const Child<T>& c) noexcept : x(c.x), y(c.y), z(c.z) {}

        Child<T>& operator=(const Child<T>& c) noexcept {
            this->x = c.x;
            this->y = c.y;
            this->z = c.z;
           
            return static_cast<Child<T>&>(*this);
        }

        ~Tuple3D() noexcept {}

    public:
        bool is_zero() const noexcept { return (this->x == T(0)) && (this->y == T(0) && (this->z == T(0)); }
        bool has_nan() const noexcept { return flisnan(this->x) || flisnan(this->y) || flisnan(this->z); }
        bool okay() const noexcept { return !this->has_nan(); }

        bool operator==(const Child<T>& c) const noexcept
        { return (this->x == c.x) && (this->y == c.y) && (this->z == c.z); }

        bool operator!=(const Child<T>& c) const noexcept
        { return (this->x != c.x) || (this->y != c.y) || (this->z != c.y); }

    public:
        T operator[](size_t i) const {
            switch (i) {
            case 0: return this->x;
            case 1: return this->y;
            case 2: return this->z;
            default: raise_range_error("index too large for a 3D Tuple");
            }
        }
       
        T& operator[](size_t i) {
            switch (i) {
            case 0: return this->x;
            case 1: return this->y;
            case 2: return this->z;
            default: raise_range_error("index too large for a 3D Tuple");
            }
        }

    public:
        Child<T> operator+(const Child<T>& c) const noexcept
        { return { this->x + c.x, this->y + c.y, this->z + c.z }; }
        
        Child<T> operator-(const Child<T>& c) const noexcept
        { return { this->x - c.x, this->y - c.y, this->z - c.z }; }
        
        Child<T> operator-() const noexcept
        { return { - this->x, - this->y, - this->z }; }
        
        Child<T> operator*(T s) const noexcept
        { return { this->x * s, this->y * s, this->z * s }; }
        
        Child<T> operator/(T d) const noexcept
        { return { this->x / d, this->y / d, this->z / d }; }

        friend inline Child<T> operator+(T lhs, const Child<T>& rhs) noexcept { return rhs *= lhs; }

        Child<T>& operator+=(const Child<T>& c) noexcept {
            this->x += c.x;
            this->y += c.y;
            this->z += c.z;
           
            return static_cast<Child<T>&>(*this);
        }
       
        Child<T>& operator-=(const Child<T>& c) noexcept {
            this->x -= c.x;
            this->y -= c.y;
            this->z -= c.z;
            
            return static_cast<Child<T>&>(*this);
        }
       
        Child<T>& operator*=(T s) noexcept {
            this->x *= s;
            this->y *= s;
            this->z *= s;
            
            return static_cast<Child<T>&>(*this);
        }

        Child<T>& operator/=(T d) noexcept {
            this->x /= d;
            this->y /= d;
            this->z /= d;
           
            return static_cast<Child<T>&>(*this);
        }

    public:
        std::string desc() const noexcept {
            return "("  + std::to_string(this->x) + ", "
                        + std::to_string(this->y) + ", "
                        + std::to_string(this->z) + ")";
        }

    public:
        T x;
        T y;
        T z;
    };
}
