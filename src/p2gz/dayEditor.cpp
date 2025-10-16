#include <p2gz/p2gz.h>
#include <p2gz/DayEditor.h>
#include <Game/TimeMgr.h>
#include <Game/GameSystem.h>

using namespace gz;

void DayEditor::init()
{
	time_mgr     = Game::gameSystem->mTimeMgr;
	set_time_opt = static_cast<FloatRangeMenuOption*>(p2gz->menu->get_option("tools/time controls/time"));
}

void DayEditor::update()
{
	if (set_time_opt && time_mgr) {
		set_time_opt->set_selection(time_mgr->mCurrentTimeOfDay);
	}
}

void DayEditor::set_time_paused(bool paused)
{
	if (!time_mgr) {
		return;
	}

	if (paused) {
		time_mgr->setFlag(Game::TIMEFLAG_Stopped);
	} else {
		time_mgr->resetFlag(Game::TIMEFLAG_Stopped);
	}
}

void DayEditor::set_time(f32 time)
{
	if (time_mgr) {
		time_mgr->setTime(time);
	}
}
