#ifndef _FREECAM_H
#define _FREECAM_H

#include <Game/CameraMgr.h>
#include <Game/Navi.h>

namespace gz {
struct FreeCam {
public:
	FreeCam()
	{
		enabled = false;
		zoom    = 0;
		camera  = nullptr;
		navi    = nullptr;

		animation_coefficient   = 0;
		is_coefficient_positive = true;
	}
	~FreeCam() { }

	bool is_enabled() { return enabled; }

	void disable();
	void enable();
	void switch_captains();
	void update();
	void draw();
	void warp_to_current_position();

private:
	void draw_current_position();
	void update_position();
	void update_zoom();

	Game::PlayCamera* camera;
	Game::Navi* navi;
	bool enabled;
	int zoom;
	f32 animation_coefficient;
	bool is_coefficient_positive;
};

} // namespace gz

#endif
