#include "text_facility.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const char keys[], const char* descs[], size_t n, bool default_state
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t idx = 0; idx < n; idx ++) {
            this->_fkeys[idx]  = keys[idx];
            this->_fdescs[idx] = descs[idx];
            this->_fokay[idx] = default_state;
        }
    }
}

Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const char keys[], const char* descs[], size_t n, bool default_states[]
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t idx = 0; idx < n; idx ++) {
            this->_fkeys[idx]  = keys[idx];
            this->_fdescs[idx] = descs[idx];
            this->_fokay[idx] = default_states[idx];
        }
    }
}

Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const facility_item_t keys[], size_t n, bool default_state
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t idx = 0; idx < n; idx ++) {
            this->_fkeys[idx]  = keys[idx].first;
            this->_fdescs[idx] = keys[idx].second;
            this->_fokay[idx] = default_state;
        }
    }
}

Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const facility_item_t keys[], size_t n, bool default_states[]
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t idx = 0; idx < n; idx ++) {
            this->_fkeys[idx]  = keys[idx].first;
            this->_fdescs[idx] = keys[idx].second;
            this->_fokay[idx] = default_states[idx];
        }
    }
}

void Plteen::TextFacilityPlane::initialize_facilities(size_t n) {
    this->n = n;
    this->working_idx = n;

    this->facilities = new Labellet*[n];
    this->_fkeys = new char[n];
    this->_fdescs = new std::string[n];
    this->_fokay = new bool[n];
}

Plteen::TextFacilityPlane::~TextFacilityPlane() noexcept {
    if (this->n > 0) {
        delete [] this->_fkeys;
        delete [] this->_fdescs;
        delete [] this->_fokay;
        delete [] this->facilities;
    }
}

/*************************************************************************************************/
void Plteen::TextFacilityPlane::load(float width, float height) {
    TheBigBang::load(width, height);

    if (n > 0) {
        for (size_t idx = 0; idx < n; idx ++) {
            this->facilities[idx] = this->insert(new Labellet(this->facility_font,
                (this->_fokay[idx]) ? this->enabled_facility_color : this->disabled_facility_color,
                "%c. %s", this->_fkeys[idx], this->_fdescs[idx].c_str()));
        }
    }
}

void Plteen::TextFacilityPlane::reflow(float width, float height) {
    TheBigBang::reflow(width, height);

    if (this->n > 0) {
        cVector offset = { float(this->facility_font->height()), 0.0F };

        this->reflow_facility(this->facilities[0], nullptr, width, height);
        for (size_t idx = 1; idx < this->n; idx ++) {
            this->reflow_facility(this->facilities[idx], this->facilities[idx - 1], width, height);
        }
    }
}

void Plteen::TextFacilityPlane::reflow_facility(Labellet* facility, Labellet* prev, float width, float height) {
    if (prev == nullptr) {
        this->move_to(facility, { 0.0F, height }, MatterPort::LB);
    } else {
        this->move_to(facility, { prev, MatterPort::RB }, MatterPort::LB, { this->facility_font->line_height(), 0.0F });
    }
}

void Plteen::TextFacilityPlane::update_facilities(bool state) {
    this->begin_update_sequence();
    for (size_t idx = 0; idx < this->n; idx ++) {
        if (this->_fokay[idx] != state) {
            this->_fokay[idx] = state;
            this->notify_updated(this->facilities[idx]);
        }
    }
    this->end_update_sequence();
}

void Plteen::TextFacilityPlane::update_facilities(bool states[]) {
    this->begin_update_sequence();
    for (size_t idx = 0; idx < this->n; idx ++) {
        if (this->_fokay[idx] != states[idx]) {
            this->_fokay[idx] = states[idx];
            this->notify_updated(this->facilities[idx]);
        }
    }
    this->end_update_sequence();
}

/*************************************************************************************************/
void Plteen::TextFacilityPlane::on_tap(Plteen::IMatter* m, float local_x, float local_y) {
    bool handled = false;

    if (this->n > 0) {
        auto f = dynamic_cast<Labellet*>(m);

        if (f != nullptr) {
            this->begin_update_sequence();
            for (size_t idx = 0; idx < this->n; idx ++) {
                if (this->facilities[idx] == f) {
                    handled = true;
                    this->deal_with_facility_at(idx);
                    break;
                }
            }
            this->end_update_sequence();
        }
    }
    
    if (!handled) {
        TheBigBang::on_tap(m, local_x, local_y);
    }
}

void Plteen::TextFacilityPlane::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    bool handled = false;

    if (this->n > 0) {
        this->begin_update_sequence();
        for (size_t idx = 0; idx < this->n; idx ++) {
            if (this->_fkeys[idx] == key) {
                handled = true;
                this->deal_with_facility_at(idx);
                break;
            }
        }
        this->end_update_sequence();
    }

    if (!handled) {
        TheBigBang::on_char(key, modifiers, repeats, pressed);
    }
}

/*************************************************************************************************/
void Plteen::TextFacilityPlane::deal_with_facility_at(size_t idx) {
    Labellet* working_facility = (this->working_idx >= this->n) ? nullptr : this->facilities[this->working_idx];
                
    if (this->_fokay[idx]) {                
        if (working_facility != this->facilities[idx]) {
            if (working_facility != nullptr) {
                if (this->_fokay[this->working_idx]) {
                    working_facility->set_foreground_color(this->enabled_facility_color);
                } else {
                    working_facility->set_foreground_color(this->disabled_facility_color);
                }
            }
            
            this->working_idx = idx;
            this->facilities[idx]->set_foreground_color(this->working_facility_color);
            this->on_facility_command(this->_fkeys[idx]);
        }
    } else {
        this->facilities[idx]->set_foreground_color(this->hi_disabled_facility_color);
    }
}

/*************************************************************************************************/
void Plteen::TextFacilityPlane::set_working_facility_color(const RGBA& color) {
    if (this->working_facility_color != color) {
        this->working_facility_color = color;
        this->notify_updated();
    }
}

void Plteen::TextFacilityPlane::set_enabled_facility_color(const RGBA& color) {
    if (this->enabled_facility_color != color) {
        this->enabled_facility_color = color;
        this->notify_updated();
    }
}

void Plteen::TextFacilityPlane::set_disabled_facility_color(const RGBA& color, const RGBA& hi_color) {
    this->begin_update_sequence();
    
    if (this->disabled_facility_color != color) {
        this->disabled_facility_color = color;
        this->notify_updated();
    }

    if (this->hi_disabled_facility_color != color) {
        this->hi_disabled_facility_color = color;
        this->notify_updated();
    }
    
    this->end_update_sequence();
}
