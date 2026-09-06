#ifndef _GZ_SEGMENT_SNAPSHOT_H
#define _GZ_SEGMENT_SNAPSHOT_H

#include <types.h>
#include <p2gz/gzCollections.h>
#include <Vector3.h>

namespace gz {

struct SegmentSnapshot {
	SegmentSnapshot();
	~SegmentSnapshot();

	void capture(bool above_ground);
	void restore_progress();
	void restore_cache();
	void restore_post_load();

private:
	SegmentSnapshot(const SegmentSnapshot&);

	// trick to force this to be non-copyable lol
	SegmentSnapshot& operator=(const SegmentSnapshot&);

	struct BridgeState {
		Vector3f position;
		bool glitched;
	};

	u8* progress;
	int progress_size;
	u8* cache_data;
	int cache_size;
	int course_index;
	u8 active_captain;
	u8 cache_header[36];
	Vec<BridgeState> bridges;
};

} // namespace gz

#endif
