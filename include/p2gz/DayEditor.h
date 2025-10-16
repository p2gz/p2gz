#ifndef _GZ_DAY_EDITOR_H
#define _GZ_DAY_EDITOR_H

#include <Game/TimeMgr.h>
#include <Game/GameSystem.h>
#include <p2gz/gzmenu.h>

namespace gz {

struct DayEditor {
public:
	void init();
	void update();

	void set_time_paused(bool paused);
	void set_time(f32 time);

private:
	Game::TimeMgr* time_mgr;
	FloatRangeMenuOption* set_time_opt;
};

}; // namespace gz

#endif
