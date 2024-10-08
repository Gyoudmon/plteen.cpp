#include "tuxmon.hpp"

#include "../../../graphics/misc.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::Tuxmon::Tuxmon(bool walk_only)
    : IPlatformMotion(true, walk_only), Sprite(digimon_mascot_path("tuxmon", "")) {}

int Plteen::Tuxmon::get_initial_costume_index() {
    return this->costume_name_to_index("idle-0");
}

void Plteen::Tuxmon::on_heading_changed(double theta_rad, double vx, double vy, double prev_vr) {
    IPlatformMotion::dispatch_heading_event(theta_rad, vx, vy, prev_vr);
}

void Plteen::Tuxmon::on_walk(double theta_rad, double vx, double vy) {
    this->play("walk");
}

void Plteen::Tuxmon::on_jump(double theta_rad, double vx, double vy) {
    this->play("buttjump");
}

void Plteen::Tuxmon::on_motion_stopped() {
    this->stop();
    this->switch_to_costume(this->get_initial_costume_index());
}

void Plteen::Tuxmon::feed_flip_signs(double* hsgn, double* vsgn) {
    game_flip_to_signs(this->current_flip_status(), hsgn, vsgn);
}
