#ifndef _GZ_SEGMENT_HISTORY_H
#define _GZ_SEGMENT_HISTORY_H

#include <types.h>

namespace gz {

struct SegmentHistory {
public:
	void draw_2d();
	void draw_cur_seed();

	bool started_creating_map;
	u32 last_seed;
};

}; // namespace gz

#endif
