#pragma once

#include "dc.hpp"
#include "font.hpp"

#include <vector>

namespace Plteen {
	struct __lambda__ HHatchMarkMetrics {
		float gap_space;
		float top_space;
		float ch;
		float em;

		float height;
		float hatch_x;
		float hatch_y;
		float hatch_width;
		float hatch_height;
		float hatch_rx;
	};

	struct __lambda__ VHatchMarkMetrics {
		size_t span;
		float mark_width;
		float gap_space;
		float top_space;
		float ch;
		float em;

		float width;
		float hatch_x;
		float hatch_y;
		float hatch_width;
		float hatch_height;
	};
	
	class __lambda__ Ruler {
	public:
		static Plteen::HHatchMarkMetrics hhatchmark_metrics(
					double vmin, double vmax, uint8_t precision = 0U);

		static Plteen::HHatchMarkMetrics hhatchmark_metrics(
					Plteen::shared_font_t font,
					double vmin, double vmax, uint8_t precision = 0U);

		static Plteen::VHatchMarkMetrics vhatchmark_metrics(
					double vmin, double vmax, uint8_t precision = 0U);

		static Plteen::VHatchMarkMetrics vhatchmark_metrics(
					Plteen::shared_font_t font,
					double vmin, double vmax, uint8_t precision = 0U);

	public:
		static void draw_ht_hatchmark(Plteen::dc_t* dc, float x, float y,
					float width, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::HHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_ht_hatchmark(Plteen::shared_font_t font,
					Plteen::dc_t* dc, float x, float y,
					float width, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::HHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_hb_hatchmark(Plteen::dc_t* dc, float x, float y,
					float width, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::HHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);
		
		static void draw_hb_hatchmark(Plteen::shared_font_t font,
					Plteen::dc_t* dc, float x, float y,
					float width, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::HHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_vl_hatchmark(Plteen::dc_t* dc, float x, float y,
					float height, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::VHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_vl_hatchmark(Plteen::shared_font_t font,
					Plteen::dc_t* dc, float x, float y,
					float height, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::VHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_vr_hatchmark(Plteen::dc_t* dc, float x, float y,
					float height, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::VHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);

		static void draw_vr_hatchmark(Plteen::shared_font_t font,
					Plteen::dc_t* dc, float x, float y,
					float height, double vmin, double vmax, uint32_t step, uint32_t color,
					Plteen::VHatchMarkMetrics* metrics = nullptr,
					uint8_t precision = 0U, bool no_short = false);
	};
}
