#include "../matter.hpp"
#include "../plane.hpp"

#include "position.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::Position::Position(const Plteen::Position& pos) : dot(pos.dot), offset(pos.offset) {
    this->xtarget = pos.xtarget;
    this->ytarget = pos.ytarget;
}

Position& Plteen::Position::operator=(const Plteen::Position& pos) {
    this->dot = pos.dot;
    this->xtarget = pos.xtarget;
    this->ytarget = pos.ytarget;
    this->offset = pos.offset;

    return *this;
}

/*************************************************************************************************/
std::complex<float> Plteen::Position::calculate_point() const {
    std::complex<float> pos;

    if (this->xtarget == nullptr) {
        pos.real(this->dot.fx);
        pos.imag(this->dot.fy);
    } else if (this->ytarget == nullptr) {
        IPlane* master = this->xtarget->master();

        if (master != nullptr) {
            pos = master->get_matter_location(const_cast<IMatter*>(this->xtarget), this->dot);
        }
    } else {
        IPlane* xmaster = this->xtarget->master();
        IPlane* ymaster = this->ytarget->master();
        std::complex<float> xdot, ydot;

        if (xmaster != nullptr) {
            xdot = xmaster->get_matter_location(const_cast<IMatter*>(this->xtarget), this->dot);
        }
        
        if (ymaster != nullptr) {
            ydot = ymaster->get_matter_location(const_cast<IMatter*>(this->ytarget), this->dot);
        }

        pos.real(xdot.real());
        pos.imag(ydot.imag());
    }

    pos += this->offset;

    return pos;
}

std::string Plteen::Position::desc() const {
    std::string description;

    if (this->xtarget == nullptr) {
        description = this->dot.desc();
    } else if (this->ytarget == nullptr) {
        description = this->dot.desc() + "@" + const_cast<IMatter*>(this->xtarget)->name();
    } else {
        description = "(" + std::to_string(this->dot.fx) + "@" + const_cast<IMatter*>(this->xtarget)->name() + ", "
                          + std::to_string(this->dot.fy) + "@" + const_cast<IMatter*>(this->ytarget)->name() + ")";
    }

    return description; 
}
