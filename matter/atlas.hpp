#pragma once

#include "../matter.hpp"
#include "../physics/color/rgba.hpp"
#include "../physics/algebra/vector.hpp"
#include "../physics/geometry/port.hpp"
#include "../physics/geometry/aabox.hpp"
#include "../physics/geometry/margin.hpp"

#include "../virtualization/filesystem/imgdb.hpp"

namespace Plteen {
    class __lambda__ IAtlas : public Plteen::IMatter {
    public:
        IAtlas(const std::string& pathname);
        IAtlas(const char* pathname) : IAtlas(std::string(pathname)) {}
        virtual ~IAtlas() {}

        void construct(Plteen::dc_t* dc) override;
        const char* name() override;

    public:
        Plteen::Box get_bounding_box() override;
        Plteen::Box get_original_bounding_box() override;
        Plteen::Margin get_margin() override;
        void draw(Plteen::dc_t* dc, float x, float y, float Width, float Height) override;
        
    public:
        virtual size_t atlas_tile_count() = 0;
        virtual size_t map_tile_count() = 0;

    public:
        int preferred_local_fps() override { return 4; }
        
    public:
        size_t logic_tile_count();
        void create_logic_grid(int row, int col, const Plteen::Margin& margin = 0.0F);
        int logic_tile_index(int x, int y, int* r = nullptr, int* c = nullptr, bool local = true);
        int logic_tile_index(float x, float y, int* r = nullptr, int* c = nullptr, bool local = true);
        Plteen::Box get_logic_tile_region();
        Plteen::Port get_logic_tile_fraction(int idx, const Port& a = 0.5F);
        Plteen::Port get_logic_tile_fraction(int row, int col, const Port& a = 0.5F);
        Plteen::cPoint get_logic_tile_location(int idx, const Port& a = 0.5F, bool local = true);
        Plteen::cPoint get_logic_tile_location(int row, int col, const Port& a = 0.5F, bool local = true);
        void set_logic_grid_color(const Plteen::RGBA& color) { this->logic_grid_color = color; }

    public:
        void move_to_logic_tile(IMatter* m, int idx, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void move_to_logic_tile(IMatter* m, int row, int col, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void glide_to_logic_tile(double sec, IMatter* m, int idx, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void glide_to_logic_tile(double sec, IMatter* m, int row, int col, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        
    protected:
        virtual int get_atlas_tile_index(size_t map_idx, int& xoff, int& yoff) { return int(map_idx); }
        virtual Plteen::Box get_map_region() = 0;
        virtual void on_tilemap_load(Plteen::shared_texture_t atlas) = 0;
        virtual Plteen::AABox<int> get_atlas_tile_region(size_t idx) = 0;
        virtual Plteen::Box get_map_tile_region(size_t idx) = 0;

    protected:
        void on_resize(float width, float height, float old_width, float old_height) override;
        
    protected:
        void invalidate_map_size() { this->map_region.invalidate(); }
        void on_map_resize(float map_width, float map_height);
        SDL_RendererFlip current_flip_status();
        float get_horizontal_scale();
        float get_vertical_scale();
        
    protected:
        float xscale = 1.0F;
        float yscale = 1.0F;

    private:
        Plteen::shared_texture_t atlas;

    private:
        Plteen::Box map_region;
        Plteen::Margin logic_margin;
        Plteen::RGBA logic_grid_color;
        float logic_tile_width = 0.0F;
        float logic_tile_height = 0.0F;
        int logic_row = 0;
        int logic_col = 0;

    private:
        std::string _pathname;
    };

    class __lambda__ GridAtlas : public Plteen::IAtlas {
    public:
        GridAtlas(const char* pathname, int row = 1, int col = 1, int xgap = 0, int ygap = 0, bool inset = false);
        GridAtlas(const std::string& pathname, int row = 1, int col = 1, int xgap = 0, int ygap = 0, bool inset = false);

    public:
        size_t atlas_tile_count() override;
        size_t map_tile_count() override;
        float atlas_tile_size_ratio();
        float map_tile_size_ratio();

    public:
        int map_tile_index(int x, int y, int* r = nullptr, int* c = nullptr, bool local = true);
        int map_tile_index(float x, float y, int* r = nullptr, int* c = nullptr, bool local = true);
        Plteen::Port get_map_tile_fraction(int idx, const Port& a = 0.5F);
        Plteen::Port get_map_tile_fraction(int row, int col, const Port& a = 0.5F);
        Plteen::cPoint get_map_tile_location(int idx, const Port& a = 0.5F, bool local = true);
        Plteen::cPoint get_map_tile_location(int row, int col, const Port& a = 0.5F, bool local = true);
        Plteen::Margin get_map_overlay();
        
    public:
        void move_to_map_tile(IMatter* m, int idx, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void move_to_map_tile(IMatter* m, int row, int col, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void glide_to_map_tile(double sec, IMatter* m, int idx, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);
        void glide_to_map_tile(double sec, IMatter* m, int row, int col, const Port& tp, const Port& p, const cVector& vec = Plteen::cO);

    protected:
        virtual Plteen::Margin get_original_map_overlay() { return this->get_original_margin(); }

    protected:
        void create_map_grid(int row, int col, float tile_width = 0.0F, float tile_height = 0.0F, float xgap = 0.0F, float ygap = 0.0F);
        
    protected:
        void on_tilemap_load(Plteen::shared_texture_t atlas) override;
        Plteen::Box get_map_region() override;
        Plteen::AABox<int> get_atlas_tile_region(size_t idx) override;
        Plteen::Box get_map_tile_region(size_t idx) override;
    
    protected:
        int atlas_row;
        int atlas_col;
        int map_row = 0;
        int map_col = 0;

    protected:
        float map_tile_xgap = 0.0F;
        float map_tile_ygap = 0.0F;
        float map_tile_width = 0.0F;
        float map_tile_height = 0.0F;

    private:
        bool atlas_inset; 
        int atlas_tile_xgap = 0;
        int atlas_tile_ygap = 0;
        int atlas_tile_width = 0;
        int atlas_tile_height = 0;
    };
}
