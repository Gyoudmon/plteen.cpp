#include "textlet.hpp"

#include "../../graphics/font.hpp"

#include "../../datum/string.hpp"
#include "../../datum/box.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

using namespace Plteen;

/*************************************************************************************************/
static Labellet* make_styled_label(shared_font_t font, const RGBA& bg_color, const RGBA& bd_color, const RGBA& fg_color, float cr) {
    Labellet* tooltip = new Labellet(font, fg_color, "");

    tooltip->set_background_color(bg_color);
    tooltip->set_border_color(bd_color);
    
    if (cr != 0.0F) {
        tooltip->set_corner_radius(cr);
    }

    return tooltip;
}

/*************************************************************************************************/
Labellet* Plteen::make_label_for_tooltip(shared_font_t font, const RGBA& bg_color, const RGBA& bd_color, const RGBA& fg_color) {
    return make_styled_label(font, bg_color, bd_color, fg_color, 0.0F);
}

/*************************************************************************************************/
Plteen::ITextlet::ITextlet() {
    this->set_text_color();
}

void Plteen::ITextlet::construct(Plteen::dc_t* dcer) {
    this->update_texture();
}

void Plteen::ITextlet::set_text_color(const RGBA& fgc) {
    if (this->foreground_color != fgc) {
        this->foreground_color = fgc;
        this->update_texture();
        this->notify_updated();
    }
}

void Plteen::ITextlet::set_text_alpha(double alpha) {
    if (this->foreground_color.alpha() != alpha) {
        this->foreground_color = RGBA(this->foreground_color, alpha);
        this->update_texture();
        this->notify_updated();
    }
}

void Plteen::ITextlet::set_background_color(const RGBA& bgc) {
    if (this->background_color != bgc) {
        this->background_color = bgc;
        this->notify_updated();
    }
}

void Plteen::ITextlet::set_border_color(const RGBA& bdc) {
    if (this->border_color != bdc) {
        this->border_color = bdc;
        this->notify_updated();
    }
}

void Plteen::ITextlet::set_corner_radius(float radius) {
    if (this->corner_radius != radius) {
        this->corner_radius = radius;
        this->notify_updated();
    }
}

void Plteen::ITextlet::set_font(shared_font_t font, MatterPort port) {
    this->moor(port);

    this->text_font = (((font.use_count() > 0) && (font->okay())) ? font : GameFont::Default());
    this->set_text(this->raw, port);
    this->on_font_changed();

    this->notify_updated();
}

void Plteen::ITextlet::set_text(const std::string& content, MatterPort port) {
    this->raw = content;

    this->moor(port);

    if ((this->text_font.use_count() == 0) || (!this->text_font->okay())) {
        this->set_font(nullptr, port);
    } else {
        this->update_texture();
    }

    this->notify_updated();
}

void Plteen::ITextlet::set_text(const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->set_text(content);
}

void Plteen::ITextlet::set_text(const RGBA& color, const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->set_text(content);
    this->set_text_color(color);
}

void Plteen::ITextlet::set_text(MatterPort port, const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->set_text(content, port);
}

void Plteen::ITextlet::append_text(const std::string& content, MatterPort port) {
    this->set_text(this->raw + content, port);
}

void Plteen::ITextlet::append_text(const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->append_text(content);
}

void Plteen::ITextlet::append_text(const RGBA& color, const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->append_text(content);
    this->set_text_color(color);
}

void Plteen::ITextlet::append_text(MatterPort port, const char* fmt, ...) {
    VSNPRINT(content, fmt);
    this->append_text(content, port);
}

size_t Plteen::ITextlet::content_utf8_size() {
    return string_utf8_length(this->raw);
}

int Plteen::ITextlet::display_width(const char* unicode) {
    if (this->text_font != nullptr) {
        return this->text_font->width(unicode);
    } else {
        return GameFont::Default()->width(unicode);
    }
}

int Plteen::ITextlet::display_height(const char* unicode) {
    if (this->text_font == nullptr) {
        return GameFont::Default()->height(unicode);
    } else {
        return this->text_font->height(unicode);
    }
}

Box Plteen::ITextlet::get_bounding_box() {
    Box box(0.0F, 0.0F);

    if ((this->texture.use_count() > 0) && (this->texture->okay())) {
        float w, h;

        this->texture->feed_extent(&w, &h);
        box = { w, h };
    }

    return box;
}

void Plteen::ITextlet::draw(Plteen::dc_t* dc, float x, float y, float Width, float Height) {
    if ((this->texture.use_count() > 0) && this->texture->okay()) {
        if (this->corner_radius == 0.0F) {
            float pos_off = 0.0F;
            float sizeoff = 0.5F;

            if (this->background_color.is_opacity()) {
                dc->fill_rect(x + pos_off, y + pos_off,
                                Width - sizeoff, Height - sizeoff,
                                this->background_color);
            }

            if (this->border_color.is_opacity()) {
                dc->draw_rect(x + pos_off, y + pos_off,
                                Width - sizeoff, Height - sizeoff,
                                this->border_color);
            }
        } else {
            float pos_off = 0.5F;
            float sizeoff = 2.0F;

            if (this->background_color.is_opacity()) {
                dc->fill_rounded_rect(x + pos_off, y + pos_off,
                                        Width - sizeoff, Height - sizeoff,
                                        this->corner_radius, this->background_color);
            }

            if (this->border_color.is_opacity()) {
                dc->draw_rounded_rect(x + pos_off, y + pos_off,
                                        Width - sizeoff, Height - sizeoff,
                                        this->corner_radius, this->border_color);
            }
        }

        if (this->foreground_color.is_opacity()) {
            dc->stamp(this->texture->self(), x, y);
        }
    }
}

void Plteen::ITextlet::update_texture() {
    Plteen::dc_t* dc = this->drawing_context();

    if ((this->raw.empty()) || (dc == nullptr)) {
        this->texture.reset();
    } else {
        this->texture.reset(new Texture(dc->create_blended_text(this->raw, this->text_font, this->foreground_color, 0)));
    }
}

/*************************************************************************************************/
Plteen::Labellet::Labellet(const char *fmt, ...) {
    VSNPRINT(caption, fmt);
    this->set_text(caption);
}

Plteen::Labellet::Labellet(shared_font_t font, const char* fmt, ...) {
    VSNPRINT(caption, fmt);
    this->set_font(font);
    this->set_text(caption);
}

Plteen::Labellet::Labellet(shared_font_t font, uint32_t hex, const char* fmt, ...) {
    VSNPRINT(caption, fmt);

    this->set_font(font);
    this->set_text_color(hex);
    this->set_text(caption);
}

Plteen::Labellet::Labellet(shared_font_t font, const RGBA& rgb, const char* fmt, ...) {
    VSNPRINT(caption, fmt);

    this->set_font(font);
    this->set_text_color(rgb);
    this->set_text(caption);
}

Plteen::Labellet::Labellet(uint32_t hex, const char* fmt, ...) {
    VSNPRINT(caption, fmt);

    this->set_text_color(hex);
    this->set_text(caption);
}

Plteen::Labellet::Labellet(const RGBA& rgb, const char* fmt, ...) {
    VSNPRINT(caption, fmt);

    this->set_text_color(rgb);
    this->set_text(caption);
}
