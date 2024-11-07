#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <string>

#include "font.hpp"

#include "../physics/color/rgba.hpp"

namespace Plteen {
    enum TextRenderMode { Solid, Shaded, Blender, LCD };

    class __lambda__ DrawingContext {
    public:
        DrawingContext(SDL_Renderer* device);
        ~DrawingContext() noexcept;

    public:
        SDL_Renderer* self() const { return this->device; }
        const char* name() const { return this->info.name; }

    public:
        int feed_output_size(int* width, int* height) { return SDL_GetRendererOutputSize(this->device, width, height); }
        SDL_Surface* snapshot(int width, int height);

    public:
        void clear(const Plteen::RGBA& color);
        void reset(const Plteen::RGBA& fgc, const Plteen::RGBA& bgc);
        void reset(SDL_Texture* texture, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc);
        void refresh(SDL_Texture* texture);

    public:
        SDL_Texture* create_blank_image(int width, int height);
        SDL_Texture* create_blank_image(float width, float height);
        SDL_Texture* get_target() { return SDL_GetRenderTarget(this->device); }
        int set_draw_color(const Plteen::RGBA& color);
        int set_target(SDL_Texture* target) { return SDL_SetRenderTarget(this->device, target); }
        bool set_clipping_region(SDL_Rect* rect) { return SDL_RenderSetClipRect(this->device, rect); }
        bool clear_clipping_region() { return this->set_clipping_region(nullptr); }

    public:
        void draw_frame(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_grid(int row, int col, int cell_width, int cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int xoff = 0, int yoff = 0);
        void fill_grid(int* grids[], int row, int col, int cell_width, int cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int xoff = 0, int yoff = 0);

        void draw_frame(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_grid(int row, int col, float cell_width, float cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float xoff = 0.0F, float yoff = 0.0F);
        void fill_grid(int* grids[], int row, int col, float cell_width, float cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float xoff = 0.0F, float yoff = 0.0F);

        void stamp(SDL_Surface* surface, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, int x, int y, int widht, int height, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, SDL_Rect* region, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);

        void stamp(SDL_Surface* surface, float x, float y, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, float x, float y, float width, float height, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, SDL_FRect* region, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        void stamp(SDL_Surface* surface, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);

        int stamp(SDL_Texture* texture, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, SDL_Rect* region, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        
        int stamp(SDL_Texture* texture, float x, float y, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, float x, float y, float width, float height, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, SDL_FRect* region, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);
        int stamp(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0);

    public:
        void draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_line(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_hline(int x, int y, int length, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_vline(int x, int y, int length, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        void draw_points(const SDL_Point* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_lines(const SDL_Point* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rect(SDL_Rect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rect(SDL_Rect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rounded_rect(SDL_Rect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_rect(SDL_Rect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_rounded_rect(int x, int y, int width, int height, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_rect(int x, int y, int width, int height, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_square(int cx, int cy, int apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_square(int cx, int cy, int apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rounded_square(int cx, int cy, int apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_square(int cx, int cy, int apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_circle(int cx, int cy, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_circle(int cx, int cy, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_ellipse(int cx, int cy, int aradius, int bradius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_ellipse(int cx, int cy, int aradius, int bradius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        void draw_polygon(short* xs, short* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_polygon(short* xs, short* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_polygon(int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx = 0, int dy = 0);
        void fill_polygon(int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx = 0, int dy = 0);
        void draw_polygon(short* dxs, short* dys, int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx = 0, int dy = 0);
        void fill_polygon(short* dxs, short* dys, int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx = 0, int dy = 0);
        
        void draw_regular_polygon(size_t n, int cx, int cy, int radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_regular_polygon(size_t n, int cx, int cy, int radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
    public:
        void draw_point(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_line(float x1, float y1, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_hline(float x, float y, float length, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_vline(float x, float y, float length, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_points(const SDL_FPoint* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_lines(const SDL_FPoint* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rect(SDL_FRect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rect(SDL_FRect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rounded_rect(SDL_FRect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_rect(SDL_FRect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void draw_rounded_rect(float x, float y, float width, float height, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_rect(float x, float y, float width, float height, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_square(float cx, float cy, float apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_square(float cx, float cy, float apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_circle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_circle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_rounded_square(float cx, float cy, float apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_rounded_square(float cx, float cy, float apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_ellipse(float cx, float cy, float aradius, float bradius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_ellipse(float cx, float cy, float aradius, float bradius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        
        void draw_polygon(float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx = 0.0F, float dy = 0.0F);
        void fill_polygon(float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx = 0.0F, float dy = 0.0F);
        void draw_polygon(short* dxs, short* dys, float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx = 0.0F, float dy = 0.0F);
        void fill_polygon(short* dxs, short* dys, float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx = 0.0F, float dy = 0.0F);

        void draw_regular_polygon(size_t n, float cx, float cy, float radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void fill_regular_polygon(size_t n, float cx, float cy, float radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    
    public:
        template<typename T>
        void draw_frame(T x, T y, T width, T height, const Plteen::RGBA& color)
        { this->draw_frame(x, y, width, height, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_grid(int row, int col, T cell_width, T cell_height, const Plteen::RGBA& color, T xoff = T(0), T yoff = T(0))
        { this->draw_grid(row, col, cell_width, cell_height, color.R(), color.G(), color.B(), color.A(), xoff, yoff); }

        template<typename T>
        void fill_grid(int* grids[], int row, int col, T cell_width, T cell_height, const Plteen::RGBA& color, T xoff = T(0), T yoff = T(0))
        { this->fill_grid(grids, row, col, cell_width, cell_height, color.R(), color.G(), color.B(), color.A(), xoff, yoff); }

        template<typename T>
        void draw_point(T x, T y, const Plteen::RGBA& color)
        { this->draw_point(x, y, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_line(T x1, T y1, T x2, T y2, const Plteen::RGBA& color)
        { this->draw_line(x1, y1, x2, y2, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_hline(T x, T y, T length, const Plteen::RGBA& color)
        { this->draw_hline(x, y, length, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_vline(T x, T y, T length, const Plteen::RGBA& color)
        { this->draw_vline(x, y, length, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_points(const T* pts, int size, const Plteen::RGBA& color)
        { this->draw_points(pts, size, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_lines(const T* pts, int size, const Plteen::RGBA& color)
        { this->draw_lines(pts, size, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_rect(T* box, const Plteen::RGBA& color)
        { this->draw_rect(box, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_rect(T* box, const Plteen::RGBA& color)
        { this->fill_rect(box, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_rect(T x, T y, T width, T height, const Plteen::RGBA& color)
        { this->draw_rect(x, y, width, height, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_rect(T x, T y, T width, T height, const Plteen::RGBA& color)
        { this->fill_rect(x, y, width, height, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_rounded_rect(T* box, float rad, const Plteen::RGBA& color)
        { this->draw_rounded_rect(box, rad, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_rounded_rect(T* box, float rad, const Plteen::RGBA& color)
        { this->fill_rounded_rect(box, rad, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_rounded_rect(T x, T y, T width, T height, float rad, const Plteen::RGBA& color)
        { this->draw_rounded_rect(x, y, width, height, rad, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_rounded_rect(T x, T y, T width, T height, float rad, const Plteen::RGBA& color)
        { this->fill_rounded_rect(x, y, width, height, rad, color.R(), color.G(), color.B(), color.A()); }
        
        template<typename T>
        void draw_square(T cx, T cy, T apothem, const Plteen::RGBA& color)
        { this->draw_square(cx, cy, apothem, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_square(T cx, T cy, T apothem, const Plteen::RGBA& color)
        { this->fill_square(cx, cy, apothem, color.R(), color.G(), color.B(), color.A()); }
        
        template<typename T>
        void draw_rounded_square(T cx, T cy, T apothem, float rad, const Plteen::RGBA& color)
        { this->draw_rounded_square(cx, cy, apothem, rad, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_rounded_square(T cx, T cy, T apothem, float rad, const Plteen::RGBA& color)
        { this->fill_rounded_square(cx, cy, apothem, rad, color.R(), color.G(), color.B(), color.A()); }
        
        template<typename T>
        void draw_circle(T cx, T cy, T radius, const Plteen::RGBA& color)
        { this->draw_circle(cx, cy, radius, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_circle(T cx, T cy, T radius, const Plteen::RGBA& color)
        { this->fill_circle(cx, cy, radius, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_ellipse(T cx, T cy, T aradius, T bradius, const Plteen::RGBA& color)
        { this->draw_ellipse(cx, cy, aradius, bradius, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_ellipse(T cx, T cy, T aradius, T bradius, const Plteen::RGBA& color)
        { this->fill_ellipse(cx, cy, aradius, bradius, color.R(), color.G(), color.B(), color.A()); }

        void draw_polygon(short* xs, short* ys, size_t n, const Plteen::RGBA& color)
        { this->draw_polygon(xs, ys, n, color.R(), color.G(), color.B(), color.A()); }

        void fill_polygon(short* xs, short* ys, size_t n, const Plteen::RGBA& color)
        { this->fill_polygon(xs, ys, n, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void draw_polygon(T* xs, T* ys, size_t n, const Plteen::RGBA& color, T dx = T(0), T dy = T(0))
        { this->draw_polygon(xs, ys, n, color.R(), color.G(), color.B(), color.A(), dx, dy); }

        template<typename T>
        void fill_polygon(T* xs, T* ys, size_t n, const Plteen::RGBA& color, T dx = T(0), T dy = T(0))
        { this->fill_polygon(xs, ys, n, color.R(), color.G(), color.B(), color.A(), dx, dy); }

        template<typename T>
        void draw_polygon(short* vxs, short* vys, T* xs, T* ys, size_t n, const Plteen::RGBA& color, T dx = T(0), T dy = T(0))
        { this->draw_polygon(vxs, vys, xs, ys, n, color.R(), color.G(), color.B(), color.A(), dx, dy); }

        template<typename T>
        void fill_polygon(short* vxs, short* vys, T* xs, T* ys, size_t n, const Plteen::RGBA& color, T dx = T(0), T dy = T(0))
        { this->fill_polygon(vxs, vys, xs, ys, n, color.R(), color.G(), color.B(), color.A(), dx, dy); }

        template<typename T>
        void draw_regular_polygon(size_t n, T cx, T cy, T radius, float rotation, const Plteen::RGBA& color)
        { this->draw_regular_polygon(n, cx, cy, radius, rotation, color.R(), color.G(), color.B(), color.A()); }

        template<typename T>
        void fill_regular_polygon(size_t n, T cx, T cy, T radius, float rotation, const Plteen::RGBA& color)
        { this->fill_regular_polygon(n, cx, cy, radius, rotation, color.R(), color.G(), color.B(), color.A()); }

    public:
        void disable_font_selection(bool yes) { this->_disable_font_selection = yes; }

        SDL_Texture* create_solid_text(const std::string& text, const shared_font_t& font, const Plteen::RGBA& fgc, int wrap = 0);
        SDL_Texture* create_shaded_text(const std::string& text, const shared_font_t& font, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        SDL_Texture* create_lcd_text(const std::string& text, const shared_font_t& font, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        SDL_Texture* create_blended_text(const std::string& text, const shared_font_t& font, const Plteen::RGBA& fgc, int wrap = 0);

        void draw_solid_text(const std::string& text, const shared_font_t& font, float x, float y, const Plteen::RGBA& rgb, int wrap = 0);
        void draw_shaded_text(const std::string& text, const shared_font_t& font, float x, float y, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        void draw_lcd_text(const std::string& text, const shared_font_t& font, float x, float y, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        void draw_blended_text(const std::string& text, const shared_font_t& font, float x, float y, const Plteen::RGBA& rgb, int wrap = 0);

        void draw_solid_text(const std::string& text, const shared_font_t& font, int x, int y, const Plteen::RGBA& rgb, int wrap = 0);
        void draw_shaded_text(const std::string& text, const shared_font_t& font, int x, int y, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        void draw_lcd_text(const std::string& text, const shared_font_t& font, int x, int y, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);
        void draw_blended_text(const std::string& text, const shared_font_t& font, int x, int y, const Plteen::RGBA& rgb, int wrap = 0);

    private:
        SDL_Texture* create_text_texture(const std::string& text, const shared_font_t& font, Plteen::TextRenderMode mode, const Plteen::RGBA& fgc, const Plteen::RGBA& bgc, int wrap = 0);

    private:
        bool _disable_font_selection = false;
        SDL_RendererInfo info;
        SDL_Renderer* device = nullptr;
    };

    typedef DrawingContext dc_t;
}
