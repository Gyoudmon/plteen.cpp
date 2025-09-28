#include "dc.hpp"

#include "../physics/mathematics.hpp"

#include "../datum/flonum.hpp"
#include "../datum/fixnum.hpp"
#include "../datum/box.hpp"

#include "image.hpp"

// https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/_s_d_l2__gfx_primitives_8h.html
#include <SDL2/SDL2_gfxPrimitives.h>

using namespace Plteen;

/**************************************************************************************************/
#define FILL_BOX(box, px, py, width, height) { box.x = px; box.y = py; box.w = width; box.h = height; }

static void pen_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    int err = 2 - 2 * radius;
    int x = -radius;
    int y = 0;
    
    do {
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);

        radius = err;
        if (radius <= y) {
            err += ++y * 2 + 1;
        }

        if ((radius > x) || (err > y)) {
            err += ++x * 2 ;
        }
    } while (x < 0);
}

static void pen_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    int err = 2 - 2 * radius;
    int x = -radius;
    int y = 0;
    
    do {
        SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y); // Q III, Q IV
        SDL_RenderDrawLine(renderer, cx + x, cy,     cx + x, cy - y); // Q II
        SDL_RenderDrawLine(renderer, cx - x, cy - y, cx,     cy - y); // Q I

        radius = err;
        if (radius <= y) {
            err += ++y * 2 + 1;
        }

        if ((radius > x) || (err > y)) {
            err += ++x * 2 + 1;
        }
    } while (x < 0);
}

static void pen_draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br) {
    /* II. quadrant from bottom left to top right */
    long x = -ar;
    long y = 0;
    long a2 = ar * ar;
    long b2 = br * br;
    long e2 = br;
    long dx = (1 + 2 * x) * e2 * e2;
    long dy = x * x;
    long err = dx + dy;

    do {
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);

        e2 = 2 * err;
        if (e2 >= dx) { x++; err += dx += 2 * b2; }    /* x step */
        if (e2 <= dy) { y++; err += dy += 2 * a2; }    /* y step */
    } while (x <= 0);

    /* to early stop for flat ellipses with a = 1, finish tip of ellipse */
    while (y++ < br) {
        SDL_RenderDrawPoint(renderer, cx, cy + y);
        SDL_RenderDrawPoint(renderer, cx, cy - y);
    }
}

static void pen_fill_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br) {
    /* Q II. from bottom left to top right */
    long x = -ar;
    long y = 0;
    long a2 = ar * ar;
    long b2 = br * br;
    long e2 = br;
    long dx = (1 + 2 * x) * e2 * e2;
    long dy = x * x;
    long err = dx + dy;

    do {
        SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y); // Q III, Q IV
        SDL_RenderDrawLine(renderer, cx + x, cy,     cx + x, cy - y); // Q II
        SDL_RenderDrawLine(renderer, cx - x, cy - y, cx,     cy - y); // Q I

        e2 = 2 * err;
        if (e2 >= dx) { x++; err += dx += 2 * b2; }     /* x step */
        if (e2 <= dy) { y++; err += dy += 2 * a2; }     /* y step */
    } while (x <= 0);

    /* to early stop for flat ellipses with a = 1, finish tip of ellipse */
    while (y++ < br) {
        SDL_RenderDrawLine(renderer, cx, cy + y, cx, cy - y);
    }
}

static void pen_draw_regular_polygon(SDL_Renderer* renderer, size_t n, float cx, float cy, float r, float rotation) {
    // for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    float start = degrees_to_radians(rotation);
    float delta = 2.0F * pi_f / float(n);
    float x0, y0, px, py;

    x0 = px = r * flcos(start) + cx;
    y0 = py = r * flsin(start) + cy;

    for (size_t idx = 1; idx < n; idx++) {
        float theta = start + delta * float(idx);
        float sx = r * flcos(theta) + cx;
        float sy = r * flsin(theta) + cy;

        SDL_RenderDrawLineF(renderer, px, py, sx, sy);
        px = sx;
        py = sy;
    }

    if (px != x0) {
        SDL_RenderDrawLineF(renderer, px, py, x0, y0);
    } else {
        SDL_RenderDrawPointF(renderer, cx, cy);
    }
}

static void pen_fill_regular_polygon(SDL_Renderer* renderer, size_t n, float cx, float cy, float r, float rotation) {
    // for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    float start = degrees_to_radians(rotation);
    float delta = 2.0F * pi_f / float(n);
    float xmin = cx - r;
    float xmax = cx + r;
    float ymin = +r + cy;
    float ymax = -r + cy;

#ifndef __windows__
    SDL_FPoint pts[n + 1];
#else
    SDL_FPoint* pts = new SDL_FPoint[n + 1];
#endif

    for (size_t idx = 0; idx < n; idx ++) {
        float theta = start + delta * float(idx);
        float sx = r * flcos(theta) + cx;
        float sy = r * flsin(theta) + cy;

        pts[idx].x = sx;
        pts[idx].y = sy;

        if (sy < ymin) ymin = sy;
        if (sy > ymax) ymax = sy;
    }
    
    pts[n] = pts[0];

    for (float y = ymin; y < ymax + 1.0; y += 1.0) {
        int pcount = 0;
        float px[2];
        float py, t;

        for (size_t i = 0; i < n / 2; i ++) {
            SDL_FPoint spt = pts[i];
            SDL_FPoint ept = pts[i + 1];

            if (lines_intersect(spt.x, spt.y, ept.x, ept.y, xmin, y, xmax, y, px + pcount, &py, &t)) {
                if (flin(0.0, t, 1.0)) pcount += 1;
            } else if (pcount == 0) {
                px[0] = spt.x;
                px[1] = ept.x;
                pcount = 2;
            }
            
            if (pcount == 2) break;

            spt = pts[n - i];
            ept = pts[n - i - 1];

            if (lines_intersect(spt.x, spt.y, ept.x, ept.y, xmin, y, xmax, y, px + pcount, &py, &t)) {
                if (flin(0.0, t, 1.0)) pcount += 1;
            } else if (pcount == 0) {
                px[0] = spt.x;
                px[1] = ept.x;
                pcount = 2;
            }
            
            if (pcount == 2) break;
        }

        if (pcount == 2) {
            SDL_RenderDrawLineF(renderer, px[0], y, px[1], y);
        } else if (n == 2) {
            SDL_RenderDrawPointF(renderer, px[0], py);
        } else if (n <= 1) {
            SDL_RenderDrawPointF(renderer, cx, cy);
        }
    }

#ifdef __windows__
    delete [] pts;
#endif
}

/*************************************************************************************************/
template<typename T>
static inline void safe_render_text_surface(dc_t* dc, SDL_Surface* message, T x, T y) {
    /** TODO: Cache the textures of text **/

    if (message != nullptr) {
        dc->stamp(message, x, y);
        SDL_FreeSurface(message);
    }
}

static inline void setup_for_text(const std::string& text, const RGBA& rgb, SDL_Color& c) {
    rgb.unbox(&c.r, &c.g, &c.b, &c.a);
}

static TTF_Font* select_font(const shared_font_t& sfont, const std::string& text) {
    shared_font_t f = sfont;

    if (!f->is_suitable(text)) {
        f = f->try_fallback_for_unicode();
    }

    if (!f->okay()) {
        f = GameFont::Default();
    }

    return f->self();
}

static SDL_Surface* game_text_surface(bool disable_font_selection, const std::string& text, const shared_font_t& sfont, TextRenderMode mode, const RGBA& fg, const RGBA& bg, int wrap) {
    SDL_Surface* surface = nullptr;
    TTF_Font* font = (disable_font_selection) ? sfont->self() : select_font(sfont, text);
    SDL_Color fgc, bgc;

    if (!text.empty()) {
        setup_for_text(text, fg, fgc);
        setup_for_text(text, bg, bgc);

#ifndef __windows__
        if (wrap >= 0) { // will wrap by newline for 0
            switch (mode) {
                case ::TextRenderMode::Solid: surface = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), fgc, wrap); break;
                case ::TextRenderMode::Shaded: surface = TTF_RenderUTF8_Shaded_Wrapped(font, text.c_str(), fgc, bgc, wrap); break;
                case ::TextRenderMode::Blender: surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), fgc, wrap); break;
                case ::TextRenderMode::LCD: surface = TTF_RenderUTF8_LCD_Wrapped(font, text.c_str(), fgc, bgc, wrap); break;
            }
        } else {
#endif
            switch (mode) {
                case ::TextRenderMode::Solid: surface = TTF_RenderUTF8_Solid(font, text.c_str(), fgc); break;
                case ::TextRenderMode::Shaded: surface = TTF_RenderUTF8_Shaded(font, text.c_str(), fgc, bgc); break;
                case ::TextRenderMode::Blender: surface = TTF_RenderUTF8_Blended(font, text.c_str(), fgc); break;
#ifndef __windows__
                case ::TextRenderMode::LCD: surface = TTF_RenderUTF8_LCD(font, text.c_str(), fgc, bgc); break;
#endif
            }

#ifndef __windows__
        }
#endif

        if (surface == nullptr) {
            fprintf(stderr, "无法渲染文本: %s\n", TTF_GetError());
        }
    }

    return surface;
}

/*************************************************************************************************/
DrawingContext::DrawingContext(SDL_Renderer* device) : device(device) {
    // TODO: ensure that the device is not a nullptr.

    SDL_GetRendererInfo(this->device, &info);
}

DrawingContext::~DrawingContext() noexcept {
    if (this->device != nullptr) {
        SDL_DestroyRenderer(this->device);
    }
}

SDL_Surface* DrawingContext::snapshot(int width, int height) {
    uint32_t format = SDL_PIXELFORMAT_RGBA8888;
    SDL_Surface* photograph = game_formatted_surface(width, height, format);

    if (photograph != nullptr) {
        if (SDL_RenderReadPixels(this->device, NULL, format, photograph->pixels, photograph->pitch) < 0) {
            SDL_FreeSurface(photograph);
            photograph = nullptr;
        }
    }

    return photograph;
}

/*************************************************************************************************/
void Plteen::DrawingContext::clear(const RGBA& color) {
    // the `alpha` might not affect the underline window instance

    this->set_draw_color(color);
    SDL_RenderClear(this->device);
}

void Plteen::DrawingContext::reset(const RGBA& fgc, const RGBA& bgc) {
    this->clear(bgc);
    this->set_draw_color(fgc);
}

void Plteen::DrawingContext::reset(SDL_Texture* texture, const RGBA& fgc, const RGBA& bgc) {
    SDL_SetRenderTarget(this->device, texture);
    this->reset(fgc, bgc);
}

void Plteen::DrawingContext::refresh(SDL_Texture* texture) {
    SDL_SetRenderTarget(this->device, nullptr);
    SDL_RenderCopy(this->device, texture, nullptr, nullptr);
    SDL_RenderPresent(this->device);
    SDL_SetRenderTarget(this->device, texture);
}

int Plteen::DrawingContext::set_draw_color(const RGBA& color) {
    return SDL_SetRenderDrawColor(this->device, color.R(), color.G(), color.B(), color.A());
}

/*************************************************************************************************/
SDL_Texture* Plteen::DrawingContext::create_blank_image(int width, int height) {
    return game_blank_image(this->device, width, height);
}

SDL_Texture* Plteen::DrawingContext::create_blank_image(float width, float height) {
    return game_blank_image(this->device, width, height);
}

/*************************************************************************************************/
void Plteen::DrawingContext::draw_frame(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Rect box;

    FILL_BOX(box, x - 1, y - 1, width + 3, height + 3);
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawRect(this->device, &box);
}

void Plteen::DrawingContext::draw_grid(int row, int col, int cell_width, int cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int xoff, int yoff) {
    int xend = xoff + col * cell_width;
    int yend = yoff + row * cell_height;

    SDL_SetRenderDrawColor(this->device, r, g, b, a);

    for (int c = 0; c <= col; c++) {
        int x = xoff + c * cell_width;

        for (int r = 0; r <= row; r++) {
            int y = yoff + r * cell_height;

            SDL_RenderDrawLine(this->device, xoff, y, xend, y);
        }

        SDL_RenderDrawLine(this->device, x, yoff, x, yend);
    }
}

void Plteen::DrawingContext::fill_grid(int* grids[], int row, int col, int cell_width, int cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int xoff, int yoff) {
    SDL_Rect cell_self;

    cell_self.w = cell_width;
    cell_self.h = cell_height;

    SDL_SetRenderDrawColor(this->device, r, g, b, a);

    for (int c = 0; c < col; c++) {
        for (int r = 0; r < row; r++) {
            if (grids[r][c] > 0) {
                cell_self.x = xoff + c * cell_self.w;
                cell_self.y = yoff + r * cell_self.h;
                SDL_RenderFillRect(this->device, &cell_self);
            }
        }
    }
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, int x, int y, SDL_RendererFlip flip, double angle) {
    this->stamp(surface, x, y, surface->w, surface->h, flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, int x, int y, int width, int height, SDL_RendererFlip flip, double angle) {
    SDL_Rect box;

    FILL_BOX(box, x, y, width, height);
    this->stamp(surface, &box, flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, SDL_Rect* region, SDL_RendererFlip flip, double angle) {
    this->stamp(surface, nullptr, region, flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip, double angle) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->device, surface);

    if (texture != nullptr) {
        this->stamp(texture, src, dst, flip, angle);
        SDL_DestroyTexture(texture);
    }
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, int x, int y, SDL_RendererFlip flip, double angle) {
    int width, height;

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    
    return this->stamp(texture, x, y, width, height, flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, int x, int y, int width, int height, SDL_RendererFlip flip, double angle) {
    SDL_Rect box;
    
    FILL_BOX(box, x, y, width, height);
    
    return this->stamp(texture, &box, flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, SDL_Rect* region, SDL_RendererFlip flip, double angle) {
    return this->stamp(texture, nullptr, region, flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, SDL_RendererFlip flip, double angle) {
    if ((flip == SDL_FLIP_NONE) && (angle == 0.0)) {
        return SDL_RenderCopy(this->device, texture, src, dst);
    } else {
        return SDL_RenderCopyEx(this->device, texture, src, dst, angle, nullptr, flip);
    }
}

/**************************************************************************************************/
void Plteen::DrawingContext::draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawPoint(this->device, x, y);
}

void Plteen::DrawingContext::draw_line(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aalineRGBA(this->device, x1, y1, x2, y2, r, g, b, a);
}

void Plteen::DrawingContext::draw_hline(int x, int y, int length, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aalineRGBA(this->device, x, y, x + length, y, r, g, b, a);
}

void Plteen::DrawingContext::draw_vline(int x, int y, int length, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aalineRGBA(this->device, x, y, x, y + length, r, g, b, a);
}

void Plteen::DrawingContext::draw_points(const SDL_Point* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawPoints(this->device, pts, size);
}

void Plteen::DrawingContext::draw_lines(const SDL_Point* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawLines(this->device, pts, size);
}

void Plteen::DrawingContext::draw_rect(SDL_Rect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawRect(this->device, box);
}

void Plteen::DrawingContext::fill_rect(SDL_Rect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderFillRect(this->device, box);
}

void Plteen::DrawingContext::draw_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Rect box;

    FILL_BOX(box, x, y, width, height);
    this->draw_rect(&box, r, g, b, a);
}

void Plteen::DrawingContext::fill_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Rect box;

    FILL_BOX(box, x, y, width, height);
    this->fill_rect(&box, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_rect(SDL_Rect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rounded_rect(box->x, box->y, box->w, box->h, rad, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_rect(SDL_Rect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rounded_rect(box->x, box->y, box->w, box->h, rad, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_rect(int x, int y, int width, int height, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rounded_rect(float(x), float(y), float(width), float(height), radius, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_rect(int x, int y, int width, int height, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rounded_rect(float(x), float(y), float(width), float(height), radius, r, g, b, a);
}

void Plteen::DrawingContext::draw_square(int cx, int cy, int apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rect(cx - apothem, cy - apothem, apothem * 2, apothem * 2, r, g, b, a);
}

void Plteen::DrawingContext::fill_square(int cx, int cy, int apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rect(cx - apothem, cy - apothem, apothem * 2, apothem * 2, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_square(int cx, int cy, int apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rounded_rect(cx - apothem, cy - apothem, apothem * 2, apothem * 2, rad, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_square(int cx, int cy, int apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rounded_rect(cx - apothem, cy - apothem, apothem * 2, apothem * 2, rad, r, g, b, a);
}

void Plteen::DrawingContext::draw_circle(int cx, int cy, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aacircleRGBA(this->device, cx, cy, radius, r, g, b, a);
}

void Plteen::DrawingContext::fill_circle(int cx, int cy, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    filledCircleRGBA(this->device, cx, cy, radius, r, g, b, a);
}

void Plteen::DrawingContext::draw_ellipse(int cx, int cy, int ar, int br, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aaellipseRGBA(this->device, cx, cy, ar, br, r, g, b, a);
}

void Plteen::DrawingContext::fill_ellipse(int cx, int cy, int ar, int br, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    filledEllipseRGBA(this->device, cx, cy, ar, br, r, g, b, a);
}

void Plteen::DrawingContext::draw_regular_polygon(size_t n, int cx, int cy, int radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_regular_polygon(n, float(cx), float(cy), float(radius), rotation, r, g, b, a);
}

void Plteen::DrawingContext::fill_regular_polygon(size_t n, int cx, int cy, int radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_regular_polygon(n, float(cx), float(cy), float(radius), rotation, r, g, b, a);
}

void Plteen::DrawingContext::draw_polygon(short* xs, short* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aapolygonRGBA(this->device, xs, ys, int(n), r, g, b, a);
}

void Plteen::DrawingContext::fill_polygon(short* xs, short* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    filledPolygonRGBA(this->device, xs, ys, int(n), r, g, b, a);
}

void Plteen::DrawingContext::draw_polygon(int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx, int dy) {
#ifndef __windows__
    short vxs[n];
    short vys[n];

    this->draw_polygon(vxs, vys, xs, ys, n, r, g, b, a, dx, dy);
#else
    short* vxs = new short[n];
    short* vys = new short[n];

    this->draw_polygon(vxs, vys, xs, ys, n, r, g, b, a, dx, dy);

    delete [] vxs;
    delete [] vys;
#endif
}

void Plteen::DrawingContext::fill_polygon(int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx, int dy) {
#ifndef __windows__
    short vxs[n];
    short vys[n];

    this->fill_polygon(vxs, vys, xs, ys, n, r, g, b, a, dx, dy);
#else
    short* vxs = new short[n];
    short* vys = new short[n];

    this->fill_polygon(vxs, vys, xs, ys, n, r, g, b, a, dx, dy);

    delete [] vxs;
    delete [] vys;
#endif
}

void Plteen::DrawingContext::draw_polygon(short* vxs, short* vys, int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx, int dy) {
    for (size_t idx = 0; idx < n; idx ++) {
        vxs[idx] = short(xs[idx] + dx);
        vys[idx] = short(ys[idx] + dy);
    }

    aapolygonRGBA(this->device, vxs, vys, int(n), r, g, b, a);
}

void Plteen::DrawingContext::fill_polygon(short* vxs, short* vys, int* xs, int* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int dx, int dy) {
    for (size_t idx = 0; idx < n; idx ++) {
        vxs[idx] = short(xs[idx] + dx);
        vys[idx] = short(ys[idx] + dy);
    }

    filledPolygonRGBA(this->device, vxs, vys, int(n), r, g, b, a);
}

/*************************************************************************************************/
void Plteen::DrawingContext::draw_frame(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_FRect box;

    FILL_BOX(box, x - 1.0F, y - 1.0F, width + 3.0F, height + 3.0F);
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawRectF(this->device, &box);
}

void Plteen::DrawingContext::draw_grid(int row, int col, float cell_width, float cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float xoff, float yoff) {
    float xend = xoff + col * cell_width;
    float yend = yoff + row * cell_height;

    SDL_SetRenderDrawColor(this->device, r, g, b, a);

    for (int c = 0; c <= col; c++) {
        float x = xoff + float(c) * cell_width;

        for (int r = 0; r <= row; r++) {
            float y = yoff + float(r) * cell_height;

            SDL_RenderDrawLineF(this->device, xoff, y, xend, y);
        }

        SDL_RenderDrawLineF(this->device, x, yoff, x, yend);
    }
}

void Plteen::DrawingContext::fill_grid(int* grids[], int row, int col, float cell_width, float cell_height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float xoff, float yoff) {
    SDL_FRect cell_self;

    cell_self.w = cell_width;
    cell_self.h = cell_height;

    SDL_SetRenderDrawColor(this->device, r, g, b, a);

    for (int c = 0; c < col; c++) {
        for (int r = 0; r < row; r++) {
            if (grids[r][c] > 0) {
                cell_self.x = xoff + float(c) * cell_self.w;
                cell_self.y = yoff + float(r) * cell_self.h;
                SDL_RenderFillRectF(this->device, &cell_self);
            }
        }
    }
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, float x, float y, SDL_RendererFlip flip, double angle) {
    this->stamp(surface, x, y, float(surface->w), float(surface->h), flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, float x, float y, float width, float height, SDL_RendererFlip flip, double angle) {
    SDL_FRect box;

    FILL_BOX(box, x, y, width, height);
    this->stamp(surface, &box, flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, SDL_FRect* region, SDL_RendererFlip flip, double angle) {
    this->stamp(surface, nullptr, region, flip, angle);
}

void Plteen::DrawingContext::stamp(SDL_Surface* surface, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip, double angle) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->device, surface);

    if (texture != nullptr) {
        this->stamp(texture, src, dst, flip, angle);
        SDL_DestroyTexture(texture);
    }
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, float x, float y, SDL_RendererFlip flip, double angle) {
    int width, height;

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    
    return this->stamp(texture, x, y, float(width), float(height), flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, float x, float y, float width, float height, SDL_RendererFlip flip, double angle) {
    SDL_FRect box;
    
    FILL_BOX(box, x, y, width, height);
    
    return this->stamp(texture, &box, flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, SDL_FRect* region, SDL_RendererFlip flip, double angle) {
    return this->stamp(texture, nullptr, region, flip, angle);
}

int Plteen::DrawingContext::stamp(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip, double angle) {
    if ((flip == SDL_FLIP_NONE) && (angle == 0.0)) {
        return SDL_RenderCopyF(this->device, texture, src, dst);
    } else {
        return SDL_RenderCopyExF(this->device, texture, src, dst, angle, nullptr, flip);
    }
}

/**************************************************************************************************/
void Plteen::DrawingContext::draw_point(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawPointF(this->device, x, y);
}

void Plteen::DrawingContext::draw_line(float x1, float y1, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    aalineRGBA(this->device, fl2fx<int16_t>(x1), fl2fx<int16_t>(y1), fl2fx<int16_t>(x2), fl2fx<int16_t>(y2), r, g, b, a);
}

void Plteen::DrawingContext::draw_hline(float x, float y, float length, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_hline(fl2fx<int>(x), fl2fx<int>(y), fl2fx<int>(length), r, g, b, a);
}

void Plteen::DrawingContext::draw_vline(float x, float y, float length, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_vline(fl2fx<int>(x), fl2fx<int>(y), fl2fx<int>(length), r, g, b, a);
}

void Plteen::DrawingContext::draw_points(const SDL_FPoint* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawPointsF(this->device, pts, size);
}

void Plteen::DrawingContext::draw_lines(const SDL_FPoint* pts, int size, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawLinesF(this->device, pts, size);
}

void Plteen::DrawingContext::draw_rect(SDL_FRect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderDrawRectF(this->device, box);
}

void Plteen::DrawingContext::fill_rect(SDL_FRect* box, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    SDL_RenderFillRectF(this->device, box);
}

void Plteen::DrawingContext::draw_rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_FRect box;

    FILL_BOX(box, x, y, width, height);
    this->draw_rect(&box, r, g, b, a);
}

void Plteen::DrawingContext::fill_rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_FRect box;

    FILL_BOX(box, x, y, width, height);
    this->fill_rect(&box, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_rect(SDL_FRect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rounded_rect(box->x, box->y, box->w, box->h, rad, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_rect(SDL_FRect* box, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rounded_rect(box->x, box->y, box->w, box->h, rad, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_rect(float x, float y, float width, float height, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t X1 = fl2fx<int16_t>(x);
    int16_t Y1 = fl2fx<int16_t>(y);
    int16_t X2 = fl2fx<int16_t>(x + width);
    int16_t Y2 = fl2fx<int16_t>(y + height);
    int16_t rad;

    if (radius < 0.0) {
        radius = fxmin(width, height) * (-radius);
    }

    rad = fl2fx<int16_t>(flmin(radius, height * 0.5F));
    roundedRectangleRGBA(this->device, X1, Y1, X2, Y2, rad, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_rect(float x, float y, float width, float height, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t X1 = fl2fx<int16_t>(x);
    int16_t Y1 = fl2fx<int16_t>(y);
    int16_t X2 = fl2fx<int16_t>(x + width);
    int16_t Y2 = fl2fx<int16_t>(y + height);
    int16_t rad;

    if (radius < 0.0) {
        radius = fxmin(width, height) * (-radius);
    }

    rad = fl2fx<int16_t>(flmin(radius, height * 0.5F));
    roundedBoxRGBA(this->device, X1, Y1, X2, Y2, rad, r, g, b, a);
}

void Plteen::DrawingContext::draw_square(float cx, float cy, float apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rect(cx - apothem, cy - apothem, apothem * 2.0F, apothem * 2.0F, r, g, b, a);
}

void Plteen::DrawingContext::fill_square(float cx, float cy, float apothem, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rect(cx - apothem, cy - apothem, apothem * 2.0F, apothem * 2.0F, r, g, b, a);
}

void Plteen::DrawingContext::draw_rounded_square(float cx, float cy, float apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->draw_rounded_rect(cx - apothem, cy - apothem, apothem * 2.0F, apothem * 2.0F, rad, r, g, b, a);
}

void Plteen::DrawingContext::fill_rounded_square(float cx, float cy, float apothem, float rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->fill_rounded_rect(cx - apothem, cy - apothem, apothem * 2.0F, apothem * 2.0F, rad, r, g, b, a);
}

void Plteen::DrawingContext::draw_circle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t CX = fl2fx<int16_t>(cx);
    int16_t CY = fl2fx<int16_t>(cy);
    int16_t R = fl2fx<int16_t>(radius);
    
    aacircleRGBA(this->device, CX, CY, R, r, g, b, a);
}

void Plteen::DrawingContext::fill_circle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t CX = fl2fx<int16_t>(cx);
    int16_t CY = fl2fx<int16_t>(cy);
    int16_t R = fl2fx<int16_t>(radius);
    
    filledCircleRGBA(this->device, CX, CY, R, r, g, b, a);
    aacircleRGBA(this->device, CX, CY, R, r, g, b, a);
}

void Plteen::DrawingContext::draw_ellipse(float cx, float cy, float ar, float br, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t CX = fl2fx<int16_t>(cx);
    int16_t CY = fl2fx<int16_t>(cy);
    int16_t AR = fl2fx<int16_t>(ar);
    int16_t BR = fl2fx<int16_t>(br);
    
    if (AR == BR) {
        aacircleRGBA(this->device, CX, CY, AR, r, g, b, a);
    } else {
        aaellipseRGBA(this->device, CX, CY, AR, BR, r, g, b, a);
    }
}

void Plteen::DrawingContext::fill_ellipse(float cx, float cy, float ar, float br, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int16_t CX = fl2fx<int16_t>(cx);
    int16_t CY = fl2fx<int16_t>(cy);
    int16_t AR = fl2fx<int16_t>(ar);
    int16_t BR = fl2fx<int16_t>(br);
    
    if (AR == BR) {
        filledCircleRGBA(this->device, CX, CY, AR, r, g, b, a);
        aacircleRGBA(this->device, CX, CY, BR, r, g, b, a);
    } else {
        filledEllipseRGBA(this->device, CX, CY, AR, BR, r, g, b, a);
        aaellipseRGBA(this->device, CX, CY, AR, BR, r, g, b, a);
    }
}

void Plteen::DrawingContext::draw_regular_polygon(size_t n, float cx, float cy, float radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    pen_draw_regular_polygon(this->device, n, cx, cy, radius, rotation);
}

void Plteen::DrawingContext::fill_regular_polygon(size_t n, float cx, float cy, float radius, float rotation, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->device, r, g, b, a);
    pen_fill_regular_polygon(this->device, n, cx, cy, radius, rotation);
}

void Plteen::DrawingContext::draw_polygon(float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx, float dy) {
#ifndef __windows__
    short vxs[n];
    short vys[n];

    this->draw_polygon(vxs, vys, xs, ys, n, r, b, g, a, dx, dy);
#else
    short* vxs = new short[n];
    short* vys = new short[n];

    this->draw_polygon(vxs, vys, xs, ys, n, r, b, g, a, dx, dy);
    
    delete [] vxs;
    delete [] vys;
#endif
}

void Plteen::DrawingContext::fill_polygon(float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx, float dy) {
#ifndef __windows__
    short vxs[n];
    short vys[n];

    this->fill_polygon(vxs, vys, xs, ys, n, r, b, g, a, dx, dy);
#else
    short* vxs = new short[n];
    short* vys = new short[n];

    this->fill_polygon(vxs, vys, xs, ys, n, r, b, g, a, dx, dy);

    delete [] vxs;
    delete [] vys;
#endif
}

void Plteen::DrawingContext::draw_polygon(short* vxs, short* vys, float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx, float dy) {
    for (size_t idx = 0; idx < n; idx ++) {
        vxs[idx] = fl2fx<short>(xs[idx] + dx);
        vys[idx] = fl2fx<short>(ys[idx] + dy);
    }

    aapolygonRGBA(this->device, vxs, vys, int(n), r, g, b, a);
}

void Plteen::DrawingContext::fill_polygon(short* vxs, short* vys, float* xs, float* ys, size_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float dx, float dy) {
    for (size_t idx = 0; idx < n; idx ++) {
        vxs[idx] = fl2fx<short>(xs[idx] + dx);
        vys[idx] = fl2fx<short>(ys[idx] + dy);
    }

    filledPolygonRGBA(this->device, vxs, vys, int(n), r, g, b, a);
}

/*************************************************************************************************/
SDL_Texture* Plteen::DrawingContext::create_text_texture(const std::string& text, const shared_font_t& font, TextRenderMode mode, const RGBA& fgc, const RGBA& bgc, int wrap) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = game_text_surface(this->_disable_font_selection, text, font, mode, fgc, bgc, wrap);

    if (surface != nullptr) {
        texture = SDL_CreateTextureFromSurface(this->device, surface);
        SDL_FreeSurface(surface);
    }

    return texture;
}

SDL_Texture* Plteen::DrawingContext::create_solid_text(const std::string& text, const shared_font_t& font, const RGBA& fgc, int wrap) {
    return this->create_text_texture(text, font, TextRenderMode::Solid, fgc, fgc, wrap);
}

SDL_Texture* Plteen::DrawingContext::create_shaded_text(const std::string& text, const shared_font_t& font, const RGBA& fgc, const RGBA& bgc, int wrap) {
    return this->create_text_texture(text, font, TextRenderMode::Shaded, fgc, bgc, wrap);
}

SDL_Texture* Plteen::DrawingContext::create_lcd_text(const std::string& text, const shared_font_t& font, const RGBA& fgc, const RGBA& bgc, int wrap) {
    return this->create_text_texture(text, font, TextRenderMode::LCD, fgc, bgc, wrap);
}

SDL_Texture* Plteen::DrawingContext::create_blended_text(const std::string& text, const shared_font_t& font, const RGBA& fgc, int wrap) {
    return this->create_text_texture(text, font, TextRenderMode::Blender, fgc, fgc, wrap);
}

void Plteen::DrawingContext::draw_solid_text(const std::string& text, const shared_font_t& font, int x, int y, const RGBA& rgb, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Solid, rgb, rgb, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_shaded_text(const std::string& text, const shared_font_t& font, int x, int y, const RGBA& fgc, const RGBA& bgc, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Shaded, fgc, bgc, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_lcd_text(const std::string& text, const shared_font_t& font, int x, int y, const RGBA& fgc, const RGBA& bgc, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::LCD, fgc, bgc, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_blended_text(const std::string& text, const shared_font_t& font, int x, int y, const RGBA& rgb, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Blender, rgb, rgb, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_solid_text(const std::string& text, const shared_font_t& font, float x, float y, const RGBA& rgb, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Solid, rgb, rgb, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_shaded_text(const std::string& text, const shared_font_t& font, float x, float y, const RGBA& fgc, const RGBA& bgc, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Shaded, fgc, bgc, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_lcd_text(const std::string& text, const shared_font_t& font, float x, float y, const RGBA& fgc, const RGBA& bgc, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::LCD, fgc, bgc, wrap);
    safe_render_text_surface(this, message, x, y);
}

void Plteen::DrawingContext::draw_blended_text(const std::string& text, const shared_font_t& font, float x, float y, const RGBA& rgb, int wrap) {
    SDL_Surface* message = game_text_surface(this->_disable_font_selection, text, font, ::TextRenderMode::Blender, rgb, rgb, wrap);
    safe_render_text_surface(this, message, x, y);
}
