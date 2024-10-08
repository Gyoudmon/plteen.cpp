#pragma once

#include "../../sheet.hpp"
#include "../../../../physics/motion/map2d.hpp"

#include "../creature.hpp"

/* For the Universe LPC Spritesheet Generator */
/* https://sanderfrenken.github.io/Universal-LPC-Spritesheet-Character-Generator/index.html */
/* https://lpc.opengameart.org/static/LPC-Style-Guide/build/index.html */

namespace Plteen {
    class SpriteULPCSheet : public Plteen::SpriteGridSheet, public Plteen::ICreature, protected Plteen::I4WayMotion {
    public:
        SpriteULPCSheet(const std::string& pathname);

    protected:
        void on_heading_changed(double theta_rad, double vx, double vy, double prev_vr) override;
        void on_nward(double theta_rad, double vx, double vy) override;
        void on_eward(double theta_rad, double vx, double vy) override;
        void on_sward(double theta_rad, double vx, double vy) override;
        void on_wward(double theta_rad, double vx, double vy) override;

    protected:
        const char* costume_index_to_name(size_t idx) override;
    };
}
