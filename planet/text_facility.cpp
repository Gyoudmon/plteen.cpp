#include "text_facility.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const char keys[], const char* descs[], size_t n, const bool* default_states
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t row = 0; row < n; row ++) {
            this->_fkeys[row]  = keys[row];
            this->_fdescs[row] = descs[row];

            for (size_t col = 0; col < n; col ++) {
                this->_fokay[row][col] = default_states[row];
            }
        }
    }
}

Plteen::TextFacilityPlane::TextFacilityPlane(const shared_font_t& font, const facility_item_t keys[], size_t n, const bool* default_states
        , const char* name, const RGBA& title_color) : TheBigBang(name, title_color) {
    if (n > 0) {
        this->facility_font = font;
        this->initialize_facilities(n);

        for (size_t row = 0; row < n; row ++) {
            this->_fkeys[row]  = keys[row].first;
            this->_fdescs[row] = keys[row].second;

            for (size_t col = 0; col < n; col ++) {
                this->_fokay[row][col] = *((default_states + row * n) + col);
            }
        }
    }
}

void Plteen::TextFacilityPlane::initialize_facilities(size_t n) {
    this->n = n;
    this->working_idx = n;

    this->facilities = new Labellet*[n];
    this->_fkeys = new char[n];
    this->_fdescs = new std::string[n];
    this->_fokay = new bool*[n];

    for (size_t idx = 0; idx < n; idx ++) {
        this->_fokay[idx] = new bool[n];
    }
}

Plteen::TextFacilityPlane::~TextFacilityPlane() noexcept {
    if (this->n > 0) {
        delete [] this->_fkeys;
        delete [] this->_fdescs;
        delete [] this->facilities;

        for (size_t col = 0; col < this->n; col ++) {
            delete [] this->_fokay[col];
        }

        delete [] this->_fokay;
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

Plteen::Labellet* Plteen::TextFacilityPlane::facility_ref(int idx) {
    return (this->n > 0) ? this->facilities[wrap_index(idx, int(this->n))] : nullptr;
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
    if (!this->trigger_facility(key)) {
        TheBigBang::on_char(key, modifiers, repeats, pressed);
    }
}

bool Plteen::TextFacilityPlane::trigger_facility(char key) {
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

    return handled;
}

/*************************************************************************************************/
void Plteen::TextFacilityPlane::deal_with_facility_at(size_t idx) {
    Labellet* working_facility = (this->working_idx >= this->n) ? nullptr : this->facilities[this->working_idx];
                
    if ((working_facility == nullptr) || (this->_fokay[this->working_idx][idx])) {                
        if (working_facility != this->facilities[idx]) {
            IScreen* master = this->master();

            if (working_facility != nullptr) {
                if (this->_fokay[this->working_idx][idx]) {
                    working_facility->set_foreground_color(this->enabled_facility_color);
                } else {
                    working_facility->set_foreground_color(this->disabled_facility_color);
                }
            }
            
            this->working_idx = idx;
            this->facilities[idx]->set_foreground_color(this->working_facility_color);

            if (master != nullptr) {
                float width, height;

                master->feed_client_extent(&width, &height);
                this->on_facility_command(idx, this->_fkeys[idx], width, height);
            } else {
                this->on_facility_command(idx, this->_fkeys[idx], 0.0F, 0.0F);
            }
        }
    } else if (this->working_idx != idx) {
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
