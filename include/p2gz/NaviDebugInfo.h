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

struct NaviDebugInfo {
	NaviDebugInfo();

	void draw();

	void set_enabled(bool enabled_) { enabled = enabled_; }

	void set_draw_position_enabled(bool enabled_) { draw_position = enabled_; }
	void set_draw_velocity_enabled(bool enabled_) { draw_velocity = enabled_; }
	void set_draw_target_velocity_enabled(bool enabled_) { draw_target_velocity = enabled_; }
	void set_draw_state_enabled(bool enabled_) { draw_state = enabled_; }

private:
	bool enabled;
	bool draw_position;
	bool draw_velocity;
	bool draw_target_velocity;
	bool draw_state;
	JUtility::TColor olimarTextColor;
	JUtility::TColor louieTextColor;
};

}; // namespace gz

#endif
