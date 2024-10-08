#include "bracer.hpp"

#include "../../../../datum/box.hpp"
#include "../../../../datum/string.hpp"
#include "../../../../physics/random.hpp"

using namespace Plteen;

/*************************************************************************************************/
Bracer* Plteen::Bracer::randomly_create(const char* nickname) {
    size_t idx = random_uniform(0, int(Bracer::name_count()) - 1);

    return Bracer::create(idx, nickname);
}

Bracer* Plteen::Bracer::create(const char* name, const char* nickname) {
    Bracer* bracer = nullptr;

    if (string_ci_equal(name, "Estelle")) {
        bracer = new Estelle(nickname);
    } else if (string_ci_equal(name, "Joshua")) {
        bracer = new Joshua(nickname);
    } else if (string_ci_equal(name, "Scherazard")) {
        bracer = new Scherazard(nickname);
    } else if (string_ci_equal(name, "Olivier")) {
        bracer = new Olivier(nickname);
    } else if (string_ci_equal(name, "Klose")) {
        bracer = new Klose(nickname);
    } else if (string_ci_equal(name, "Agate")) {
        bracer = new Agate(nickname);
    } else if (string_ci_equal(name, "Tita")) {
        bracer = new Tita(nickname);
    } else if (string_ci_equal(name, "Zin")) {
        bracer = new Zin(nickname);
    } else {
        bracer = new Estelle("[No Such a Bracer]");
    }

    return bracer;
}

Bracer* Plteen::Bracer::create(size_t seq, const char* nickname) {
    Bracer* bracer = nullptr;

    switch (seq % Bracer::name_count()) {
    case 0: bracer = new Estelle(nickname); break;
    case 1: bracer = new Joshua(nickname); break;
    case 2: bracer = new Scherazard(nickname); break;
    case 3: bracer = new Olivier(nickname); break;
    case 4: bracer = new Klose(nickname); break;
    case 5: bracer = new Agate(nickname); break;
    case 6: bracer = new Tita(nickname); break;
    case 7: bracer = new Zin(nickname); break;
    default: bracer = new Estelle("[DEADCODE]"); break;
    }

    return bracer;
}

/*************************************************************************************************/
Plteen::Bracer::Bracer(const char* name, const char* nickname)
    : Citizen(digimon_mascot_path(name, "", TRAIL_BRACERS_PATH), nickname) {}

void Plteen::Bracer::on_costumes_load() {
    Citizen::on_costumes_load();
    this->do_mode_switching(BracerMode::Walk, MatterPort::CC);
}

void Plteen::Bracer::try_switch_mode(BracerMode mode, int repeat, MatterPort anchor) {
    if (this->mode != mode) {
        this->switch_mode(mode, repeat, anchor);
    }
}

void Plteen::Bracer::switch_mode(BracerMode mode, int repeat, MatterPort anchor) {
    if (this->mode != mode) {
        this->do_mode_switching(mode, anchor);
    }
    
    // don't move this into the IF-block above
    switch (this->mode) {
    case BracerMode::Walk: this->on_walk_mode(repeat); break;
    case BracerMode::Run: this->on_run_mode(repeat); break;
    case BracerMode::Win: this->on_win_mode(repeat); break;
    case BracerMode::Lose: this->on_lose_mode(repeat); break;
    }
}

void Plteen::Bracer::retrigger_heading_change_event() {
    double vx, vy, vr;

    this->get_velocity(&vr, &vx, &vy);
    this->dispatch_heading_event(vr, vx, vy, vr);
}

void Plteen::Bracer::do_mode_switching(BracerMode mode, MatterPort anchor) {
    float cwidth, cheight;

    this->mode = mode;

    this->moor(anchor);
    this->feed_canvas_size(mode, &cwidth, &cheight);
    this->set_virtual_canvas(cwidth, cheight);
    this->clear_moor();
}

/*************************************************************************************************/
void Plteen::Bracer::on_walk_mode(int repeat) {
    this->retrigger_heading_change_event();
}

void Plteen::Bracer::on_run_mode(int repeat) {
    this->retrigger_heading_change_event();
}

void Plteen::Bracer::on_win_mode(int repeat) {
    this->play("win", repeat);
}

void Plteen::Bracer::on_lose_mode(int repeat) {
    this->stop();
    this->retrigger_heading_change_event();
}

void Plteen::Bracer::feed_canvas_size(BracerMode mode, float* width, float* height) {
    switch (mode) {
    case BracerMode::Walk: SET_VALUES(width, 36.0F, height, 72.0F); break;
    case BracerMode::Run:  SET_VALUES(width, 48.0F, height, 72.0F); break;
    case BracerMode::Win:  SET_VALUES(width, 90.0F, height, 90.0F); break;
    case BracerMode::Lose: SET_VALUES(width, 90.0F, height, 90.0F); break;
    }
}

/*************************************************************************************************/
void Plteen::Estelle::feed_canvas_size(BracerMode mode, float* width, float* height) {
    if (mode == BracerMode::Win) {
        SET_VALUES(width, 96.0F, height, 96.0F);
    } else {
        Bracer::feed_canvas_size(mode, width, height);
    }
}

void Plteen::Tita::feed_canvas_size(BracerMode mode, float* width, float* height) {
    switch (mode) {
    case BracerMode::Walk: SET_VALUES(width, 48.0F, height, 72.0F); break;
    case BracerMode::Run:  SET_VALUES(width, 50.0F, height, 72.0F); break;
    default: Bracer::feed_canvas_size(mode, width, height);
    }
}

void Plteen::Zin::feed_canvas_size(BracerMode mode, float* width, float* height) {
    switch (mode) {
    case BracerMode::Walk: case BracerMode::Run: SET_VALUES(width, 64.0F, height, 96.0F); break;
    default: Bracer::feed_canvas_size(mode, width, height);
    }
}

/*************************************************************************************************/
void Plteen::Bracer::on_eward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_e_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_e"); break;
    default: this->play("walk_e_"); break;
    }
}

void Plteen::Bracer::on_wward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_w_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_w"); break;
    default: this->play("walk_w_"); break;
    }
}

void Plteen::Bracer::on_sward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_s_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_s"); break;
    default: this->play("walk_s_"); break;
    }
}

void Plteen::Bracer::on_nward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_n_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_n"); break;
    default: this->play("walk_n_"); break;
    }
}

void Plteen::Bracer::on_esward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_es_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_es"); break;
    default: this->play("walk_es_"); break;
    }
}

void Plteen::Bracer::on_enward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_en_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_en"); break;
    default: this->play("walk_en_"); break;
    }
}

void Plteen::Bracer::on_wsward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_ws_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_ws"); break;
    default: this->play("walk_ws_"); break;
    }
}

void Plteen::Bracer::on_wnward(double theta_rad, double vx, double vy) {
    switch (this->mode) {
    case BracerMode::Run: this->play("run_wn_"); break;
    case BracerMode::Lose: this->switch_to_costume("lose_wn"); break;
    default: this->play("walk_wn_"); break;
    }
}
