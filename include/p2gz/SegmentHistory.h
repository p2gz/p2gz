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
		preset       = nullptr;
		seed         = 0;
		dest         = WarpDestination();
		use_set_seed = false;
	}

	~Segment()
	{
		if (preset) {
			preset->del();
		}
		preset = nullptr;
	}

	Preset* preset;
	WarpDestination dest;
	u32 seed;
	bool use_set_seed;
};

struct SegmentHistory {
public:
	SegmentHistory()
	{
		started_creating_map    = false;
		entering_next_segment   = false;
		capturing_segment_cache = false;
		cave_floor0_preset      = nullptr;
		current_segment         = nullptr;
	}

	void draw_2d();
	void update();

	Segment* start_segment();
	Segment* cur_segment();
	void capture_segment();

	void retry_segment();
	void retry_same_seed();
	void retry_cave();

	bool started_creating_map;
	bool entering_next_segment;
	bool capturing_segment_cache;

private:
	void draw_cur_seed();
	void draw_reset_controls();

	Segment* current_segment;
	Preset* cave_floor0_preset;       // pinned until we leave the cave
	WarpDestination cave_floor0_dest; // area+cave the pinned preset belongs to
};

}; // namespace gz

#endif
