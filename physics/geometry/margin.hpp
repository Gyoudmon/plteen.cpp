#pragma once

#include "../algebra/point.hpp"
#include "../algebra/vector.hpp"
#include "port.hpp"

#include "../../datum/flonum.hpp"
#include "../mathematics.hpp"

namespace Plteen {
    class __lambda__ Margin {
    public:
        Margin() : Margin(0.0F) {}
        Margin(float s) : Margin(s, s, s, s) {}
        Margin(float v, float h) : Margin(v, h, v, h) {}
        Margin(float t, float r, float b, float l) : top(t), right(r), bottom(b), left(l) {}
        Margin(const Plteen::Margin& m) : top(m.top), right(m.right), bottom(m.bottom), left(m.left) {}
        Margin(const Plteen::Margin& m, float sx, float sy) : Margin(m) { this->scale(sx, sy); }
        
        Plteen::Margin& operator=(const Plteen::Margin& m) {
            this->left = m.left;
            this->top = m.top;
            this->bottom = m.bottom;
            this->right = m.right;
           
            return (*this);
        }

        ~Margin() noexcept {}

    public:
        Plteen::Dot ltdot() const { return { this->left, this->top }; }
        Plteen::Dot rtdot() const { return { this->right, this->top }; }
        Plteen::Dot lbdot() const { return { this->left, this->bottom }; }
        Plteen::Dot rbdot() const { return { this->right, this->top }; }

        float vertical() const { return this->top + this->bottom; }
        float horizon() const { return this->left + this->right; }

    public:
        bool is_zero() const {
            return (this->left == 0.0F) && (this->right == 0.0F)
                    && (this->top == 0.0F) && (this->bottom == 0.0F);
        }

        Plteen::Margin& scale(float s) { return this->scale(s, s); }
        Plteen::Margin& scale(float sx, float sy);

    public:
        bool operator!=(const Margin& m) const { return !(this->operator==(m)); }
        bool operator==(const Margin& m) const {
            return (this->top == m.top) && (this->right == m.right)
                    && (this->bottom == m.bottom) && (this->left == m.left);
        }
        
        Plteen::Margin operator*(float s) const { Plteen::Margin m(*this); m *= s; return m; }
        Plteen::Margin operator/(float d) const { Plteen::Margin m(*this); m /= d; return m; }

        friend inline Plteen::Margin operator*(float lhs, Plteen::Margin rhs) { return rhs *= lhs; }

        Plteen::Margin& operator*=(float s) { return this->scale(s, s); }
        Plteen::Margin& operator/=(float d) { return this->scale(1.0F / d); }

    public:
        float top = 0.0F;
        float right = 0.0F;
        float bottom = 0.0F;
        float left = 0.0F;
    };
}
