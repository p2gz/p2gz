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
		use_set_seed = false;
	}

	~Segment()
	{
		// We cannot delete preset even if it's generated because it's now
		// potentially shared between more than one segment due to retries.
		// TODO: can we store generated presets with a ref count or something
		// so we can free them eventually?
	}

	Preset* preset;
	WarpDestination dest;
	u32 seed;
	bool use_set_seed;
};

struct SegmentHistory {
public:
	void draw_2d();
	void update();

	Segment* start_segment();
	Segment* cur_segment();
	void record_squad();

	void retry_segment();
	void retry_same_seed();
	void retry_cave();

	bool started_creating_map;
	bool entering_next_segment;

private:
	void draw_cur_seed();
	void draw_reset_controls(bool draw_cave_retry);

	RingBuffer<32, Segment*> segments;
};

}; // namespace gz

#endif
