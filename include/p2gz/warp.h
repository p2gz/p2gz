#ifndef _GZ_WARP_H
#define _GZ_WARP_H

#include <types.h>
#include <p2gz/Preset.h>
#include <Game/SingleGameSection.h>
#include <Game/Entities/ItemCave.h>

namespace gz {

struct WarpDestination {
public:
	WarpDestination()
	{
		area         = 0;
		cave         = 0;
		sublevel     = 0;
		day          = 2;
		use_set_seed = false;
	}

	~WarpDestination() { }

	u8 area;
	u8 cave;
	u8 sublevel;
	u8 day;
	u32 seed;
	bool use_set_seed;

	// whether to do the falling animation or the ship fly-in animation when warping to AG
	size_t enter_area_type;
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

	static WarpDestination current_dest();
	void set_dest(WarpDestination new_dest);

	void set_warp_area(size_t area);
	void set_warp_cave(size_t cave);
	void set_warp_sublevel(s32 sublevel);
	void set_warp_day(s32 day) { dest.day = day - 1; }
	void set_allow_zero_piki_in_caves(bool allow) { allow_zero_pikmin_in_caves = allow; }
	void set_enter_area_type(size_t type) { dest.enter_area_type = type == 1; }
	void set_seed(u32);
	void set_random_seed() { dest.use_set_seed = false; }

	void set_preset(Preset* preset, int preset_status);
	Preset* get_preset() { return preset; }

	bool using_set_seed() { return dest.use_set_seed; }
	u32 get_seed() { return dest.seed; }

	void do_warp();
	void post_warp();

	bool allow_zero_pikmin_in_caves;
	bool warping_from_menu;

private:
	void update_cave_opt();
	void update_sublevel_opt();
	void update_preset_opt();
	void warp_to_cave(Game::SingleGameSection* game);
	void warp_to_area(Game::SingleGameSection* game);
	void save_pikmin();
	void reset_cave_treasure_collections(Game::SingleGameSection* game);

	Preset* preset;
	PresetStatus preset_status;
	WarpDestination dest;
	bool needs_post_warp;

	RadioMenuOption* area_opt;
	RangeMenuOption* sublevel_opt;
	RadioMenuOption* cave_opt;
	RangeMenuOption* day_opt;
	RadioMenuOption* enter_area_type_opt;
	HexInputOption* seed_opt;
	PresetMenuOption* preset_opt;

	Game::ItemCave::Item* cave;
};

}; // namespace gz

#endif
