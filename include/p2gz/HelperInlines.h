#ifndef _GZ_HELPER_INLINES_H
#define _GZ_HELPER_INLINES_H

#include <Game/GameSystem.h>
#include <Game/SingleGame.h>
#include <GameFlow.h>

namespace gz {

inline bool inBootUp()
{
	// make sure gameflow is in boot up state
	return GameFlow::mActiveSectionFlag == GameFlow::SN_Boot;
}

inline bool inTitleScreen()
{
	// make sure gameflow is title state (incl. options menus, hiscore menus, etc)
	return GameFlow::mActiveSectionFlag == GameFlow::SN_MainTitle;
}

inline bool inStoryMode()
{
	// make sure game system is active and we're not in challenge mode/vs mode/piklopedia/etc
	return Game::gameSystem && Game::gameSystem->isStoryMode();
}

inline Game::SingleGameSection* getSGS()
{
	// make sure current section is castable to SingleGameSection
	if (!inStoryMode()) {
		return nullptr;
	}
	return static_cast<Game::SingleGameSection*>(Game::gameSystem->getSection());
}

inline bool inFileSelect()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inWorldMap()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inAboveGroundPlay()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inCavePlay()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inEndOfDay()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inCaveResults()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inLoad()
{
	// must be in single player mode
	Game::SingleGameSection* sgs = getSGS();
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

inline bool inEnterCaveLoad()
{
	// must be in a single player load state
	if (!inLoad()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(getSGS()->getCurrState());
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

inline bool inBetweenSublevelLoad()
{
	// must be in single player mode
	if (!inLoad()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(getSGS()->getCurrState());
	if (!state) {
		return false;
	}
	// must be in a load that increments sublevel count
	if (!state->mIsCaveDeeper) {
		return false;
	}
	return true;
}

inline bool inExitCaveLoad()
{
	// must be in single player mode
	if (!inLoad()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(getSGS()->getCurrState());
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

inline bool inEnterAreaLoad()
{
	// must be in single player mode
	if (!inLoad()) {
		return false;
	}
	Game::SingleGame::LoadState* state = static_cast<Game::SingleGame::LoadState*>(getSGS()->getCurrState());
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
