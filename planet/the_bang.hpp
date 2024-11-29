#pragma once

#include "../game.hpp"

#include <utility>

namespace Plteen {
    /* Constants */
    static const char* unknown_plane_name = "冒险越来越深入了";

    /*********************************************************************************************/
    class __lambda__ TheBigBang : public Plteen::Plane {
    public:
        TheBigBang(const char* name = unknown_plane_name, const Plteen::RGBA& title_color = BLACK)
            : Plteen::Plane(name), title_color(title_color) {}
        
        virtual ~TheBigBang() noexcept {}

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    protected:
        virtual const char* the_title_prefix() const { return "青少计算机科学"; }

    protected:
        float get_titlebar_height();
    
    protected:
        Plteen::Linkmon* agent = nullptr;
        Plteen::Labellet* title = nullptr;
        Plteen::Labellet* tooltip = nullptr;
    
    private:
        Plteen::RGBA title_color;
    };
}
