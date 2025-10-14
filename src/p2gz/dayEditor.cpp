#include <p2gz/DayEditor.h>
#include <Game/TimeMgr.h>
#include <Game/GameSystem.h>

using namespace gz;

void DayEditor::set_time_paused(bool paused)
{
	if (paused) {
		get_time_mgr()->setFlag(Game::TIMEFLAG_Stopped);
	} else {
		get_time_mgr()->resetFlag(Game::TIMEFLAG_Stopped);
	}
}

void DayEditor::jump_forward()
{
	Game::TimeMgr* time_mgr = get_time_mgr();
	time_mgr->setTime(time_mgr->mCurrentTimeOfDay + 1.0);
}

void DayEditor::jump_backward()
{
	Game::TimeMgr* time_mgr = get_time_mgr();
	time_mgr->setTime(time_mgr->mCurrentTimeOfDay - 1.0);
}
