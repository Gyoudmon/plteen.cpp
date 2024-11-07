#pragma once

#include "../../canvaslet.hpp"
#include "../../../physics/geometry/aabox.hpp"

#include <vector>

namespace Plteen {
    class __lambda__ Radarlet : public Plteen::ICanvaslet {
    public:
        Radarlet(size_t n, float radius, const Plteen::RGBA& color = GRAY, double fill_alpha = 0.0);

    public:
        Plteen::Box get_bounding_box() override;
        const char* name() override { return this->_name.c_str(); }

    public:
        void set_title(const char* title, ...);
        void set_title(const std::string& title) { this->set_title(title, Plteen::MatterPort::CB); }
        void set_title(Plteen::MatterPort anchor, const char* title, ...);
        void set_title(const std::string& title, Plteen::MatterPort anchor);

        void set_main_spoke(size_t idx);
        void set_start_angle(double angle, bool is_radian = true);
        
        void set_variable_names(const char* variables[], Plteen::MatterPort anchor = Plteen::MatterPort::CC);
        void set_levels(const double* levels, size_t n);
        void set_range(double range);
        
        double range() const { return this->variable_range; }
        size_t variable_length() const { return this->n; }
        void push_observation(const double* variables, const Plteen::RGBA& pen_color, double fill_alpha = 0.0);
        void set_observation_colors(size_t idx, const Plteen::RGBA& pen_color, double fill_alpha = 0.0);
        void set_observation(size_t idx, const double* variables);
        void clear_observations();

    public:
        template<size_t N>
        void set_levels(const double (&levels)[N]) { this->set_levels(levels, N); }

    protected:
        void draw_on_canvas(Plteen::dc_t* dc, float Width, float Height) override;

    private:
        void draw_variable(Plteen::dc_t* dc, double percentage, float x, float y, const Plteen::shared_font_t& font, const Plteen::RGBA& color);
        void draw_variable_name(Plteen::dc_t* dc, size_t idx, float spoke_x, float spoke_y, float ox, float oy);
        
    private:
        size_t n;
        size_t mspoke;
        float radius;
        double variable_range;

    private:
        std::vector<std::vector<double>> observations;
        std::vector<Plteen::RGBA> colors;
        std::vector<Plteen::RGBA> fill_colors;
        std::vector<double> levels;
        std::vector<float> spoke_xs;
        std::vector<float> spoke_ys;

    private:
        Plteen::shared_texture_t title;
        std::vector<Plteen::shared_texture_t> variables;
        
    private:
        std::string _name;
        float padding_width;
        float padding_height;
    };
}
