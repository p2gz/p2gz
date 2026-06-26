#ifndef _GZ_RACE_MODE_H
#define _GZ_RACE_MODE_H

#include <types.h>
#include <p2gz/Preset.h>
#include <JSystem/JUtility/TColor.h>

// forward declaration (global namespace, from JSystem)
struct J2DPrint;

namespace gz {

// forward declarations
struct ListMenu;
struct RadioMenuOption;
struct HexInputOption;

// race/speedrun mode - start a fresh file for a PoD or AT speedrun with some extra functionality
struct RaceMode {
public:
	RaceMode();
	~RaceMode() { }

	void init();
	void update();
	void draw_2d();
	void draw_on_top();

	bool is_active() { return active; }
	bool using_set_seed() { return use_set_seed; }
	u32 get_seed() { return seed; }
	PresetCategory get_category() { return category; }

	// where a run begins
	enum StartPoint {
		SP_FreshFile   = 0, // day-1 crash landing (full run)
		SP_PresetStart = 1, // common "race file" start point (SH1 for PoD, WW for AT)
	};

	// menu callbacks
	void set_category(size_t idx);
	void set_start_point(size_t idx);
	void set_seed(u32 seed_);
	void set_random_seed();
	void start_run();

	// hooks called from game code
	void on_ending(bool is_all_treasures);        // run-end detection
	void request_reset();                         // X+B+Start = floor retry override
	void notify_cutscene_skipped(u32 skipped_ms); // advance the run clock by skipped vanilla cutscene time
	void notify_save_skipped(f32 offset_seconds); // advance the run clock by a skipped save-prompt's vanilla time
	void notify_enemy_defeated();                 // fun stats counter
	void notify_pikmin_thrown();                  // fun stats counter

private:
	void begin_fresh_file();
	void begin_from_preset();            // warp to the category's "race file" start point (SP_PresetStart)
	void populate_start_point_options(); // (re)label the start-point radio for the current category
	void do_reset();
	void abort_run();
	void capture_stats();

	u32 rta_ms();
	void add_skipped_time(u32 ms); // advance the run clock by p2gz-skipped vanilla time (cutscenes, save prompts)

	void draw_timers();
	void draw_timer_line(J2DPrint& j2d, f32 decimal_x, f32 z, const char* label, u32 ms); // one decimal-aligned RTA/IGT row
	void draw_summary();
	void draw_controls_hint();
	void draw_menu_controls(); // right-side reset/abort combo overlay shown while the race submenu is open

	static u32 cur_ms();
	static void ms_to_clock(u32 ms, u32& hours, u32& minutes, u32& seconds, u32& tenths);

	// internal reset-penalty constants (for tuning later if desired)
	static const f32 RESET_PENALTY_SET_SEED; // applied per reset in set-seed runs
	static const f32 RESET_PENALTY_RANDOM;   // applied per reset in vanilla runs

	// abort/close race mode combo: hold L+R+Z+Start
	static const u32 ABORT_COMBO_MASK;

	bool active;             // run in progress?
	bool finished;           // run ended?
	bool at_fresh_start;     // on the day-1 crash-landing segment (reset = redo fresh file, not retry segment)
	PresetCategory category; // PoD or AT
	size_t start_point;      // SP_FreshFile or SP_PresetStart
	bool use_set_seed;       // set seed vs vanilla
	u32 seed;                // the fixed seed (when use_set_seed)

	u32 reset_count;
	u32 load_ms;           // accumulated loading time (for IGT)
	bool was_loading;      // edge-detect in_load() for load_ms accounting
	u32 load_enter_ms;     // wall-clock ms when the current load began
	u32 abort_hold_frames; // frames the abort combo has been held

	u32 run_start_ms; // wall-clock ms when the run started
	bool timer_started;

	bool pending_reset; // set by request_reset(), consumed in update()

	u32 final_rta_ms;            // frozen at run end
	u32 final_igt_ms;            // frozen at run end
	u32 final_pikmin_lost;       // frozen at run end (DeathMgr total + today)
	bool final_is_all_treasures; // which ending we stopped on (overlay label)

	u32 enemies_defeated;
	u32 pikmin_thrown;

	// menu hooks
	RadioMenuOption* category_opt;
	RadioMenuOption* start_point_opt;
	RadioMenuOption* region_opt;
};

}; // namespace gz

#endif
