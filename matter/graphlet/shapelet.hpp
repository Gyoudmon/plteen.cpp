#pragma once

#include "../canvaslet.hpp"
#include "../../physics/algebra/vector.hpp"
#include "../../physics/geometry/aabox.hpp"
#include "../../physics/geometry/vertices.hpp"

namespace Plteen {
    class __lambda__ IShapelet : public Plteen::ICanvaslet {
    public:
        IShapelet(const Plteen::RGBA& color = transparent, const Plteen::RGBA& border_color = transparent);
        virtual ~IShapelet() noexcept {}

    public:
        void draw_on_canvas(Plteen::dc_t* dc, float Width, float Height) override;

    protected:
        virtual void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
        virtual void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    };

    /*********************************************************************************************/
    class __lambda__ Linelet : public Plteen::IShapelet {
    public:
	    Linelet(float ex, float ey, const Plteen::RGBA& color);

	public:
        Plteen::Box get_bounding_box() override;
        
    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override {}
        void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

    private:
        float epx;
        float epy;
    };

    class __lambda__ HLinelet : public Plteen::Linelet {
    public:
        HLinelet(float width, const Plteen::RGBA& color) : Linelet(width, 0.0F, color) {}
    };

    class __lambda__ VLinelet : public Plteen::Linelet {
    public:
        VLinelet(float height, const Plteen::RGBA& color) : Linelet(0.0F, height, color) {}
    };

    /*********************************************************************************************/
    class __lambda__ Rectanglet : public Plteen::IShapelet {
    public:
	    Rectanglet(float edge_size, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    Rectanglet(float width, float height, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    
    public:
        Plteen::Box get_bounding_box() override;

    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
        void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	private:
	    float width;
	    float height;
    };

    class __lambda__ Squarelet : public Plteen::Rectanglet {
    public:
        Squarelet(float edge_size, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent)
            : Rectanglet(edge_size, color, border_color) {}
    };

    class __lambda__ RoundedRectanglet : public Plteen::IShapelet {
    public:
	    RoundedRectanglet(float edge_size, float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    RoundedRectanglet(float width, float height, float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    
	public:
	    Plteen::Box get_bounding_box() override;
        
    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
        void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	private:
	    float width;
	    float height;
        float radius;
    };

    class __lambda__ RoundedSquarelet : public Plteen::RoundedRectanglet {
    public:
        RoundedSquarelet(float edge_size, float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent)
            : RoundedRectanglet(edge_size, edge_size, radius, color, border_color) {}
    };

    class __lambda__ Ellipselet : public Plteen::IShapelet {
    public:
	    Ellipselet(float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    Ellipselet(float aradius, float bradius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);

	public:
	    Plteen::Box get_bounding_box() override;
        
    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
        void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	private:
	    float aradius;
	    float bradius;
    };
    
    class __lambda__ Circlet : public Plteen::Ellipselet {
    public:
	    Circlet(float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent)
            : Ellipselet(radius, radius, color, border_color) {}
    };
    
    /*********************************************************************************************/
    class __lambda__ Polygonlet : public Plteen::IShapelet {
    public:
	    Polygonlet(const Plteen::Vertices& vertices, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    virtual ~Polygonlet();

	public:
	    Plteen::Box get_bounding_box() override;
        
    public:
        size_t side_count() { return this->n; }
        float leftmost_x() { return this->lx; }
        float topmost_y() { return this->ty; }
	    
    protected:
        void on_resize(float new_width, float new_height, float old_width, float old_height) override;
        void draw_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
        void fill_shape(Plteen::dc_t* dc, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

    private:
        void initialize_vertices(float xscale, float yscale);

	private:
        size_t n;
	    float* xs = nullptr;
        float* ys = nullptr;
        short* txs = nullptr;
        short* tys = nullptr;

    private:
        float lx;
        float ty;
        float rx;
        float by;
    };

    class __lambda__ RegularPolygonlet : public Plteen::Polygonlet {
    public:
	    RegularPolygonlet(size_t n, float radius, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    RegularPolygonlet(size_t n, float radius, float rotation, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);

    public:
        float radius() { return this->_radius; }

    private:
        float _radius;
	};

    /*********************************************************************************************/
    class __lambda__ Trianglet : public Plteen::RegularPolygonlet {
    public:
	    Trianglet(float side_length, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
	    Trianglet(float side_length, float rotation, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
    };

    class __lambda__ HexagonTilelet : public Plteen::Polygonlet {
    public:
	    HexagonTilelet(float width, float height, const Plteen::RGBA& color, const Plteen::RGBA& border_color = transparent);
    };
}
