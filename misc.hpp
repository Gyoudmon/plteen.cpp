#pragma once

#include <SDL2/SDL.h>
#include <complex>

/*************************************************************************************************/
namespace Plteen {
    bool inline is_shift_pressed() { return (SDL_GetModState() & KMOD_SHIFT); }
    bool inline is_ctrl_pressed() { return (SDL_GetModState() & KMOD_CTRL); }
    bool inline is_alt_pressed() { return (SDL_GetModState() & KMOD_ALT); }
    bool inline is_cmd_pressed() { return (SDL_GetModState() & KMOD_GUI); }

    std::complex<float> inline get_current_mouse_location() {
        int mx, my;

        SDL_GetMouseState(&mx, &my);
        
        return { float(mx), float(my) };
    }
}
