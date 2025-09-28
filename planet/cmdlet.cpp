#include "cmdlet.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::CmdletPlane::CmdletPlane(const shared_font_t& font, const cmdlet_item_t keys[], size_t n, const bool* default_states)
        : TheBigBang("Ignored Arguments because of the Virtual Inheritance", 0) {
    if (n > 0) {
        this->cmdlet_font = font;
        this->initialize_facilities(n);

        for (size_t row = 0; row < n; row ++) {
            this->_fkeys[row]  = keys[row].first;
            this->_fdescs[row] = keys[row].second;

            if (default_states != nullptr) {
                for (size_t col = 0; col < n; col ++) {
                    this->_fokay[row][col] = *((default_states + row * n) + col);
                }
            } else {
                for (size_t col = 0; col < n; col ++) {
                    this->_fokay[row][col] = false;
                }
            }
        }
    }
}

void Plteen::CmdletPlane::initialize_facilities(size_t n) {
    this->N = n;
    this->working_idx = n;

    this->facilities = new Labellet*[n];
    this->_fkeys = new char[n + 1];
    this->_fdescs = new std::string[n + 1];
    this->_fokay = new bool*[n];

    for (size_t idx = 0; idx < n; idx ++) {
        this->_fokay[idx] = new bool[n];
    }

    this->_fkeys[n] = '\0';
    this->_fdescs[n] = "";
    this->cmdlet_job_done();
}

void Plteen::CmdletPlane::cmdlet_job_done() {
    if (this->working_idx < this->N) {
        for (int i = 0; i < this->N; i ++) {
            this->facilities[i]->set_foreground_color(this->enabled_cmdlet_color);
        }
        
        this->working_idx = this->N;
    }
}

bool Plteen::CmdletPlane::is_cmdlet_key(char key) {
    bool handled = false;

    if (this->N > 0) {
        for (size_t idx = 0; idx < this->N; idx ++) {
            if (this->_fkeys[idx] == key) {
                handled = true;
                break;
            }
        }
    }

    return handled;
}

Plteen::CmdletPlane::~CmdletPlane() noexcept {
    if (this->N > 0) {
        delete [] this->_fkeys;
        delete [] this->_fdescs;
        delete [] this->facilities;

        for (size_t col = 0; col < this->N; col ++) {
            delete [] this->_fokay[col];
        }

        delete [] this->_fokay;
    }
}

/*************************************************************************************************/
void Plteen::CmdletPlane::load(float width, float height) {
    TheBigBang::load(width, height);

    if (this->N > 0) {
        for (size_t idx = 0; idx < this->N; idx ++) {
            this->facilities[idx] = this->insert(new Labellet(this->cmdlet_font,
                (this->_fokay[idx]) ? this->enabled_cmdlet_color : this->disabled_cmdlet_color,
                "%c. %s", this->_fkeys[idx], this->_fdescs[idx].c_str()));
        }
    }
}

void Plteen::CmdletPlane::reflow(float width, float height) {
    TheBigBang::reflow(width, height);

    if (this->N > 0) {
        Vector offset = { float(this->cmdlet_font->height()), 0.0F };

        this->reflow_cmdlet(this->facilities[0], nullptr, width, height);
        for (size_t idx = 1; idx < this->N; idx ++) {
            this->reflow_cmdlet(this->facilities[idx], this->facilities[idx - 1], width, height);
        }
    }
}

void Plteen::CmdletPlane::reflow_cmdlet(Labellet* cmdlet, Labellet* prev, float width, float height) {
    if (prev == nullptr) {
        this->move_to(cmdlet, { 0.0F, height }, MatterPort::LB);
    } else {
        this->move_to(cmdlet, { prev, MatterPort::RB }, MatterPort::LB, { this->cmdlet_font->line_height(), 0.0F });
    }
}

Plteen::Labellet* Plteen::CmdletPlane::cmdlet_ref(int idx) {
    return (this->N > 0) ? this->facilities[wrap_index(idx, int(this->N))] : nullptr;
}

/*************************************************************************************************/
void Plteen::CmdletPlane::on_tap(Plteen::IMatter* m, float local_x, float local_y) {
    bool handled = false;

    if (this->N > 0) {
        auto f = dynamic_cast<Labellet*>(m);

        if (f != nullptr) {
            this->begin_update_sequence();
            for (size_t idx = 0; idx < this->N; idx ++) {
                if (this->facilities[idx] == f) {
                    this->deal_with_cmdlet_at(idx);
                    handled = true;
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

void Plteen::CmdletPlane::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    if (!this->trigger_cmdlet(key)) {
        TheBigBang::on_char(key, modifiers, repeats, pressed);
    }
}

bool Plteen::CmdletPlane::trigger_cmdlet(char key) {
    bool handled = false;

    if (this->N > 0) {
        this->begin_update_sequence();
        for (size_t idx = 0; idx < this->N; idx ++) {
            if (this->_fkeys[idx] == key) {
                this->deal_with_cmdlet_at(idx);
                handled = true;
                break;
            }
        }
        this->end_update_sequence();
    }

    return handled;
}

/*************************************************************************************************/
void Plteen::CmdletPlane::deal_with_cmdlet_at(size_t idx) {
    Labellet* working_cmdlet = (this->working_idx >= this->N) ? nullptr : this->facilities[this->working_idx];
                
    if ((working_cmdlet == nullptr) || (this->_fokay[this->working_idx][idx])) {                
        if (working_cmdlet != this->facilities[idx]) {
            IScreen* master = this->master();

            if (working_cmdlet != nullptr) {
                if (this->_fokay[this->working_idx][idx]) {
                    working_cmdlet->set_foreground_color(this->enabled_cmdlet_color);
                } else {
                    working_cmdlet->set_foreground_color(this->disabled_cmdlet_color);
                }
            }
            
            this->working_idx = idx;
            this->facilities[idx]->set_foreground_color(this->working_cmdlet_color);

            
            if (master != nullptr) {
                float width, height;

                master->feed_client_extent(&width, &height);
                this->on_cmdlet(idx, this->_fkeys[idx], this->_fdescs[idx], width, height);
            } else {
                this->on_cmdlet(idx, this->_fkeys[idx], this->_fdescs[idx], 0.0F, 0.0F);
            }
        }
    } else if (this->working_idx != idx) {
        this->facilities[idx]->set_foreground_color(this->hi_disabled_cmdlet_color);
    }
}

/*************************************************************************************************/
void Plteen::CmdletPlane::set_processing_cmdlet_color(const RGBA& color) {
    if (this->working_cmdlet_color != color) {
        this->working_cmdlet_color = color;
        this->notify_updated();
    }
}

void Plteen::CmdletPlane::set_enabled_cmdlet_color(const RGBA& color) {
    if (this->enabled_cmdlet_color != color) {
        this->enabled_cmdlet_color = color;
        this->notify_updated();
    }
}

void Plteen::CmdletPlane::set_disabled_cmdlet_color(const RGBA& color, const RGBA& hi_color) {
    this->begin_update_sequence();
    
    if (this->disabled_cmdlet_color != color) {
        this->disabled_cmdlet_color = color;
        this->notify_updated();
    }

    if (this->hi_disabled_cmdlet_color != color) {
        this->hi_disabled_cmdlet_color = color;
        this->notify_updated();
    }
    
    this->end_update_sequence();
}
