#ifndef _GZ_WARP_H
#define _GZ_WARP_H

#include <types.h>
#include <Game/SingleGameSection.h>

namespace gz {

struct Warp {
public:
	Warp();
	~Warp() { }

	void init();

	void set_warp_area(size_t area);
	void set_warp_cave(size_t cave);
	void set_warp_sublevel(s32 sublevel);
	void set_warp_day(s32 day) { warp_day = day - 1; }

	void do_warp();

private:
	void update_cave_opt();
	void update_sublevel_opt();
	void warp_to_cave(Game::SingleGameSection* game);
	void warp_to_area(Game::SingleGameSection* game);

	u32 warp_area;
	u32 warp_cave;
	u32 warp_sublevel;
	u32 warp_day;
};

}; // namespace gz

#endif
