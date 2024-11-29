#pragma once

#include "../screen.hpp"
#include "../../forward.hpp"

namespace Plteen {
	class __lambda__ Pasteboard : public Plteen::IScreen {
	public:
		Pasteboard(Plteen::IMatter* master) : matter(master) {}
	
    public:
        Plteen::IDisplay* display() override;
    
    public:
        void refresh() override;
        void feed_extent(float* width, float* height) override;
        void feed_client_extent(float* width, float* height) override;
        uint32_t frame_rate() override;
        void toggle_window_fullscreen() override { /* disabled for subwindows */ }
    
    public:
        void begin_update_sequence() override;
        bool is_in_update_sequence() override;
        void end_update_sequence() override;
        bool should_update() override;
        void notify_updated() override;

    public:
        void log_message(Plteen::Log level, const std::string& message) override;
        void start_input_text(const std::string& prompt) override;

    private:
        Plteen::IMatter* matter;
    };
}
