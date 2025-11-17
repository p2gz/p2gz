#ifndef _GZ_SEGMENT_HISTORY_H
#define _GZ_SEGMENT_HISTORY_H

#include <types.h>
#include <Game/PikiContainer.h>
#include <p2gz/warp.h>

namespace gz {

struct Segment {
public:
	Segment()
	{
		preset = nullptr;
		dest   = WarpDestination();
	}

	~Segment() { delete preset; }

	Preset* preset;
	WarpDestination dest;
};

struct SegmentHistory {
public:
	void draw_2d();
	void update();

	Segment* start_segment();
	Segment* cur_segment();
	void record_squad();

	bool started_creating_map;
	bool entering_next_segment;

private:
	void draw_cur_seed();
	void draw_reset_controls(bool draw_cave_retry);

	RingBuffer<32, Segment*> segments;
};

}; // namespace gz

#endif
