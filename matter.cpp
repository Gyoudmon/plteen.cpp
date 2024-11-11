#include "matter.hpp"
#include "plane.hpp"

#include "datum/box.hpp"
#include "graphics/image.hpp"
#include "physics/algebra/vector.hpp"

#include <typeinfo>

using namespace Plteen;

/**************************************************************************************************/
Plteen::IMatter::~IMatter() {
    if (this->info != nullptr) {
        delete this->info;
        this->info = nullptr;
    }

    if (this->_metatdata != nullptr) {
        delete this->_metatdata;
        this->_metatdata = nullptr;
    }
}

IPlane* Plteen::IMatter::master() const {
    IPlane* plane = nullptr;

    if (this->info != nullptr) {
        plane = this->info->master;
    }

    return plane;
}

dc_t* Plteen::IMatter::drawing_context() const {
    IPlane* master = this->master();
    dc_t* device = nullptr;

    if (master != nullptr) {
        device = master->drawing_context();
    }

    return device;
}

void Plteen::IMatter::attach_metadata(IMatterMetadata* metadata) {
    if (this->_metatdata != nullptr) {
        delete this->_metatdata;
    }

    this->_metatdata = metadata;
}

bool Plteen::IMatter::is_colliding(const cPoint& local_pt) {
    Box box = this->get_bounding_box();
    Margin margin = this->get_margin();

    return margin.is_point_inside(_X(local_pt), _Y(local_pt), box.width(), box.height());
}

void Plteen::IMatter::scale(float x_ratio, float y_ratio, const Port& port) {
    if (this->can_resize) {
        if ((x_ratio != 1.0F) || (y_ratio != 1.0F)) {
            Box box = this->get_bounding_box();

	        this->moor(port);
            this->on_resize(box.width() * x_ratio, box.height() * y_ratio, box.width(), box.height());
	        this->notify_updated();
        }
    }
}

void Plteen::IMatter::scale_to(float x_ratio, float y_ratio, const Port& port) {
    if (this->can_resize) {
        float nwidth, nheight;
        Box cbox = this->get_bounding_box();
        Box obox = this->get_original_bounding_box();

        nwidth = obox.width() * x_ratio;
        nheight = obox.height() * y_ratio;

        if ((nwidth != cbox.width()) || (nheight != cbox.height())) {
	        this->moor(port);
            this->on_resize(nwidth, nheight, cbox.width(), cbox.height());
	        this->notify_updated();
        }
    }
}

void Plteen::IMatter::scale_by_size(float size, bool given_width, const Port& port) {
    if (this->can_resize) {
        float nwidth, nheight;
        Box box = this->get_bounding_box();
        
        if (given_width) {
            nwidth = size;
            nheight = box.height() * (size / box.width());
        } else {
            nheight = size;
            nwidth = box.width() * (size / box.height());
        }
        
        if ((nwidth != box.width()) || (nheight != box.height())) {
            this->moor(port);
            this->on_resize(nwidth, nheight, box.width(), box.height());
	        this->notify_updated();
        }
    }
}

void Plteen::IMatter::resize(float nwidth, float nheight, const Port& port) {
    if (this->can_resize) {
        Box box = this->get_bounding_box();

        if (nwidth == 0.0F) {
            nwidth = box.width();
        }

        if (nheight == 0.0F) {
            nheight = box.height();
        }

	    if ((box.width() != nwidth) || (box.height() != nheight)) {
            this->moor(port);
            this->on_resize(nwidth, nheight, box.width(), box.height());
	        this->notify_updated();
	    }
    }
}

void Plteen::IMatter::notify_updated() {
    if (this->info != nullptr) {
        if (!this->port.is_zero()) {
            cPoint dot = this->info->master->get_matter_location(this, this->port);

            /** NOTE
             * Gliding dramatically increasing the complexity of moving as glidings might be queued,
             *   the anchored moving here therefore uses relative target position,
             *   and do the moving immediately.
             **/

            if (dot != this->port_dot) {
                this->info->master->move(this, this->port_dot - dot, true);
            }

            this->clear_moor();
        }

        this->info->master->notify_updated(this);
    }
}

void Plteen::IMatter::notify_timeline_restart(uint32_t count0, int duration) {
    if (this->info != nullptr) {
        this->info->master->notify_matter_timeline_restart(this, count0, duration);
    }
}

void Plteen::IMatter::moor(const Port& port) {
    if (this->port != port) {
        if (this->info != nullptr) {
            this->port = port;
            this->port_dot = this->info->master->get_matter_location(this, port);
        }
    }
}

void Plteen::IMatter::clear_moor() {
    this->port.reset();
}

bool Plteen::IMatter::has_caret() {
    bool careted = false;

    if (this->info != nullptr) {
        careted = (this->info->master->get_focused_matter() == this);
    }

    return careted;
}

void Plteen::IMatter::show(bool yes_no) {
    if (this->invisible == yes_no) {
        this->invisible = !yes_no;
        this->notify_updated();
    }
}

cPoint Plteen::IMatter::get_location(const Port& p) {
    cPoint dot(flnan_f, flnan_f);

    if (this->info != nullptr) {
        dot = this->info->master->get_matter_location(this, p);
    }

    return dot;
}

void Plteen::IMatter::log_message(Log level, const std::string& msg) {
    if (this->info != nullptr) {
        this->info->master->log_message(level, msg);
    }
}

const char* Plteen::IMatter::name() {
    return typeid(this).name();
}
