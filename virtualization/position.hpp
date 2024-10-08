#pragma once

#include "../forward.hpp"

#include "../physics/algebra/point.hpp"
#include "../physics/algebra/vector.hpp"
#include "../physics/geometry/port.hpp"

namespace Plteen {
    class __lambda__ Position {
    public:
        Position() : Position(0.0F, 0.0F) {}
        Position(float x, float y) : dot(x, y) {}
        Position(const Plteen::Dot& dot) : Position(dot.x, dot.y) {}
        Position(const Plteen::Vector& vec) : Position(vec.x, vec.y) {}

        template<typename T>
        Position(T x, T y) : Position(float(x), float(y)) {}

        Position(const Plteen::IMatter* target, const Plteen::Port& port)
            : dot(port), xtarget(target) {}

        Position(const Plteen::IMatter* xtarget, const Plteen::Port& xport,
                    const Plteen::IMatter* ytarget, const Plteen::Port& yport)
            : dot(xport.fx, yport.fy), xtarget(xtarget), ytarget(ytarget) {}

        Position(const Plteen::Position& pos);

        ~Position() noexcept { /* instances of Position don't own matters */ }

    public:
        Plteen::Position& operator=(const Plteen::Position& c);

    public:
        Plteen::Dot calculate_point() const;
        Plteen::Vector get_offset() const { return this->offset; }
        void set_offset(const Plteen::Vector& vec) { this->offset = vec; }

    public:
        std::string desc() const;

    private:
        Plteen::Port dot; // also serves as an absolute location
        const Plteen::IMatter* xtarget = nullptr;
        const Plteen::IMatter* ytarget = nullptr;
        Plteen::Vector offset;
    };
}
