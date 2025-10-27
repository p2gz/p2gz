#ifndef _GZ_WARP_H
#define _GZ_WARP_H

#include <types.h>
#include <p2gz/Preset.h>
#include <Game/SingleGameSection.h>

namespace gz {

struct WarpDestination {
public:
	WarpDestination() { }
	~WarpDestination() { }

	u32 area;
	u32 cave;
	u32 sublevel;
	u32 day;
	u32 seed;
	bool use_set_seed;

	// whether to do the falling animation or the ship fly-in animation when warping to AG
	size_t enter_area_type;
};

struct Warp {
public:
	Warp();
	~Warp() { }

	void init();

	static WarpDestination current_dest();
	void set_dest(WarpDestination dest_) { dest = dest_; }

	void set_warp_area(size_t area);
	void set_warp_cave(size_t cave);
	void set_warp_sublevel(s32 sublevel);
	void set_warp_day(s32 day) { dest.day = day - 1; }
	void set_allow_zero_piki_in_caves(bool allow) { allow_zero_pikmin_in_caves = allow; }
	void set_enter_area_type(size_t type) { dest.enter_area_type = type == 1; }
	void set_seed(u32);
	void set_random_seed() { dest.use_set_seed = false; }
	void set_preset(Preset* preset);

	bool using_set_seed() { return dest.use_set_seed; }
	u32 get_seed() { return dest.seed; }
	Preset* get_current_preset();

	void do_warp();

	bool allow_zero_pikmin_in_caves;

private:
	void update_cave_opt();
	void update_sublevel_opt();
	void update_preset_opt();
	void warp_to_cave(Game::SingleGameSection* game);
	void warp_to_area(Game::SingleGameSection* game);
	void save_pikmin();
	void reset_cave_treasure_collections(Game::SingleGameSection* game);

	WarpDestination dest;

	RadioMenuOption* area_opt;
	RangeMenuOption* sublevel_opt;
	RadioMenuOption* cave_opt;
	RangeMenuOption* day_opt;
	RadioMenuOption* enter_area_type_opt;
	HexInputOption* seed_opt;
	PresetMenuOption* preset_opt;
};

}; // namespace gz

#endif
