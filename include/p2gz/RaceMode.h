#ifndef _GZ_RACE_MODE_H
#define _GZ_RACE_MODE_H

#include <types.h>
#include <p2gz/Preset.h> // PresetCategory
#include <JSystem/JUtility/TColor.h>

namespace gz {

// forward declarations
struct ListMenu;
struct RadioMenuOption;
struct HexInputOption;

/// "Race / Run" mode: start a genuine fresh file for a PoD or AT speedrun, optionally on a fixed
/// seed applied to every floor, with a precise dual timer (RTA + load-removed IGT), an end-of-run
/// stats overlay, an in-run floor reset (replacing soft reset), and the P2GZ menu locked out.
struct RaceMode {
public:
	RaceMode();
	~RaceMode() { }

	void init();
	void update();
	void draw_2d();

	// queried from game-code hooks
	bool is_active() { return active; }
	bool using_set_seed() { return use_set_seed; }
	u32 get_seed() { return seed; }
	PresetCategory get_category() { return category; }

	// menu callbacks
	void set_category(size_t idx);
	void set_seed(u32 seed_);
	void set_random_seed();
	void start_run();

	// hooks called from game code
	void on_ending(bool is_complete); // run-end detection (RDS_GoToPayDebt / RDS_GameComplete)
	void request_reset();             // X+B+Start floor retry, deferred from reset.cpp
	void notify_enemy_defeated();     // stats counter
	void notify_pikmin_thrown();      // stats counter

private:
	void begin_fresh_file();
	void do_reset();
	void abort_run();
	void capture_stats();

	void draw_timers();
	void draw_summary();
	void draw_controls_hint();

	static u32 cur_ms();
	static void ms_to_clock(u32 ms, u32& minutes, u32& seconds, u32& tenths);

	// internal reset-penalty constants (NOT menu-exposed); both 0 for now, tunable later
	static const f32 RESET_PENALTY_SET_SEED; // applied per reset in fixed-seed runs
	static const f32 RESET_PENALTY_RANDOM;   // applied per reset in vanilla-random runs

	// abort combo: hold L+R+Z+Start
	static const u32 ABORT_COMBO_MASK;

	bool active;            // run in progress (gates menu + drives hooks)
	bool finished;          // run ended; overlay + timers frozen
	bool at_fresh_start;    // on the day-1 crash-landing segment (reset must re-run the new file, not warp)
	PresetCategory category; // PoD or AT
	bool use_set_seed;       // fixed-seed vs vanilla-random
	u32 seed;                // the fixed seed (when use_set_seed)

	u32 reset_count;
	u32 load_ms;            // accumulated loading time (for IGT)
	bool was_loading;       // edge-detect in_load() for load_ms accounting
	u32 load_enter_ms;      // wall-clock ms when the current load began
	u32 abort_hold_frames;  // frames the abort combo has been held

	bool pending_reset;     // set by request_reset(), consumed in update()

	u32 final_rta_ms;       // frozen at run end
	u32 final_igt_ms;
	u32 final_pikmin_lost;  // frozen at run end (DeathMgr total + today)
	bool final_is_complete; // which ending we stopped on (overlay label)

	u32 enemies_defeated;
	u32 pikmin_thrown;

	// menu hooks
	RadioMenuOption* category_opt;
	RadioMenuOption* region_opt;
};

}; // namespace gz

#endif
