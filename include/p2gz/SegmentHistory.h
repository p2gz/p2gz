#ifndef _GZ_SEGMENT_HISTORY_H
#define _GZ_SEGMENT_HISTORY_H

#include <types.h>
#include <Game/PikiContainer.h>

namespace gz {

struct Segment {
public:
	u32 seed;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
	u32 area;
	u32 cave;
	u32 sublevel;
	u32 day;
};

struct SegmentHistory {
public:
	void draw_2d();
	void update();

	void start_segment(u32 seed);
	Segment* cur_segment()
	{
		if (segments.len() > 0)
			return segments[segments.len() - 1];
		else
			return nullptr;
	}

	bool started_creating_map;
	bool entering_next_sublevel;

private:
	void draw_cur_seed();
	void draw_reset_controls();

	Vec<Segment*> segments;
};

}; // namespace gz

#endif
