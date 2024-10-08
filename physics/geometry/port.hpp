#pragma once

#include "../../datum/string.hpp"

namespace Plteen {
    enum class MatterPort {
        LT, CT, RT, LC, CC, RC, LB, CB, RB,
        NW,  N, NE,  W,  C,  E, SW,  S, SE
    };
    
    struct __lambda__ Port {
    public:
        Port(float frac = 0.0F) : Port(frac, frac) {}
        Port(float fx, float fy) : fx(fx), fy(fy) {}
        Port(const Plteen::Port& a) : fx(a.fx), fy(a.fy) {}
        Port(const Plteen::MatterPort& name);

        void reset(float frac = 0.0F) { this->reset(frac, frac); }
        void reset(float fx, float fy) { this->fx = fx; this->fy = fy; }
        void reset(const Plteen::Port& a) { this->fx = a.fx; this->fy = a.fy; }
        void reset(const Plteen::MatterPort& name);

        Plteen::Port& operator=(float frac) { this->reset(frac); return (*this); }
        Plteen::Port& operator=(const Plteen::Port& c) { this->reset(c); return (*this); }
        Plteen::Port& operator=(const Plteen::MatterPort& name) { this->reset(name); return (*this); }

    public:
        bool is_zero() const { return (this->fx == 0.0F) && (this->fy == 0.0F); }
        
        friend inline bool operator!=(const Plteen::Port& lhs, const Plteen::Port& rhs)
            { return (lhs.fx != rhs.fx) || (lhs.fy != rhs.fy); }

	    friend inline bool operator==(const Plteen::Port& lhs, const Plteen::Port& rhs)
            { return (lhs.fx == rhs.fx) && (lhs.fy == rhs.fy); }

    public:
        std::string desc() const { return make_nstring("(%f, %f)", this->fx, this->fy); }
        
    public:
        float fx;
        float fy;
    };
}
