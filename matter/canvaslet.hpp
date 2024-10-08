#pragma once

#include "graphlet.hpp"

#include "../graphics/texture.hpp"
#include "../graphics/misc.hpp"
#include "../physics/color/rgba.hpp"

namespace Plteen {
    class __lambda__ ICanvaslet : public Plteen::IGraphlet {
	public:
		ICanvaslet() { this->enable_resize(true); }
		virtual ~ICanvaslet() { this->invalidate_canvas(); }

	public:
		void draw(Plteen::dc_t* dc, float x, float y, float Width, float Height) override;

	public:
		void dirty_canvas(const RGBA& c);
		void dirty_canvas() { this->dirty_canvas(this->canvas_background_color); }
		void dirty_canvas(uint32_t bg_color, double bg_alpha) { this->dirty_canvas(RGBA(bg_color, bg_alpha)); }
    
	    void set_color_mixture(Plteen::ColorMixture mixture);
        Plteen::RGBA get_canvas_color() { return this->canvas_background_color; }
		void set_canvas_alpha(uint8_t alpha) { this->set_canvas_alpha(double(alpha) / 255.0); }
        void set_canvas_alpha(double alpha);
		
		bool brush_okay(uint8_t* r = nullptr, uint8_t* g = nullptr, uint8_t* b = nullptr, uint8_t* a = nullptr);
		Plteen::RGBA get_brush_color() { return this->brush_color; }
		void set_brush_color(const RGBA& c);
        void set_brush_alpha(uint8_t alpha) { this->set_brush_alpha(double(alpha) / 255.0); }
		void set_brush_alpha(double alpha);
        
		bool pen_okay(uint8_t* r = nullptr, uint8_t* g = nullptr, uint8_t* b = nullptr, uint8_t* a = nullptr);
        Plteen::RGBA get_pen_color() { return this->pen_color; }
		void set_pen_color(const RGBA& c);
        void set_pen_alpha(uint8_t alpha) { this->set_pen_alpha(double(alpha) / 255.0); }
		void set_pen_alpha(double alpha);
        
	protected:
		void on_resize(float w, float h, float width, float height) override;

	protected:
		virtual void draw_on_canvas(Plteen::dc_t* dc, float Width, float Height) {}
		virtual void draw_before_canvas(Plteen::dc_t* dc, float x, float y, float Width, float Height) {}
		virtual void draw_after_canvas(Plteen::dc_t* dc, float x, float y, float Width, float Height) {}
		virtual void invalidate_canvas();
		virtual void on_canvas_invalidated() {}

    protected:
		shared_texture_t canvas = nullptr;

	private:
        bool needs_refresh_canvas = true;
		Plteen::RGBA canvas_background_color;

	private:
		Plteen::ColorMixture mixture = ColorMixture::Alpha;
		Plteen::RGBA brush_color;
		Plteen::RGBA pen_color;
	};
}
