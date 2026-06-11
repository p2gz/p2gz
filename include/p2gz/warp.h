#ifndef _GZ_WARP_H
#define _GZ_WARP_H

#include <types.h>
#include <p2gz/Preset.h>
#include <Game/SingleGameSection.h>

namespace gz {

struct Preset;
struct PresetPreview;
struct PresetMenuOption;

struct WarpDestination {
public:
	WarpDestination()
	{
		area            = 0;
		cave            = 0;
		sublevel        = 0;
		day             = 2;
		enter_area_type = 0;
	}

	u8 area;
	u8 cave;
	u8 sublevel;
	u8 day;

	// whether to do the falling animation or the ship fly-in animation when warping to AG
	u8 enter_area_type;
};

typedef enum PresetStatus {
	PS_Stale     = 0,
	PS_Generated = 1,
	PS_Suggested = 2,
	PS_Chosen    = 3,
} PresetStatus;

struct Warp {
public:
	Warp();
	~Warp() { }

	void init();
	void sync();

	void set_dest(WarpDestination new_dest);

	void set_warp_area(size_t area);
	void set_warp_cave(size_t cave);
	void set_warp_sublevel(s32 sublevel);
	void set_warp_day(s32 day) { dest.day = day - 1; }
	void set_allow_zero_piki_in_caves(bool allow) { allow_zero_pikmin_in_caves = allow; }
	void set_enter_area_type(size_t type) { dest.enter_area_type = type; }
	void set_active_captain(size_t captain) { active_captain = captain; }

	void set_seed(u32);
	void set_random_seed();
	bool using_set_seed() { return use_set_seed; }
	u32 get_seed() { return seed; }

	void set_preset(PresetPreview* preset, int preset_status);
	void set_preset(Preset* preset, int preset_status);
	Preset* get_preset_during_warp()
	{
		GZASSERTLINE(warping);
		return preset_during_warp;
	}

	void do_warp();
	void do_post_warp();

	bool is_warp_lockout() { return lockout_frames > 0; }
	void set_lockout_frames(u8 frames) { lockout_frames = frames; }
	void update_lockout_frames()
	{
		if (lockout_frames > 0) {
			lockout_frames--;
		}
	}

	bool allow_zero_pikmin_in_caves;
	bool warping_from_menu;
	bool warping;
	bool already_saved_generators;
	u8 active_captain;

private:
	void update_cave_opt();
	void update_sublevel_opt();
	void update_preset_opt();
	void update_day_opt();
	void update_enter_type_opt();
	void update_captain_opt();
	void warp_to_cave(Game::SingleGameSection* game);
	void warp_to_area(Game::SingleGameSection* game);
	void save_pikmin();
	void reset_cave_treasure_collections(Game::SingleGameSection* game);

	bool has_next_preset() { return next_preset_p != nullptr || next_preset != nullptr; }
	int next_preset_category();

	// only one of these two will be active at a time
	PresetPreview* next_preset_p;
	Preset* next_preset;

	Preset* preset_during_warp;
	PresetStatus preset_status;
	WarpDestination dest;

	RadioMenuOption* area_opt;
	RangeMenuOption* sublevel_opt;
	RadioMenuOption* cave_opt;
	RangeMenuOption* day_opt;
	RadioMenuOption* captain_opt;
	RadioMenuOption* enter_area_type_opt;
	HexInputOption* seed_opt;
	PresetMenuOption* preset_opt;

	u32 seed;
	bool use_set_seed;

	bool needs_post_load_action;

	// some instances (such as caveresult and day end result) crash when you warp within the 1st frame of everything
	// being loaded, so this prevents that by adding some frames where warp can't happen
	u8 lockout_frames;
};

}; // namespace gz

#endif
