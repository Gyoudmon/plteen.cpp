#pragma once

#include "../../../atlas.hpp"

#include <utility>

namespace Plteen {
    enum class MarioPipeColor {
        // Order matters
        Green = 0,
        Gray = 1,
        Red = 2,
        Steel = 3,
        Orange = 4,
        Blue = 5
    };

    enum class MarioVPipeDirection { Up, Down, Both };
    enum class MarioHPipeDirection { Left, Right, Both };

    /*********************************************************************************************/
    class __lambda__ MarioGroundAtlas : public Plteen::GridAtlas {
    public:
        MarioGroundAtlas(size_t slot, int row, int col, float tile_size = 0.0F);

    protected:
        int get_atlas_tile_index(size_t map_idx, int& xoff, int& yoff) override;
        
    protected:
        virtual void feed_ground_tile_index(size_t map_idx, int& row, int& col);
    
    private:
        size_t slot = 0;
    };

    /*********************************************************************************************/
    class __lambda__ MarioPipe : public Plteen::GridAtlas {
    public:
        MarioPipe(int row, int col,
            Plteen::MarioPipeColor color = MarioPipeColor::Green,
            float tile_size = 0.0F);

    public:
        void set_color(Plteen::MarioPipeColor color);
        Plteen::MarioPipeColor get_color();

    protected:
        int color_idx;
    };

    class __lambda__ MarioVPipe : public Plteen::MarioPipe {
    public:
        MarioVPipe(int length,
            Plteen::MarioVPipeDirection dir = MarioVPipeDirection::Up,
            Plteen::MarioPipeColor color = MarioPipeColor::Green,
            float tile_size = 0.0F);

    protected:
        int get_atlas_tile_index(size_t map_idx, int& xoff, int& yoff) override;
        
    private:
        Plteen::MarioVPipeDirection direction;
    };

    class __lambda__ MarioHPipe : public Plteen::MarioPipe {
    public:
        MarioHPipe(int length,
            Plteen::MarioHPipeDirection dir = MarioHPipeDirection::Both,
            Plteen::MarioPipeColor color = MarioPipeColor::Green,
            float tile_size = 0.0F);

    protected:
        int get_atlas_tile_index(size_t map_idx, int& xoff, int& yoff) override;
        
    private:
        Plteen::MarioHPipeDirection direction;
    };
}
