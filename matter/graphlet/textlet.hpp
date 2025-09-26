#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

#include "../graphlet.hpp"

#include "../../graphics/font.hpp"
#include "../../graphics/texture.hpp"
#include "../../physics/color/rgba.hpp"
#include "../../physics/color/names.hpp"
#include "../../physics/geometry/aabox.hpp"

namespace Plteen {
    class __lambda__ ITextlet : public virtual Plteen::IGraphlet {
    public:
        ITextlet();
        virtual ~ITextlet() noexcept {}

        void construct(Plteen::dc_t* dc) override;
        const char* name() override { return this->c_str(); }

    public:
        void set_text(const char* fmt, ...);
        void set_text(const Plteen::RGBA& color, const char* fmt, ...);
        void set_text(Plteen::MatterPort port, const char* fmt, ...);
        void set_text(const std::string& content, Plteen::MatterPort port = MatterPort::LT);
        
    public:
        void set_font(shared_font_t font, Plteen::MatterPort port = MatterPort::LT);
        void set_text_color(const Plteen::RGBA& color = SILVER);
        void set_foreground_color(const Plteen::RGBA& color = SILVER) { this->set_text_color(color); }
        void set_text_alpha(double alpha);
        void set_foreground_alpha(double alpha) { this->set_text_alpha(alpha); }
        Plteen::RGBA get_foreground_color() { return this->foreground_color; }
        void set_background_color(const Plteen::RGBA& color);
        Plteen::RGBA get_background_color() { return this->background_color; }
        void set_border_color(const Plteen::RGBA& color);
        Plteen::RGBA get_border_color() { return this->border_color; }
        void set_corner_radius(float radius);

    public:
        Plteen::Box get_bounding_box() override;
        void draw(Plteen::dc_t* dc, float x, float y, float Width, float Height) override;

    public:
        size_t content_size() { return this->raw.size(); }
        size_t content_utf8_size();
        const char* c_str() const { return this->raw.c_str(); }
        int display_width() { return this->display_width(this->raw.c_str()); }
        int display_width(const std::string& unicode) { return this->display_width(unicode.c_str()); }
        int display_width(const char* unicode);
        int display_height() { return this->display_height(this->raw.c_str()); }
        int display_height(const std::string& unicode) { return this->display_height(unicode.c_str()); }
        int display_height(const char* unicode);

    protected:
        virtual void on_font_changed() {}

    private:
        void update_texture();

    protected:
        shared_font_t text_font = nullptr;
        shared_texture_t texture = nullptr;
        Plteen::RGBA foreground_color;
        Plteen::RGBA background_color;
        Plteen::RGBA border_color;
        float corner_radius = 0.0F;

    private:
        std::string raw;
    };

    class __lambda__ Labellet : public virtual Plteen::ITextlet {
    public:
        Labellet(const char* fmt, ...);
        Labellet(shared_font_t font, const char* fmt, ...);
        Labellet(uint32_t hex, const char* fmt, ...);
        Labellet(const Plteen::RGBA& rgb, const char* fmt, ...);
        Labellet(shared_font_t font, uint32_t hex, const char* fmt, ...);
        Labellet(shared_font_t font, const Plteen::RGBA& rgb, const char* fmt, ...);
    };

    /*********************************************************************************************/
    __lambda__ Labellet* make_label_for_tooltip(shared_font_t font,
        const Plteen::RGBA& background_color = SNOW,
        const Plteen::RGBA& border_color = GOLD,
        const Plteen::RGBA& foreground_color = BLACK);
}
