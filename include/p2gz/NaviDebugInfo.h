#ifndef _GZ_NAVI_DEBUG_INFO_H
#define _GZ_NAVI_DEBUG_INFO_H

#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <p2gz/gzCollections.h>
#include <Graphics.h>
#include <types.h>
#include <Vector3.h>
#include <Color4.h>

namespace gz {

enum NaviSwapTimeState {
	SWAP_Inactive,
	SWAP_Swapping,
	SWAP_Complete,
};

struct NaviDebugInfo {
	NaviDebugInfo();

	void update();
	void draw();

	void set_enabled(bool enabled_) { enabled = enabled_; }

	void set_draw_position_enabled(bool enabled_) { draw_position = enabled_; }
	void set_draw_velocity_enabled(bool enabled_) { draw_velocity = enabled_; }
	void set_draw_target_velocity_enabled(bool enabled_) { draw_target_velocity = enabled_; }
	void set_draw_face_dir_enabled(bool enabled_) { draw_face_dir = enabled_; }
	void set_draw_state_enabled(bool enabled_) { draw_state = enabled_; }
	void set_draw_swap_time_enabled(bool enabled_) { draw_swap_time = enabled_; }

	void swap_start(int startNaviID);
	void swap_complete();
	bool is_swapping() { return swap_time_state == SWAP_Swapping; }

private:
	bool enabled;
	bool draw_position;
	bool draw_velocity;
	bool draw_target_velocity;
	bool draw_face_dir;
	bool draw_state;
	JUtility::TColor olimarTextColor;
	JUtility::TColor louieTextColor;

	// captain swap timer info
	bool draw_swap_time;
	NaviSwapTimeState swap_time_state;
	f32 swap_time;
	f32 swap_display_fade_timer;
	int swap_navi_source_ID;
};

}; // namespace gz

#endif
