#include "types.h"
#include "nans.h"
#include "TParticle2dMgr.h"
#include "Game/gameStat.h"
#include "Game/SingleGame.h"
#include "Game/MoviePlayer.h"
#include "Screen/Game2DMgr.h"
#include "Game/GameSystem.h"
#include "Radar.h"
#include <p2gz/p2gz.h>

namespace Game {
namespace SingleGame {

/**
 * @note Address: N/A
 * @note Size: 0xE4
 */
// void _Print(char*, ...)
//{
// UNUSED FUNCTION
//}

/**
 * @note Address: 0x80239E84
 * @note Size: 0xB4
 */
void LoadState::init(SingleGameSection* game, StateArg* arg)
{
	moviePlayer->reset();
	Screen::gGame2DMgr->mScreenMgr->reset();

	if (Radar::mgr) {
		Radar::mgr->clear();
	}

	P2ASSERTLINE(33, arg != nullptr);

	LoadArg* a     = static_cast<LoadArg*>(arg);
	mGameLoadType  = a->mGameLoadType;
	mDontClearHeap = a->mDontClearHeap;
	mIsCaveLoad    = a->mInCave;
	mIsCaveDeeper  = a->mIsCaveDeeper;

	mIsInitialized = false;
	mHasDrawn      = false;

	// @P2GZ - timer
	// logic for when to reset a timer on load

	switch (mGameLoadType) {
	case MapEnter_NewGame:
		// new game, load between (auto-skipped) movie and crash landing cutscene
		// we reset timer on A press, so don't reset it again
		// TODO: offset timer because of autoskipped movie? maybe?
		break;

	case MapEnter_NewDay:
		// either entering area from world map, or entering cave from file select
		if (mIsCaveLoad) {
			// file select -> cave
			p2gz->timer->set_sub_timer_enabled(true);
			p2gz->timer->reset_main_timer();
		} else {
			// world map -> area, don't reset timer unless we're coming from file select
			if (p2gz->timer->get_FS_map_flag()) {
				p2gz->timer->set_enabled(true);
				p2gz->timer->set_sub_timer_enabled(false);

				p2gz->timer->reset_main_timer();
				p2gz->timer->set_FS_map_flag(false);
			}
		}
		break;

	case MapEnter_CaveEnter:
		// AG -> cave - don't reset either timer, since we need to do it before the save prompt in GameState
		break;

	case MapEnter_CaveGiveUp:
	case MapEnter_CaveExtinction:
		// cave results -> AG - weird niche case, but reset both + hide subtimer
		p2gz->timer->set_sub_timer_enabled(false);
		p2gz->timer->reset_main_timer();
		break;

	case MapEnter_CaveNavisDown:
		// either cave results -> AG, or file select -> AG - either way, reset the timer on load + hide sub timer
		p2gz->timer->set_sub_timer_enabled(false);
		p2gz->timer->reset_main_timer();
		break;

	case MapEnter_CaveGeyser:
		// file select -> AG (as if from cave) OR cave results -> AG OR cave sublevel -> cave sublevel
		if (mIsCaveDeeper) {
			// cave sublevel to next, don't reset anything, since we need to do it before the save prompt in CaveState
		} else {
			// file select or cave results -> AG - reset both, hide sub timer
			p2gz->timer->set_sub_timer_enabled(false);
			p2gz->timer->reset_main_timer();
		}
		break;
	}

	// @P2GZ - segment history
	// Start new segment on any loading screen
	gz::Segment* segment = p2gz->segment_history->start_segment();
	gz::Preset* preset   = nullptr;
	gz::WarpDestination dest;
	dest.area = game->mCurrentCourseInfo->mCourseIndex;

	if (p2gz->warp->warping) {
		preset = p2gz->warp->get_preset_during_warp();
		if (preset) {
			segment->preset = preset;
			dest.day        = preset->day;
			preset->ref();
		}
		if (p2gz->warp->using_set_seed()) {
			segment->seed         = p2gz->warp->get_seed();
			segment->use_set_seed = true;
		}
	}

	if (!preset) {
		segment->preset = p2gz->preset_mgr->create();
		// race mode gets weird if you reset day 1, this fixes that
		segment->preset->day  = Game::gameSystem->mTimeMgr->mDayCount + 1;
		segment->preset->time = Game::gameSystem->mTimeMgr->mCurrentTimeOfDay;
		dest.day              = Game::gameSystem->mTimeMgr->mDayCount;
	}

	if (!(mIsCaveLoad || mIsCaveDeeper)) {
		dest.cave = 0;
		if (mGameLoadType == Game::SingleGame::MapEnter_NewDay) {
			segment->preset->enter_kind = gz::PEK_FromMap;
		} else {
			segment->preset->enter_kind = gz::PEK_FromCave;
		}
	} else {
		ID32 cave_id(game->getCaveID());
		dest.cave     = game->mCurrentCourseInfo->getCaveIndex_FromID(cave_id) + 1;
		dest.sublevel = Game::playData->mCaveSaveData.mCurrentFloor;
	}

	segment->dest = dest;

	// @P2GZ localization-swap:
	// update treasure region before pellet loading if required
	if (p2gz->localization_op->require_update()) {
		p2gz->localization_op->update_region();
	}

	// @P2GZ - Structure editor and treasure editor
	// Clear structures currently in menu whenever we load a new area/sublevel
	p2gz->structure_editor->clear_gates();
	p2gz->structure_editor->clear_bridges();
	p2gz->structure_editor->clear_plugs();
	p2gz->structure_editor->clear_bags();
	p2gz->treasure_editor->clear_treasures();
}

/**
 * @note Address: N/A
 * @note Size: 0x160
 */
void LoadState::initNext(SingleGameSection* game)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x80239F38
 * @note Size: 0x238
 */
void LoadState::exec(SingleGameSection* game)
{
	if (!mIsInitialized) {
		mIsInitialized = true;
		if (!mDontClearHeap) {
			game->clearHeap();
		}

		// @P2GZ - show seed
		// Reset the flag that indicates the seed is for the currently generated floor
		// If we don't do this, the loading screen will show the previous RNG seed for a moment
		p2gz->segment_history->started_creating_map = false;

		if (mIsCaveLoad || mIsCaveDeeper) {
			if (mIsCaveDeeper) {
				game->mCurrentFloor++;
			}
			game->mInCave = true;
			og::Screen::DispMemberFloor dispFloor;

			dispFloor.mSublevel = game->mCurrentFloor + 1;
			dispFloor.mCaveID   = game->mCaveIndex;
			Screen::gGame2DMgr->open_Floor(dispFloor);
			gameSystem->mIsInCave = true;

		} else {
			int courseIdx = game->mCurrentCourseInfo->mCourseIndex;
			og::Screen::DispMemberCourseName dispCourseName;
			dispCourseName.mCourseIndex = courseIdx;

			Screen::gGame2DMgr->open_CourseName(dispCourseName);
			game->mInCave         = false;
			gameSystem->mIsInCave = false;
		}

		GameStat::clear();
		_10           = 0;
		mHasLoadBegun = false;
		mHasDrawn     = false;

	} else {
		if (mHasDrawn && !mHasLoadBegun) {
			sys->dvdLoadUseCallBack(&game->mDvdThread, game->mLoadGameCallback);
			mHasLoadBegun = true;
		}
		if (game->mDvdThread.mMode == DvdThreadCommand::CM_Completed) {
			game->postSetupFloatMemory();
			if (game->mInCave) {
				transit(game, SGS_Cave, nullptr);
			} else {
				GameArg arg(true, mGameLoadType);
				transit(game, SGS_Game, &arg);
			}

			// @P2GZ - memory logging
			OSReport("==== Game heap free/total: %.2f KB / %.2f KB\n", JKRGetCurrentHeap()->getTotalFreeSize() / 1024.0f,
			         JKRGetCurrentHeap()->getHeapSize() / 1024.0f);
			OSReport("==== Sys heap free/total: %.2f KB / %.2f KB\n", sys->mSysHeap->getTotalFreeSize() / 1024.0f,
			         sys->mSysHeap->getHeapSize() / 1024.0f);
		}
		if (particle2dMgr) {
			particle2dMgr->update();
		}
		Screen::gGame2DMgr->update();
	}
}

/**
 * @note Address: 0x8023A170
 * @note Size: 0x90
 */
void LoadState::draw(SingleGameSection* game, Graphics& gfx)
{
	if (mIsInitialized) {
		mHasDrawn = true;
		gfx.mPerspGraph.setPort();
		particle2dMgr->draw(1, 0);
		Screen::gGame2DMgr->draw(gfx);
		gfx.mPerspGraph.setPort();
		particle2dMgr->draw(0, 0);
	}
}

/**
 * @note Address: 0x8023A200
 * @note Size: 0x4
 */
void LoadState::cleanup(SingleGameSection*)
{
}

} // namespace SingleGame
} // namespace Game
