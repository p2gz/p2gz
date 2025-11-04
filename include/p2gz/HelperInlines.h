#ifndef _GZ_HELPER_INLINES_H
#define _GZ_HELPER_INLINES_H

#include <Game/GameSystem.h>
#include <Game/SingleGame.h>
#include <GameFlow.h>

namespace gz {

inline bool is_30_fps()
{
	return sys && (sys->mFrameRate == 2.0f);
}

inline bool in_boot_up()
{
	// make sure gameflow is in boot up state
	return GameFlow::mActiveSectionFlag == GameFlow::SN_Boot;
}

inline bool in_title_screen()
{
	// make sure gameflow is title state (incl. options menus, hiscore menus, etc)
	return GameFlow::mActiveSectionFlag == GameFlow::SN_MainTitle;
}

inline bool in_story_mode()
{
	// make sure game system is active and we're not in challenge mode/vs mode/piklopedia/etc
	return Game::gameSystem && Game::gameSystem->isStoryMode();
}

inline Game::SingleGameSection* get_SGS()
{
	// make sure current section is castable to SingleGameSection
	if (!in_story_mode()) {
		return nullptr;
	}
	return static_cast<Game::SingleGameSection*>(Game::gameSystem->getSection());
}

inline bool in_file_select()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is the choose a file OR no memory card found screen
	if (state->getCurrStateID() != Game::SingleGame::SGS_File) {
		return false;
	}

	return true;
}

inline bool in_world_map()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is the select an area/world map screen
	if (state->getCurrStateID() != Game::SingleGame::SGS_Select) {
		return false;
	}
	return true;
}

inline bool in_above_ground_play()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is regular above-ground gameplay
	if (state->getCurrStateID() != Game::SingleGame::SGS_Game) {
		return false;
	}
	return true;
}

inline bool in_cave_play()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is regular in-cave gameplay
	if (state->getCurrStateID() != Game::SingleGame::SGS_Cave) {
		return false;
	}
	return true;
}

inline bool in_day_end_sunset()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is day ending cutscene (returning to onyons + ship)
	if (state->getCurrStateID() != Game::SingleGame::SGS_DayEnd) {
		return false;
	}
	return true;
}

inline bool in_end_of_day_result()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is end of day results
	if (state->getCurrStateID() != Game::SingleGame::SGS_MainResult) {
		return false;
	}
	return true;
}

// Similar to above, but used for warping - must not be loading any BLO stuff, or else we get weird crashes
inline bool in_end_of_day_result_safe_to_warp()
{
	// must be in day end results (duh)
	if (!in_end_of_day_result()) {
		return false;
	}

	Game::SingleGameSection* sgs                  = get_SGS();
	Game::SingleGame::State* state                = sgs->getCurrState();
	Game::SingleGame::MainResultState* resultCast = static_cast<Game::SingleGame::MainResultState*>(state);

	// State 3 is the regular state; states 0 relate to loading BLOs and prepartions stuff, not safe
	if (resultCast->mStatus != Game::SingleGame::MainResultState::Result_ScreenActive
	    && resultCast->mStatus != Game::SingleGame::MainResultState::Result_OpenWait) {
		return false;
	}
	return true;
}

inline bool in_cave_results()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is cave results
	if (state->getCurrStateID() != Game::SingleGame::SGS_CaveResult) {
		return false;
	}
	return true;
}

// Similar to above, but used for warping - must not be loading any BLO stuff, or else we get lockout crash
inline bool in_cave_results_safe_to_warp()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is cave results
	if (state->getCurrStateID() != Game::SingleGame::SGS_CaveResult) {
		return false;
	}
	Game::SingleGame::CaveResultState* caveCast = static_cast<Game::SingleGame::CaveResultState*>(state);
	// State 3 is the regular state; states 0 and 1 related to loading BLOs and prepartions stuff, not safe
	if (caveCast->mStatus != 3) {
		return false;
	}
	return true;
}

inline bool in_load()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs) {
		return false;
	}
	Game::SingleGame::State* state = sgs->getCurrState();
	if (!state) {
		return false;
	}
	// check we're in the correct state - this is any loading state between other states
	if (state->getCurrStateID() != Game::SingleGame::SGS_Load) {
		return false;
	}
	return true;
}

inline bool in_enter_cave_load()
{
	// must be in a single player load state
	if (!in_load()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(get_SGS()->getCurrState());
	if (!state) {
		return false;
	}
	// must be in a cave load
	if (!state->mIsCaveLoad) {
		return false;
	}
	// must NOT be in a load that increments sublevel count
	if (state->mIsCaveDeeper) {
		return false;
	}
	return true;
}

inline bool in_between_sublevel_load()
{
	// must be in single player mode
	if (!in_load()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(get_SGS()->getCurrState());
	if (!state) {
		return false;
	}
	// must be in a load that increments sublevel count
	if (!state->mIsCaveDeeper) {
		return false;
	}
	return true;
}

inline bool in_exit_cave_load()
{
	// must be in single player mode
	if (!in_load()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(get_SGS()->getCurrState());
	if (!state) {
		return false;
	}
	// must NOT be in either kind of cave load
	if (state->mIsCaveLoad || state->mIsCaveDeeper) {
		return false;
	}
	// must be exiting "via geyser" (always this to exit a cave to AG)
	if (state->mGameLoadType != Game::SingleGame::MapEnter_CaveGeyser) {
		return false;
	}
	return true;
}

inline bool in_enter_area_load()
{
	// must be in single player mode
	if (!in_load()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(get_SGS()->getCurrState());
	if (!state) {
		return false;
	}
	// must NOT be in either kind of cave load
	if (state->mIsCaveLoad || state->mIsCaveDeeper) {
		return false;
	}
	// must be entering area from world map or file select
	if (state->mGameLoadType != Game::SingleGame::MapEnter_NewDay && state->mGameLoadType != Game::SingleGame::MapEnter_NewGame) {
		return false;
	}
	return true;
}

} // namespace gz

#endif
