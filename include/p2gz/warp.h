#ifndef _GZ_WARP_H
#define _GZ_WARP_H

#include <types.h>

namespace gz {

struct Warp {
public:
	Warp();
	~Warp() { }

	void init();

	void set_warp_area(size_t area);
	void set_warp_cave(size_t cave);
	void set_warp_sublevel(s32 sublevel);

private:
	void update_cave_opt();
	void update_sublevel_opt();

	u32 warp_area;
	u32 warp_cave;
	u32 warp_sublevel;
};

}; // namespace gz

#endif
