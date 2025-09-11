#pragma once

#include "the_bang.hpp"

#include <utility>

namespace Plteen {
    typedef std::pair<char, const char*> facility_item_t;

    template<size_t N>
    struct __lambda__ TextFacilityConfig {
        struct Keys { facility_item_t self[N]; } keys;
        struct States { bool self[N][N]; } states;
    
        constexpr TextFacilityConfig(
            std::initializer_list<facility_item_t> ordered_keys,
            std::initializer_list<std::initializer_list<bool>> states_init
        ) {
            size_t i = 0;
            
            std::copy(ordered_keys.begin(), ordered_keys.end(), keys.self);
            for (const auto& row : states_init) {
                std::copy(row.begin(), row.end(), states.self[i++]);
            }
        }
    };

    /*********************************************************************************************/
    class __lambda__ TextFacilityPlane : public virtual Plteen::TheBigBang {
    public:
        TextFacilityPlane(const facility_item_t ordered_keys[], size_t n, const bool* default_states
            , const char* name = unknown_plane_name, const Plteen::RGBA& title_color = BLACK)
            : TextFacilityPlane(Plteen::GameFont::monospace(), ordered_keys, n, default_states, name, title_color) {}

        TextFacilityPlane(const Plteen::shared_font_t& font, const facility_item_t ordered_keys[], size_t n, const bool* default_states
            , const char* name = unknown_plane_name, const Plteen::RGBA& title_color = BLACK);

        virtual ~TextFacilityPlane() noexcept;

    public:
        template<size_t N>
        TextFacilityPlane(const TextFacilityConfig<N>& c, const char* name = unknown_plane_name, const Plteen::RGBA& title_color = BLACK)
            : TextFacilityPlane(c.keys.self, N, reinterpret_cast<const bool*>(c.states.self), name, title_color) {}

        template<size_t N>
        TextFacilityPlane(const Plteen::shared_font_t& font, const TextFacilityConfig<N>& c
            , const char* name = unknown_plane_name, const Plteen::RGBA& title_color = BLACK)
            : TextFacilityPlane(font, c.keys.self, N, reinterpret_cast<const bool*>(c.states.self), name, title_color) {}

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    public:
        void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;
        void on_tap(Plteen::IMatter* m, float local_x, float local_y) override;

    public:
        void set_working_facility_color(const Plteen::RGBA& color);
        void set_enabled_facility_color(const Plteen::RGBA& color);
        void set_disabled_facility_color(const Plteen::RGBA& color, const Plteen::RGBA& hi_color);
        void set_disabled_facility_color(const Plteen::RGBA& color) { this->set_disabled_facility_color(color, color); }

    protected:
        size_t facility_count() { return this->n; }
        Plteen::Labellet* facility_ref(int idx);
        bool trigger_facility(char key);

    protected:
        virtual void reflow_facility(Plteen::Labellet* facility, Plteen::Labellet* prev, float width, float height);
        virtual void on_facility_command(size_t idx, char key, float width, float height) {}

    private:
        void initialize_facilities(size_t n);
        void deal_with_facility_at(size_t idx);

    private:
        Plteen::Labellet** facilities = nullptr;
        Plteen::shared_font_t facility_font;
        Plteen::RGBA working_facility_color = GREEN;
        Plteen::RGBA enabled_facility_color = ROYALBLUE;
        Plteen::RGBA disabled_facility_color = GRAY;
        Plteen::RGBA hi_disabled_facility_color = CRIMSON;

    private:
        size_t working_idx;
        char* _fkeys = nullptr;
        bool** _fokay = nullptr;
        std::string* _fdescs = nullptr;
        size_t n = 0;
    };
}
