#pragma once

#include "graphics/dc.hpp"

#include <cstdint>
#include <string>

#include "forward.hpp"
#include "physics/motion.hpp"
#include "physics/color/names.hpp"
#include "physics/algebra/point.hpp"
#include "physics/geometry/port.hpp"
#include "physics/geometry/aabox.hpp"
#include "physics/geometry/margin.hpp"

namespace Plteen {
    class __lambda__ IMatterInfo {
    public:
        virtual ~IMatterInfo() {};
        IMatterInfo(IPlane* master) : master(master) {};
    
    public:
        IPlane* master;
    };

    class __lambda__ IMatterMetadata {};

    class __lambda__ IMatter : public Plteen::IMovable {
    public:
        IMatter() {}
        virtual ~IMatter();

    public:
        Plteen::IPlane* master() const;
        Plteen::dc_t* drawing_context() const;

    public:
        void attach_metadata(IMatterMetadata* metadata);
        IMatterMetadata* metadata() const { return this->_metatdata; }

        template<typename MD>
        MD* unsafe_metadata() const { return static_cast<MD*>(this->metadata()); }

    public:
        virtual void construct(Plteen::dc_t* renderer) {}
        virtual Plteen::Box get_bounding_box() { return Plteen::Box(/* keep it invalid */); }
        virtual Plteen::Box get_original_bounding_box() { return this->get_bounding_box(); }
        virtual Plteen::Margin get_margin() { return this->get_original_margin(); }
        virtual Plteen::Margin get_original_margin() { return Plteen::Margin(); }
        virtual int update(uint64_t count, uint32_t interval, uint64_t uptime) { return 0; }
        virtual void draw(Plteen::dc_t* renderer, float x, float y, float Width, float Height) = 0;
        virtual void draw_in_progress(Plteen::dc_t* renderer, float x, float y, float Width, float Height) {}
        virtual bool ready() { return true; }

    public:
        virtual void own_caret(bool is_own) {}
        virtual int preferred_local_fps() { return 0; }
        virtual const char* name();
        
    public:
        virtual bool is_colliding(const Plteen::Dot& local_pt);

    public:
        virtual void on_location_changed(float x, float y, float old_x, float old_y) {}
        virtual bool on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) { return false; }
        virtual bool on_text(const char* text, size_t size, bool entire) { return false; }
        virtual bool on_editing_text(const char* text, int pos, int span) { return false; }
        virtual bool on_hover(float local_x, float local_y) { return false; }
        virtual bool on_tap(float local_x, float local_y) { return false; }
        virtual bool on_double_tap(float local_x, float local_y) { return false; }
        virtual bool on_goodbye(float local_x, float local_y) { return false; }

    public: // low-level events
        virtual bool on_pointer_pressed(uint8_t button, float local_x, float local_y, uint8_t clicks) { return false; }
        virtual bool on_pointer_move(uint32_t state, float local_x, float local_y, float dx, float dy, bool bye) { return false; }
        virtual bool on_pointer_released(uint8_t button, float local_x, float local_y, uint8_t clicks) { return false; }

    public:
        bool resizable() { return this->can_resize; }
        void scale(float ratio, const Plteen::Port& port = 0.5F) { this->scale(ratio, ratio, port); }
        void scale(float x_ratio, float y_ratio, const Plteen::Port& port = 0.5F);
        void scale_to(float ratio, const Plteen::Port& port = 0.5F) { this->scale_to(ratio, ratio, port); }
        void scale_to(float x_ratio, float y_ratio, const Plteen::Port& port = 0.5F);
        void resize(float size, const Plteen::Port& port = 0.5F) { this->resize(size, size, port); }
        void resize(float width, float height, const Plteen::Port& port = 0.5F);
        void resize_by_width(float size, const Plteen::Port& port = 0.5F) { this->scale_by_size(size, true, port); }
        void resize_by_height(float size, const Plteen::Port& port = 0.5F) { this->scale_by_size(size, false, port); }

    public:
        bool events_allowed() { return this->deal_with_events; }
        bool low_level_events_allowed() { return (this->events_allowed() && this->deal_with_low_level_events); }
    
    public:
        bool has_caret();
        void moor(const Plteen::Port& port);
        void clear_moor(); /* the notify_updated() will clear the moor,
                              but the notification is not always guaranteed to be done,
                              use this method to do it manually. */

    public:
        void camouflage(bool yes = true) { this->findable = !yes; }
        bool concealled() { return !this->findable; }
        void show(bool yes = true);
        bool visible() { return !this->invisible; }

    public:
        void notify_updated();
        void notify_timeline_restart(uint32_t count0 = 0, int duration = 0);
        Plteen::Dot get_location(const Plteen::Port& p = 0.0F);
        void log_message(Plteen::Log level, const std::string& msg);
        
    public:
        IMatterInfo* info = nullptr;    

    protected:
        void enable_events(bool yes = true, bool low_level = false) { this->deal_with_events = yes; this->deal_with_low_level_events = low_level; }
        void enable_resize(bool yes = true) { this->can_resize = yes; }
        virtual void on_resize(float width, float height, float old_width, float old_height) {}

    private:
        void scale_by_size(float size, bool given_width, const Plteen::Port& port);

    private:
        bool findable = true;
        bool invisible = false;
        bool can_resize = false;
        bool deal_with_events = false;
        bool deal_with_low_level_events = false;
        // bool wheel_translation = true;
    
    private:
        Plteen::Port port;
        Plteen::Dot port_dot;

    private:
        Plteen::IMatterMetadata* _metatdata = nullptr;
    };
}
