#pragma once

#include "the_bang.hpp"

#include <utility>

namespace Plteen {
    typedef std::pair<char, const char*> cmdlet_item_t;

    template<size_t N>
    struct __lambda__ CmdletConfig {
        struct Keys { cmdlet_item_t self[N]; } keys;
        struct States { bool self[N][N]; } states;
    
        constexpr CmdletConfig(
            std::initializer_list<cmdlet_item_t> ordered_keys,
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
    class __lambda__ CmdletPlane : public virtual Plteen::TheBigBang {
    public:
        CmdletPlane(const cmdlet_item_t ordered_keys[], size_t n, const bool* default_states)
            : CmdletPlane(Plteen::GameFont::monospace(), ordered_keys, n, default_states) {}

        CmdletPlane(const Plteen::shared_font_t& font, const cmdlet_item_t ordered_keys[], size_t n, const bool* default_states);

        virtual ~CmdletPlane() noexcept;

    public:
        template<size_t N>
        CmdletPlane(const CmdletConfig<N>& c)
            : CmdletPlane(c.keys.self, N, reinterpret_cast<const bool*>(c.states.self)) {}

        template<size_t N>
        CmdletPlane(const Plteen::shared_font_t& font, const CmdletConfig<N>& c)
            : CmdletPlane(font, c.keys.self, N, reinterpret_cast<const bool*>(c.states.self)) {}

        template<size_t N>
        CmdletPlane(const cmdlet_item_t (&order_keys)[N]) : CmdletPlane(order_keys, N, nullptr) {}

        template<size_t N>
        CmdletPlane(const Plteen::shared_font_t& font, const cmdlet_item_t (&ordered_keys)[N])
            : CmdletPlane(font, ordered_keys, N, nullptr) {}

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    public:
        void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;
        void on_tap(Plteen::IMatter* m, float local_x, float local_y) override;

    public:
        void set_processing_cmdlet_color(const Plteen::RGBA& color);
        void set_enabled_cmdlet_color(const Plteen::RGBA& color);
        void set_disabled_cmdlet_color(const Plteen::RGBA& color, const Plteen::RGBA& hi_color);
        void set_disabled_cmdlet_color(const Plteen::RGBA& color) { this->set_disabled_cmdlet_color(color, color); }

    protected:
        size_t cmdlet_count() { return this->N; }
        Plteen::Labellet* cmdlet_ref(int idx);
        void cmdlet_job_done();
        bool in_processing_cmdlet() { return (this->working_idx < this->N); }
        char current_cmdlet() { return this->_fkeys[this->working_idx]; }
        const char* current_cmdlet_name() { return this->_fdescs[this->working_idx].c_str(); }
        bool is_cmdlet_key(char key);
        bool trigger_cmdlet(char key);

    protected:
        virtual void reflow_cmdlet(Plteen::Labellet* cmdlet, Plteen::Labellet* prev, float width, float height);
        virtual void on_cmdlet(size_t idx, char key, const std::string& name, float width, float height) {}

    private:
        void initialize_facilities(size_t n);
        void deal_with_cmdlet_at(size_t idx);

    private:
        Plteen::Labellet** facilities = nullptr;
        Plteen::shared_font_t cmdlet_font;
        Plteen::RGBA working_cmdlet_color = GREEN;
        Plteen::RGBA enabled_cmdlet_color = ROYALBLUE;
        Plteen::RGBA disabled_cmdlet_color = GRAY;
        Plteen::RGBA hi_disabled_cmdlet_color = CRIMSON;

    private:
        size_t working_idx;
        char* _fkeys = nullptr;
        bool** _fokay = nullptr;
        std::string* _fdescs = nullptr;
        size_t N = 0;
    };
}
