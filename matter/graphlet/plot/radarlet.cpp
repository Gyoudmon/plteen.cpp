#include "radarlet.hpp"

#include "../../../datum/box.hpp"
#include "../../../datum/flonum.hpp"
#include "../../../datum/fixnum.hpp"

#include "../../../plane.hpp"
#include "../../../graphics/image.hpp"
#include "../../../physics/mathematics.hpp"

using namespace Plteen;

/*************************************************************************************************/
static const uint32_t font_hex = DIMGRAY;

#define HEAD_FONT GameFont::serif(FontSize::medium)
#define DATA_FONT GameFont::monospace(FontSize::xx_small)
#define MVAR_FONT GameFont::fangsong(FontSize::x_small)

/*************************************************************************************************/
Plteen::Radarlet::Radarlet(size_t n, float radius, const RGBA& color, double fill_alpha)
        : ICanvaslet(), n(fxmax(n, size_t(3))), radius(radius), variable_range(0.0) {
    this->mspoke = this->n - 1;
    this->spoke_xs = std::vector<float>(this->n);
    this->spoke_ys = std::vector<float>(this->n);

    this->title = std::make_shared<Texture>(nullptr);
    this->variables = std::vector<shared_texture_t>(this->n, std::make_shared<Texture>(nullptr));
    this->set_pen_color(color);
    this->set_brush_color(RGBA(color, fill_alpha));
    this->enable_resize(false);

    this->set_start_angle(-pi_f * 0.5F, true);
    this->clear_observations();
}

Box Plteen::Radarlet::get_bounding_box() {
    float radar_size = this->radius * 2.0F;
    float level_height = float(DATA_FONT->height());
    float title_width, title_height;
    
    this->title->feed_extent(&title_width, &title_height);

    return { flmax(radar_size + this->padding_width, title_width),
             radar_size + this->padding_height + title_height + level_height * 1.618F };
}

/*************************************************************************************************/
void Plteen::Radarlet::set_title(const std::string& title, Plteen::MatterPort anchor) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        this->moor(anchor);
        this->title.reset(new Texture(dc->create_blended_text(title, HEAD_FONT, font_hex)));
        this->dirty_canvas();
        this->clear_moor();
    }
}

void Plteen::Radarlet::set_title(const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t);
}

void Plteen::Radarlet::set_title(Plteen::MatterPort anchor, const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t, anchor);
}

void Plteen::Radarlet::set_main_spoke(size_t idx) {
    idx %= this->n;

    if (idx != this->mspoke) {
        this->mspoke = idx;
        this->dirty_canvas();
    }
}

void Plteen::Radarlet::set_start_angle(double angle, bool is_radian) {
    float delta = pi_f * 2.0F / float(this->n);
    float radian = (is_radian) ? angle : degrees_to_radians(angle);
    
    this->moor(MatterPort::CC);

    for (size_t idx = 0; idx < this->n; idx ++) {
        circle_point(this->radius, radian + delta * float(idx), &this->spoke_xs[idx], &this->spoke_ys[idx], true);
    }

    this->dirty_canvas();
    this->clear_moor();
}

void Plteen::Radarlet::set_variable_names(const char* variables[], Plteen::MatterPort anchor) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        float vwidth, vheight, vwoff, vhoff; 
        float lx = -this->radius;
        float rx = +this->radius;
        float ty = -this->radius;
        float by = +this->radius;

        this->moor(anchor);

        for (size_t idx = 0; idx < this->n; idx ++) {
            float theta = flatan(this->spoke_ys[idx], this->spoke_xs[idx]);
            
            this->variables[idx].reset(new Texture(dc->create_blended_text(variables[idx], MVAR_FONT, font_hex)));
            this->variables[idx]->feed_extent(&vwidth, &vheight);

            vwoff = vheight * flcos(theta) + vwidth * 0.618F;
            vhoff = vheight * flsin(theta);

            lx = flmin(lx, this->spoke_xs[idx] - vwoff);
            rx = flmax(rx, this->spoke_xs[idx] + vwoff);
            ty = flmin(ty, this->spoke_ys[idx] - vhoff);
            by = flmin(by, this->spoke_ys[idx] + vhoff);
        }

        this->padding_width  = flmax(-lx - this->radius, +rx - this->radius) * 2.0F;
        this->padding_height = flmax(- ty - this->radius, by - this->radius) * 2.0F;

        this->dirty_canvas();
        this->clear_moor();
    }
}

void Plteen::Radarlet::set_levels(const double* levels, size_t count) {
    this->levels.resize(count);
    
    for (size_t idx = 0; idx < count; idx ++) {
        float t = clamp(levels[idx], 0.0, 1.0);

        if ((levels[idx] > 0.0) && (levels[idx] < 1.0)) {
            this->levels[idx] = t;
        } else {
            this->levels[idx] = flnan_f;
        }
    }
}

void Plteen::Radarlet::set_range(double range) {
    if (this->variable_range != range) {
        this->variable_range = range;
        this->dirty_canvas();
    }
}

void Plteen::Radarlet::push_observation(const double* variables, const Plteen::RGBA& pen_color, double fill_alpha) {
    std::vector<double> observation = std::vector<double>(this->n, 0.0);

    for (size_t v = 0; v < this->n; v++) {
        observation[v] = clamp(variables[v], 0.0, 1.0);
    }

    this->observations.push_back(observation);
    this->colors.push_back(pen_color);
    this->fill_colors.push_back(RGBA(pen_color, fill_alpha));

    this->dirty_canvas();
}

void Plteen::Radarlet::set_observation(size_t idx, const double* variables) {
    if (idx < this->observations.size()) {
        for (size_t v = 0; v < this->n; v++) {
            this->observations[idx][v] = clamp(variables[v], 0.0, 1.0);
        }

        this->dirty_canvas();
    }
}

void Plteen::Radarlet::set_observation_colors(size_t idx, const Plteen::RGBA& pen_color, double fill_alpha) {
    if (idx < this->colors.size()) {
        this->colors[idx] = pen_color;
        this->fill_colors[idx] = RGBA(pen_color, fill_alpha);
        this->dirty_canvas();
    }
}

/*************************************************************************************************/
void Plteen::Radarlet::draw_on_canvas(Plteen::dc_t* dc, float flwidth, float flheight) {
    std::vector<short> vxs(this->n);
    std::vector<short> vys(this->n);
    std::vector<float> txs(this->n);
    std::vector<float> tys(this->n);
    RGBA font_color = RGBA(font_hex);
    float ox = flwidth  * 0.5F;
    float oy = flheight * 0.5F;
    uint8_t r, g, b, a, fr, fg, fb, fa;

    font_color.unbox(&fr, &fg, &fb, &fa);

    /* draw title */ {
        float title_width, title_height;
    
        this->title->feed_extent(&title_width, &title_height);
        dc->stamp(this->title->self(), (flwidth - title_width) * 0.5F, 0.0);
        
        if (title_height > 0.0F) {
            dc->draw_line(0.0F, title_height, flwidth, title_height, fr, fg, fb, fa);
        }

        oy += title_height * 0.5F;
    }

    /* draw spokes */ {
        if (this->brush_okay(&r, &g, &b, &a)) {
            dc->fill_polygon(vxs.data(), vys.data(), this->spoke_xs.data(), this->spoke_ys.data(), this->n, r, g, b, a, ox, oy);
        }

        if (this->pen_okay(&r, &g, &b, &a)) {
            RGBA pen = this->get_pen_color();
            float ax, ay;
    
            dc->draw_polygon(vxs.data(), vys.data(), this->spoke_xs.data(), this->spoke_ys.data(), this->n, r, g, b, a, ox, oy);
            this->draw_variable(dc, 0, ox, oy, DATA_FONT, font_color);

            for (size_t idx = 0; idx < this->levels.size(); idx ++) {
                if (!flisnan(this->levels[idx])) {
                    for (size_t v = 0; v < this->n; v++) {
                        linear_lerp(this->spoke_xs[v], this->spoke_ys[v], this->levels[idx], &txs.data()[v], &tys.data()[v]);
                    }

                    dc->draw_polygon(vxs.data(), vys.data(), txs.data(), tys.data(), this->n, r, g, b, a, ox, oy);
                    this->draw_variable(dc, this->levels[idx], ox + txs[this->mspoke], oy + tys[this->mspoke], DATA_FONT, font_color);
                }
            }

            this->draw_variable(dc, 1.0, ox + this->spoke_xs[this->mspoke], oy + this->spoke_ys[this->mspoke], DATA_FONT, font_color);

            for (size_t idx = 0; idx < this->spoke_xs.size(); idx ++) {
                ax = ox + this->spoke_xs[idx];
                ay = oy + this->spoke_ys[idx];

                dc->draw_line(ox, oy, ax, ay, r, g, b, a);
                this->draw_variable_name(dc, idx, this->spoke_xs[idx], this->spoke_ys[idx], ox, oy);
            }
        }
    }

    /* draw observations */ {
        for (size_t idx = 0; idx < this->observations.size(); idx ++) {
            for (size_t v = 0; v < this->n; v++) {
                linear_lerp(this->spoke_xs[v], this->spoke_ys[v], this->observations[idx][v], &txs.data()[v], &tys.data()[v]);
            }

            if (!this->fill_colors[idx].is_transparent()) {
                dc->fill_polygon(vxs.data(), vys.data(), txs.data(), tys.data(), this->n, this->fill_colors[idx], ox, oy);
            }

            if (!this->colors[idx].is_transparent()) {
                this->colors[idx].unbox(&r, &g, &b, &a);

                dc->draw_polygon(vxs.data(), vys.data(), txs.data(), tys.data(), this->n, r, g, b, a, ox, oy);

                for (size_t v = 0; v < this->n; v ++) {
                    float vx = ox + txs[v];
                    float vy = oy + tys[v];
                
                    dc->fill_circle(vx, vy, 2.0F, r, g, b, a);
                    this->draw_variable(dc, this->observations[idx][v], vx, vy, DATA_FONT, this->colors[idx]);
                }
            }
        }
    }
}

/*************************************************************************************************/
void Plteen::Radarlet::draw_variable(dc_t* dc, double percentage, float x, float y, const shared_font_t& font, const RGBA& color) {
    std::string value = (percentage == 0.0)
                            ? "0" : ((this->variable_range == 0.0)
                                        ? make_nstring("%.0lf%%", flround(100.0 * percentage))
                                        : make_nstring("%.1lf", this->variable_range * percentage));
    shared_texture_t label = std::make_shared<Texture>(dc->create_blended_text(value, font, color));
    float width, height;

    label->feed_extent(&width, &height);
    dc->stamp(label->self(), x - width, y - height);
}

void Plteen::Radarlet::draw_variable_name(dc_t* dc, size_t idx, float sx, float sy, float ox, float oy) {
    shared_texture_t name = this->variables[idx];
    float theta = flatan(sy, sx);
    float wratio = flcos(theta);
    float hratio = flsin(theta); 
    float vx = sx + ox;
    float vy = sy + oy;
    float width, height;

    name->feed_extent(&width, &height);
    dc->stamp(name->self(), vx + height * wratio - width * 0.5F, vy + height * hratio - height * 0.5F);
}

void Plteen::Radarlet::clear_observations() {
    if (!this->observations.empty()) {
        this->observations.clear();
        this->colors.clear();
        this->fill_colors.clear();
        this->dirty_canvas();
    }
}
