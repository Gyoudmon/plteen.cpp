#include "animal.hpp"

#include <filesystem>

#include "../../../../datum/path.hpp"
#include "../../../../physics/random.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::Animal::Animal(const std::string& fullpath) : Sprite(fullpath) {}

Plteen::Cat::Cat() : Animal(digimon_mascot_path("Cat", "", "trail/Animals")) {
    this->set_virtual_canvas(48.0F, 48.0F);
}

Plteen::Cow::Cow() : Animal(digimon_mascot_path("Cow", "", "trail/Animals")) {
    this->set_virtual_canvas(96.0F, 96.0F);
}

Plteen::Rooster::Rooster() : Animal(digimon_mascot_path("Rooster", "", "trail/Animals")) {
    this->set_virtual_canvas(42.0F, 42.0F);
}

Plteen::Pigeon::Pigeon() : Animal(digimon_mascot_path("Pigeon", "", "trail/Animals")) {
    this->set_virtual_canvas(32.0F, 32.0F);
}

/*************************************************************************************************/
void Plteen::Animal::on_costumes_load() {
    this->play("walk_s");
}

void Plteen::Animal::on_heading_changed(double theta_rad, double vx, double vy, double prev_vr) {
    I8WayMotion::dispatch_heading_event(theta_rad, vx, vy, prev_vr);
}

void Plteen::Animal::on_eward(double theta_rad, double vx, double vy) {
    this->play("walk_e_");
}

void Plteen::Animal::on_wward(double theta_rad, double vx, double vy) {
    this->play("walk_w_");
}

void Plteen::Animal::on_sward(double theta_rad, double vx, double vy) {
    this->play("walk_s_");
}

void Plteen::Animal::on_nward(double theta_rad, double vx, double vy) {
    this->play("walk_n_");
}

void Plteen::Animal::on_esward(double theta_rad, double vx, double vy) {
    this->play("walk_es_");
}

void Plteen::Animal::on_enward(double theta_rad, double vx, double vy) {
    this->play("walk_en_");
}

void Plteen::Animal::on_wsward(double theta_rad, double vx, double vy) {
    this->play("walk_ws_");
}

void Plteen::Animal::on_wnward(double theta_rad, double vx, double vy) {
    this->play("walk_wn_");
}
