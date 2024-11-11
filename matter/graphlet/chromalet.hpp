#pragma once

#include "../canvaslet.hpp"
#include "../../physics/color/CIE.hpp"
#include "../../physics/algebra/vector.hpp"
#include "../../physics/geometry/aabox.hpp"

namespace Plteen {
    class __lambda__ Chromalet : public Plteen::ICanvaslet {
    public:
        Chromalet(float width, float height = 0.0F, Plteen::CIE_Standard std = CIE_Standard::Primary, double Y = 1.0);
        
    public:
        Plteen::Box get_bounding_box() override;
        bool is_colliding(const Plteen::cPoint& local_pt) override;

    public:
        void set_standard(Plteen::CIE_Standard std = CIE_Standard::Primary);
        Plteen::CIE_Standard get_standard() { return this->standard; }
        void set_luminance(double Y = 1.0);

    public:
        void recalculate_primary_colors(int idx = -1);
        void set_pseudo_primary_triangle_color(const Plteen::RGBA& color);
        void set_pseudo_primary_triangle_alpha(double alpha);
        void set_pseudo_primary_color(const Plteen::RGBA& color, size_t idx);
        void feed_pseudo_primary_color_location(size_t idx, float* x, float* y, float* fx = nullptr, float* fy = nullptr);
        void feed_pseudo_primary_color_location(size_t idx, double* x, double* y, double* fx = nullptr, double* fy = nullptr);
        void feed_color_location(const Plteen::RGBA& color, float* x, float* y, float* fx = nullptr, float* fy = nullptr);
        void feed_color_location(const Plteen::RGBA& color, double* x, double* y, double* fx = nullptr, double* fy = nullptr);
        Plteen::RGBA get_color_at(float mx, float my, bool after_special = false) { return this->get_color_at(double(mx), double(my), after_special); }
        Plteen::RGBA get_color_at(double mx, double my, bool after_special = false);
        bool is_point_inside_the_spectrum(double x, double y);
        
    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        
    protected:
        void draw_before_canvas(Plteen::dc_t* dc, float x, float y, float Width, float Height) override;
        void draw_on_canvas(Plteen::dc_t* dc, float Width, float Height) override;
        void draw_after_canvas(Plteen::dc_t* dc, float x, float y, float Width, float Height) override;
        void on_canvas_invalidated() override;
    
    private:
        void draw_color_triangle(Plteen::dc_t* dc, double dx = 0.0, double dy = 0.0);
        void draw_color_map(Plteen::dc_t* dc, double width, double height, double dx = 0.0, double dy = 0.0);
        void draw_spectral_locus(Plteen::dc_t* dc, double width, double height, double dx = 0.0, double dy = 0.0);
        void draw_chromaticity(Plteen::dc_t* dc, double width, double height, double dx = 0.0, double dy = 0.0);

    private:
        void fix_render_location(double* x, double* y);
        void make_locus_polygon(double width, double height);
        void spectrum_intersection_vpoints(double x, double flheight, int& slt_idx, int& slb_idx, double* ty, double* by);
        void render_special_dot(Plteen::dc_t* dc, const Plteen::RGBA& c, float x, float y);
        void render_dot(Plteen::dc_t* dc, double x, double y, double width, double height,
                            double R, double G, double B, double dx, double dy, double A = 1.0);

    private:
        void invalidate_locus();

    private:
        float width;
        float height;

    private:
        Plteen::CIE_Standard standard;
        Plteen::RGBA pseudo_primary_triangle_color;
        Plteen::RGBA pseudo_primaries[3];
        double luminance;
        
    private:
        double* locus_xs = nullptr;
        double* locus_ys = nullptr;
        size_t locus_count = 0U;
        double scanline_start = 0.0;
        double scanline_end = 0.0;
        int scanline_idx0 = 0U;
    };
}
