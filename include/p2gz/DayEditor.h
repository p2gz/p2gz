#ifndef _GZ_DAY_EDITOR_H
#define _GZ_DAY_EDITOR_H

#include <Game/TimeMgr.h>
#include <Game/GameSystem.h>

namespace gz {

struct DayEditor {
public:
	void set_time_paused(bool paused);
	void jump_forward();
	void jump_backward();

private:
	Game::TimeMgr* get_time_mgr() { return Game::gameSystem->mTimeMgr; }
};

}; // namespace gz

#endif
