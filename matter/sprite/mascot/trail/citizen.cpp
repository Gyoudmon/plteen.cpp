#include "citizen.hpp"

#include <filesystem>

#include "../../../../datum/path.hpp"
#include "../../../../physics/random.hpp"

using namespace Plteen;
using namespace std::filesystem;

/*************************************************************************************************/
static std::vector<std::string> list_citizen_names(const char* subdir) {
    std::string rootdir = digimon_mascot_subdir(subdir);
    std::vector<std::string> names;

    for (auto entry : directory_iterator(rootdir)) {
        path self = entry.path();

        if (entry.is_directory()) {
            names.push_back(self.filename().string());
        }
    }

    return names;
}

static size_t citizen_name_count(const char* subdir) {
    std::string rootdir = digimon_mascot_subdir(subdir);
    size_t n = 0;

    for (auto entry : directory_iterator(rootdir)) {
        path self = entry.path();

        if (entry.is_directory()) {
            n += 1;
        }
    }

    return n;
}

static std::string citizen_name_ref(const char* subdir, size_t idx) {
    std::vector<std::string> names = list_citizen_names(subdir);
    
    if (names.empty()) {
        return "";
    } else {
        return names[idx % names.size()];
    }
}

static inline void citizen_default_virtual_canvas(Citizen* self) {
    self->set_virtual_canvas(36.0F, 72.0F);
}

static inline void kid_set_virtual_canvas(Citizen* self) {
    self->set_virtual_canvas(32.0F, 56.0F);
}

static inline void student_set_virtual_canvas(Citizen* self) {
    self->set_virtual_canvas(32.0F, 68.0F);
}

/*************************************************************************************************/
size_t Plteen::Citizen::special_name_count() {
    return citizen_name_count(TRAIL_SPECIALS_PATH);
}

std::vector<std::string> Plteen::Citizen::list_special_names() {
    return list_citizen_names(TRAIL_SPECIALS_PATH);
}

size_t Plteen::Citizen::bracer_name_count() {
    return citizen_name_count(TRAIL_BRACERS_PATH);
}

std::vector<std::string> Plteen::Citizen::list_bracer_names() {
    return list_citizen_names(TRAIL_BRACERS_PATH);
}

size_t Plteen::TrailKid::name_count() {
    return citizen_name_count(TRAIL_KIDS_PATH);
}

std::vector<std::string> Plteen::TrailKid::list_names() {
    return list_citizen_names(TRAIL_KIDS_PATH);
}

size_t Plteen::TrailStudent::name_count() {
    return citizen_name_count(TRAIL_STUDENTS_PATH);
}

std::vector<std::string> Plteen::TrailStudent::list_names() {
    return list_citizen_names(TRAIL_STUDENTS_PATH);
}

TrailKid* Plteen::TrailKid::randomly_create() {
    std::vector<std::string> names = TrailKid::list_names();
    TrailKid* kid = nullptr;

    if (!names.empty()) {
        kid = new TrailKid(names[random_uniform(0, int(names.size()) - 1)]);
    }

    return kid;
}

TrailStudent* Plteen::TrailStudent::randomly_create() {
    std::vector<std::string> names = TrailStudent::list_names();
    TrailStudent* student = nullptr;

    if (!names.empty()) {
        student = new TrailStudent(names[random_uniform(0, int(names.size()) - 1)]);
    }

    return student;
}

/*************************************************************************************************/
Plteen::Citizen::Citizen(const std::string& fullpath, const char* nickname) : Sprite(fullpath) {
    citizen_default_virtual_canvas(this);
    this->give_nickname(nickname);
}

Plteen::Citizen::Citizen(size_t seq, const char* rootdir, const char* nickname)
    : Citizen(citizen_name_ref(rootdir, seq).c_str(), rootdir, nickname) {}

Plteen::Citizen::Citizen(const char* name, const char* rootdir, const char* nickname) : Sprite(digimon_mascot_path(name, "", rootdir)) {
    if (string_equal(rootdir, TRAIL_KIDS_PATH) == 0) {
        kid_set_virtual_canvas(this);
    } else if (string_equal(rootdir, TRAIL_STUDENTS_PATH) == 0) {
        student_set_virtual_canvas(this);
    } else {
        citizen_default_virtual_canvas(this);
    }

    this->give_nickname(nickname);
}

Plteen::TrailKid::TrailKid(const char* name, const char* nickname)
    : Citizen(digimon_mascot_path(name, "", TRAIL_KIDS_PATH), nickname) {
    kid_set_virtual_canvas(this);
}

Plteen::TrailKid::TrailKid(size_t idx, const char* nickname)
    : TrailKid(citizen_name_ref(TRAIL_KIDS_PATH, idx).c_str(), nickname) {}

Plteen::TrailStudent::TrailStudent(const char* name, const char* nickname)
    : Citizen(digimon_mascot_path(name, "", TRAIL_STUDENTS_PATH), nickname) {
    student_set_virtual_canvas(this);
}

Plteen::TrailStudent::TrailStudent(size_t idx, const char* nickname)
    : TrailStudent(citizen_name_ref(TRAIL_STUDENTS_PATH, idx).c_str(), nickname) {}

/*************************************************************************************************/
void Plteen::Citizen::on_costumes_load() {
    this->play("walk_s");
}

void Plteen::Citizen::on_heading_changed(double theta_rad, double vx, double vy, double prev_vr) {
    I8WayMotion::dispatch_heading_event(theta_rad, vx, vy, prev_vr);
}

void Plteen::Citizen::on_eward(double theta_rad, double vx, double vy) {
    this->play("walk_e_");
}

void Plteen::Citizen::on_wward(double theta_rad, double vx, double vy) {
    this->play("walk_w_");
}

void Plteen::Citizen::on_sward(double theta_rad, double vx, double vy) {
    this->play("walk_s_");
}

void Plteen::Citizen::on_nward(double theta_rad, double vx, double vy) {
    this->play("walk_n_");
}

void Plteen::Citizen::on_esward(double theta_rad, double vx, double vy) {
    this->play("walk_es_");
}

void Plteen::Citizen::on_enward(double theta_rad, double vx, double vy) {
    this->play("walk_en_");
}

void Plteen::Citizen::on_wsward(double theta_rad, double vx, double vy) {
    this->play("walk_ws_");
}

void Plteen::Citizen::on_wnward(double theta_rad, double vx, double vy) {
    this->play("walk_wn_");
}
