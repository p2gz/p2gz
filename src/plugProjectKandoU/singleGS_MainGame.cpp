#include "Game/DeathMgr.h"
#include "Game/Entities/ItemOnyon.h"
#include "Game/Entities/ItemDownFloor.h"
#include "Game/Entities/ItemPikihead.h"
#include "Game/Entities/ItemCave.h"
#include "Game/GameSystem.h"
#include "Game/Piki.h"
#include "Game/TimeMgr.h"
#include "Game/MapMgr.h"
#include "Game/gamePlayData.h"
#include "Game/gameStat.h"
#include "Game/generalEnemyMgr.h"
#include "Game/MoviePlayer.h"
#include "Game/Navi.h"
#include "Game/NaviParms.h"
#include "Game/PikiMgr.h"
#include "Game/SingleGame.h"
#include "P2Macros.h"
#include "PSGame/PikScene.h"
#include "PSSystem/PSGame.h"
#include "Radar.h"
#include "Screen/Game2DMgr.h"
#include "og/Screen/DispMember.h"
#include "PSSystem/PSMainSide_Scene.h"
#include "TParticle2dMgr.h"
#include "kh/khPayDept.h"
#include "nans.h"
#include "utilityU.h"
#include "Game/NaviState.h"
#include "GlobalData.h" // @P2GZ
#include "JSystem/J2D/J2DPrint.h" // @P2GZ
#include "P2JME/P2JME.h" //@P2GZ
#include "Dolphin/os.h" // @P2GZ

// @P2GZ
#include "Game/CameraMgr.h"
#include "JSystem/J2D/J2DPrint.h"
#include "og/Sound.h"
#include "GlobalData.h"

#define LOUIE_START_X   (-1260.0f)
#define LOUIE_START_Y   (-80.0f)
#define LOUIE_START_Z   (4350.0f)
#define LOUIE_START_DIR (7.6969025f) // in radians (even though this is above tau lol). it's like 81 degrees.

static const u32 padding[]    = { 0, 0, 0 };
static const char className[] = "SingleGS_Game";

namespace Game {
namespace SingleGame {

/**
 * @note Address: 0x802135AC
 * @note Size: 0x8B0
 */
void GameState::init(SingleGameSection* game, StateArg* arg)
{
	// @P2GZ Start
	s64 currentTime = OSTicksToMilliseconds(OSGetTime());
	SegmentRecord* previousRecord = p2gz->history->peek();
	if (previousRecord != nullptr) {
		previousRecord->endTime = currentTime;
	}

	SegmentRecord record;
	record.squad = playData->mPikiContainer;
	record.startTime = currentTime;
	p2gz->history->push(record);
	// @P2GZ End

	DeathMgr::mSoundDeathCount = 0;
	moviePlayer->reset();
	gameSystem->setFlag(GAMESYS_IsGameWorldActive);
	mIsPostExtinct = false;
	mCheckRepay    = 0;
	mInSaveScreen  = false;
	int courseID   = game->mCurrentCourseInfo->mCourseIndex;
	playData->setCurrentCourse(courseID);

	// Refill each captain life if they arent dead
	if (!(playData->mDeadNaviID & 1)) {
		playData->mNaviLifeMax[NAVIID_Olimar] = naviMgr->mNaviParms->mNaviParms.mMaxHealth;
		naviMgr->getAt(NAVIID_Olimar)->setLifeMax();
	}
	if (!(playData->mDeadNaviID >> 1 & 1)) {
		playData->mNaviLifeMax[NAVIID_Louie] = naviMgr->mNaviParms->mNaviParms.mMaxHealth;
		naviMgr->getAt(NAVIID_Louie)->setLifeMax();
	}

	GameArg* castedArg = static_cast<GameArg*>(arg);
	P2ASSERTLINE(624, castedArg);
	u16 startType = castedArg->mStartType;

	game->setFixNearFar(false, 0.0f, 0.0f);
	static_cast<newScreen::Mgr*>(Screen::gGame2DMgr->mScreenMgr)->mInCave      = false;
	static_cast<newScreen::Mgr*>(Screen::gGame2DMgr->mScreenMgr)->mCourseIndex = courseID;

	gameSystem->mIsInCave = false;
	game->mCurrentFloor   = 0;
	game->mIsExitingMap   = false;

	if ((playData->mDeadNaviID & 1) == 0) {
		game->setPlayerMode(0);
	} else {
		game->setPlayerMode(1);
	}
	game->setCamController();
	if (game->mWeatherEfx) {
		game->mWeatherEfx->create(nullptr);
	}
	game->setupMainMapGames();
	pikiMgr->debugShapeDL("BGS::GAME START");
	moviePlayer->getActiveGameCamera();

	switch (startType) {
	case MapEnter_CaveNavisDown: {
		DayEndArg dayEndArg(DayEndState::DETYPE_CaptainsDown);
		transit(game, SGS_DayEnd, &dayEndArg);
		return;
	} break;
	case MapEnter_NewGame: {
		char* courseName = const_cast<char*>(game->mCurrentCourseInfo->mName);
		MoviePlayArg moviePlayArg("x01_gamestart", courseName, game->mMovieFinishCallback, 0);
		playData->setDemoFlag(DEMO_Day_One_Start);

		// Set all enemies to be active in intro
		GeneralMgrIterator<EnemyBase> iEnemyMgr(generalEnemyMgr);
		CI_LOOP(iEnemyMgr) { iEnemyMgr.getObject()->movie_begin(false); }

		Iterator<Piki> iPiki(pikiMgr);
		CI_LOOP(iPiki) { (*iPiki)->movie_begin(false); }
		moviePlayArg.mDelegateStart = game->mMovieStartCallback;
		moviePlayer->play(moviePlayArg);
		gameSystem->mTimeMgr->setStartTime();
		game->mWeatherEfx->fade();
	} break;

	case MapEnter_NewDay: {
		int flagID = game->mCurrentCourseInfo->mCourseIndex + DEMO_Day_One_Start;
		if (!playData->isDemoFlag(flagID)) {
			playData->setDemoFlag(flagID);

			// Play a different cutscene depending on the map (if its your first visit)
			char* courseInMovies[4] = { "x01_gamestart", "x01_coursein_forest", "x01_coursein_yakushima", "x01_coursein_last" };
			IDelegate3<MovieConfig*, u32, u32>* delegate = game->mMovieFinishCallback;
			CourseInfo* info                             = game->mCurrentCourseInfo;
			char* course                                 = courseInMovies[info->mCourseIndex];
			char* demoName                               = const_cast<char*>(info->mName);
			MoviePlayArg moviePlayArg(course, demoName, delegate, 0);
			moviePlayArg.mDelegateStart = game->mMovieStartCallback;
			moviePlayer->play(moviePlayArg);
		} else {
			char* courseName = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("s00_coursein", courseName, game->mMovieFinishCallback, 0);
			moviePlayArg.mDelegateStart = game->mMovieStartCallback;
			moviePlayer->play(moviePlayArg);
		}
		gameSystem->mTimeMgr->setStartTime();
	} break;
	case MapEnter_CaveExtinction:
	case MapEnter_CaveGiveUp:
	case MapEnter_CaveGeyser: {
		char* courseName = const_cast<char*>(game->mCurrentCourseInfo->mName);
		MoviePlayArg moviePlayArg("s0E_return_cave", courseName, game->mMovieFinishCallback, 0);
		moviePlayArg.mDelegateStart = game->mMovieStartCallback;
		moviePlayer->play(moviePlayArg);

		// Force all onion beams to be active
		for (int i = 0; i < 3; i++) {
			if (playData->hasBootContainer(i)) {
				Onyon* onyon = ItemOnyon::mgr->getOnyon(i);
				if (onyon) {
					onyon->setSpotState(Onyon::SPOTSTATE_Opened);
				}
			}
		}
		if (startType == 1) {
			gameSystem->mTimeMgr->setTime(playData->mCaveSaveData.mTime);
		}
	} break;
	default:
		JUT_PANICLINE(832, "illegal state GameState::init\n");
	}

	sys->heapStatusDump(true);
	gameSystem->mTimeMgr->mFlags.unset(TIMEFLAG_Stopped);
	if (startType != MapEnter_CaveExtinction) {
		// Check if any pikmin types are extinct, if they are, the post-extinction cutscene is needed
		bool noPikisLeft = false;
		for (int i = 0; i <= 2; i++) {
			if (playData->hasContainer(i) && GameStat::getAllPikmins(i) == 0) {
				playData->mPikiContainer.getColorSum(i);
				noPikisLeft = true;
				break;
			}
		}
		if (noPikisLeft) {
			mIsPostExtinct = true;
		}
	} else {
		mIsPostExtinct = false;
	}
	_11 = 0;
	_14 = 6.0f;
	game->clearCaveMenus();
	mDoExit = 0;
	gameSystem->resetFlag(GAMESYS_IsPlaying);
	Screen::gGame2DMgr->initInCourse();
	game->mTreasureRadarActive = 0;

	// Do a dumb calcNearestTreasure because any non-0 return means a treasure at least exists
	Vector3f dummyPos;
	f32 dummyDist;
	if (Radar::mgr->calcNearestTreasure(Vector3f::zero, FLOAT_DIST_MAX, dummyPos, dummyDist) == 0) {
		game->mNeedTreasureCalc = true;
	} else {
		game->mNeedTreasureCalc = false;
	}

	// Change sound effects if no treasures left
	if (Radar::mgr && Radar::mgr->getNumOtakaraItems() == 0) {
		PSM::Scene_Ground* scene = static_cast<PSM::Scene_Ground*>(PSMGetChildScene());
		PSSystem::checkGameScene(scene);
		scene->setPollutUp();
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x174
 */
unknown GameState::gameStart(SingleGameSection*)
{
	// Feels like there should be more to this, but the DispObjGround stuff is sometimes before this stuff, sometimes after, idk
	gameSystem->setFlag(GAMESYS_IsPlaying);
	if (gameSystem->mTimeMgr->mDayCount != 0) {
		PSMGetSceneMgr()->mScenes->mChild->startMainSeq();
	} else {
		static_cast<PSM::Scene_Objects*>(PSMGetSceneMgr()->mScenes->mChild)->onStartMainSeq();
		static_cast<PSM::Scene_Objects*>(PSMGetSceneMgr()->mScenes->mChild)->getEnvSe()->on();
	}
}

/**
 * @note Address: 0x80213E5C
 * @note Size: 0x288
 */
bool GameState::check_DemoInout(SingleGameSection* game)
{
	if ((!playData->isDemoFlag(DEMO_Purples_In_Ship)) && (0 < playData->mPikiContainer.getColorSum(Purple))) {
		playData->setDemoFlag(DEMO_Purples_In_Ship);

		char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
		MoviePlayArg moviePlayArg("g26_inout_black", name, game->mMovieFinishCallback, 0);
		if (ItemOnyon::mgr->mUfo) {
			ItemOnyon::mgr->mUfo->movie_begin(false);
		}

		moviePlayer->play(moviePlayArg);
		return true;
	}

	if (gameSystem->isFlag(GAMESYS_IsGameWorldActive)
	    && (!playData->isDemoFlag(DEMO_Whites_In_Ship) && (0 < playData->mPikiContainer.getColorSum(White)))) {
		playData->setDemoFlag(DEMO_Whites_In_Ship);

		char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
		MoviePlayArg moviePlayArg("g29_inout_white", name, game->mMovieFinishCallback, 0);

		if (ItemOnyon::mgr->mUfo) {
			ItemOnyon::mgr->mUfo->movie_begin(false);
		}

		moviePlayer->play(moviePlayArg);
		return true;
	}

	if (gameSystem->isFlag(GAMESYS_IsGameWorldActive)
	    && (!playData->isDemoFlag(DEMO_Reds_In_Onion) && (0 < playData->mPikiContainer.getColorSum(Red)))) {
		playData->setDemoFlag(DEMO_Reds_In_Onion);

		char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
		MoviePlayArg moviePlayArg("g2C_inout_red", name, game->mMovieFinishCallback, 0);

		Onyon* redOnyon = ItemOnyon::mgr->getOnyon(Red);
		if (redOnyon) {
			redOnyon->movie_begin(false);
		}

		if (ItemOnyon::mgr->mUfo) {
			ItemOnyon::mgr->mUfo->movie_begin(false);
		}

		moviePlayer->play(moviePlayArg);
		return true;
	}

	return false;
}

/**
 * @note Address: 0x802140E4
 * @note Size: 0xC
 */
void GameState::on_section_fadeout(SingleGameSection*) { mDoExit = 1; }

/**
 * @note Address: 0x802140F0
 * @note Size: 0xB28
 */
void GameState::on_demo_timer(SingleGameSection* game, u32 id)
{
	// Yes its not a switch
	if (id == DEMOTIMER_YouAppearLost) {
		ItemDownFloor::Item* bagObj = nullptr;
		Iterator<BaseItem> iterator(ItemDownFloor::mgr);
		CI_LOOP(iterator)
		{
			ItemDownFloor::Item* dwfl = static_cast<ItemDownFloor::Item*>(*iterator);
			if (dwfl->mBagMaxWeight == 15) {
				bagObj = dwfl;
			}
		}
		JUT_ASSERTLINE(1025, bagObj, "no down floor 20\n"); // This panic seems to imply the bag once weighed 20 instead of 15

		MoviePlayArg moviePlayArg("x17_join_guide", nullptr, nullptr, 0);
		moviePlayArg.mOrigin       = bagObj->getPosition();
		moviePlayArg.mAngle        = bagObj->getFaceDir();
		moviePlayer->mTargetObject = bagObj;
		moviePlayer->play(moviePlayArg);
		gameSystem->mSection->disableTimer(DEMOTIMER_YouAppearLost);
	}
	if (id == DEMOTIMER_Reds_Purples_Tutorial) {
		Navi* navi = naviMgr->getActiveNavi();
		if (!navi) {
			int id = 1;
			if (!gameSystem->mSection->mPrevNaviIdx)
				id = 0;
			navi = naviMgr->getAt(id);
		}
		playData->setDemoFlag(DEMO_Reds_Purples_Tutorial);
		MoviePlayArg moviePlayArg("x15_exp_x", nullptr, nullptr, 0);
		moviePlayArg.mOrigin       = navi->getPosition();
		moviePlayArg.mAngle        = navi->getFaceDir();
		moviePlayer->mTargetObject = navi;
		moviePlayer->play(moviePlayArg);
		gameSystem->mSection->disableTimer(DEMOTIMER_Reds_Purples_Tutorial);
	}
	if (id == DEMOTIMER_Meet_Red_Pikmin) {
		Navi* navi = naviMgr->getActiveNavi();
		if (!navi) {
			int id = 1;
			if (!gameSystem->mSection->mPrevNaviIdx)
				id = 0;
			navi = naviMgr->getAt(id);
		}
		Iterator<Piki> iterator(pikiMgr);
		CI_LOOP(iterator)
		{
			Piki* piki = *iterator;
			piki->movie_begin(false);
		}
		playData->setDemoFlag(DEMO_Meet_Red_Pikmin);
		MoviePlayArg moviePlayArg("x02_watch_red_pikmin", nullptr, game->mMovieFinishCallback, 0);
		moviePlayArg.mOrigin       = navi->getPosition();
		moviePlayArg.mAngle        = navi->getFaceDir();
		moviePlayer->mTargetObject = navi;
		moviePlayer->play(moviePlayArg);
		gameSystem->mSection->disableTimer(DEMOTIMER_Meet_Red_Pikmin);
	}
	if (id == DEMOTIMER_Unlock_Switch_To_Louie) {
		Navi* navi = naviMgr->getActiveNavi();
		if (!navi) {
			int id = 1;
			if (!gameSystem->mSection->mPrevNaviIdx)
				id = 0;
			navi = naviMgr->getAt(id);
		}
		MoviePlayArg moviePlayArg("x04_exp_y", nullptr, game->mMovieFinishCallback, 0);
		moviePlayArg.mOrigin = navi->getPosition();
		moviePlayArg.mAngle  = navi->getFaceDir();
		moviePlayer->play(moviePlayArg);
		playData->setDemoFlag(DEMO_Unlock_Captain_Switch);
		game->disableTimer(DEMOTIMER_Unlock_Switch_To_Louie);
	}
	if (id == DEMOTIMER_Piki_Seed_In_Ground) {
		if (!playData->isDemoFlag(DEMO_Pluck_First_Pikmin)) {
			ItemPikihead::Item* seed = nullptr;
			Iterator<ItemPikihead::Item> iterator(ItemPikihead::mgr);
			CI_LOOP(iterator)
			{
				seed = *iterator;
				break;
			}
			P2ASSERTLINE(1138, seed);
			MoviePlayArg moviePlayArg("g01_pick_me", nullptr, game->mMovieFinishCallback, 0);
			moviePlayArg.mOrigin = seed->getPosition();
			moviePlayArg.mAngle  = seed->getFaceDir();
			moviePlayer->play(moviePlayArg);
			playData->setDemoFlag(DEMO_Pluck_First_Pikmin);
		}
		game->disableTimer(DEMOTIMER_Piki_Seed_In_Ground);
	} else if (id == DEMOTIMER_Camera_Tutorial) {
		if (!playData->isDemoFlag(DEMO_UNUSED_Camera_Demo)) {
			if (naviMgr->getActiveNavi()) {
				char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
				MoviePlayArg moviePlayArg("g33_camera_demo", name, game->mMovieFinishCallback, 0);
				Navi* navi = naviMgr->getAt(NAVIID_Olimar);
				if (!navi) {
					navi = naviMgr->getActiveNavi();
				}
				moviePlayArg.mOrigin = navi->getPosition();
				moviePlayArg.mAngle  = navi->getFaceDir();
				moviePlayer->play(moviePlayArg);
				playData->setDemoFlag(DEMO_UNUSED_Camera_Demo);
			} else {
				return;
			}
		}
		game->disableTimer(DEMOTIMER_Camera_Tutorial);
	}
}

/**
 * @note Address: 0x80214C18
 * @note Size: 0x6E0
 */
void GameState::exec(SingleGameSection* game)
{
	if (mDoExit) {
		return;
	}

	// when you enter a cave from above ground, this state is still technically active until after the game saves
	if (mInSaveScreen) {
		particle2dMgr->update();
		Screen::gGame2DMgr->update();
		if ((u8)Screen::gGame2DMgr->check_Save()) {
			// MapEnter type isnt used when loading into caves, someone put 100 here for the funny
			LoadArg arg(100, true, false, false);
			transit(game, SGS_Load, &arg);
		}
		return;
	}

	game->BaseGameSection::doUpdate();

	// the state has changed, abort rest of the function
	if (game->mCurrentState->mId != mId) {
		return;
	}

	game->updateMainMapScreen();

	// Check starting the "you appear lost" cutscene timer
	if (GameStat::getMapPikmins(AllPikminCalcs) >= 15 && moviePlayer->mDemoState == DEMOSTATE_Inactive
	    && !playData->isDemoFlag(DEMO_You_Appear_Lost) && playData->hasBootContainer(Red)) {
		playData->setDemoFlag(DEMO_You_Appear_Lost);
		game->enableTimer(180.0f, DEMOTIMER_YouAppearLost);
	}

	if (moviePlayer->mDemoState == DEMOSTATE_Inactive && needRepayDemo()) {
		startRepayDemo();
	}

	// Check if anything needs to be done following a % of debt cutscene
	int repaystate = updateRepayDemo();
	switch (repaystate) {
	case RDS_Started:
		return;
	case RDS_GoToPayDebt: { // end the day, go to ending (debt repayed)
		pikiMgr->forceEnterPikmins(false);
		game->saveToGeneratorCache(game->mCurrentCourseInfo);
		game->advanceDayCount();
		gameSystem->setPause(false, "repay-done", 3);
		EndingArg arg(0);
		transit(game, SGS_Ending, &arg);
		return;
	}
	case RDS_GameComplete: { // end the day, go to ending (all treasures)
		pikiMgr->forceEnterPikmins(false);
		game->saveToGeneratorCache(game->mCurrentCourseInfo);
		game->advanceDayCount();
		gameSystem->setPause(false, "repay-done", 3);
		EndingArg arg(EndingState::Ending_IsComplete);
		transit(game, SGS_Ending, &arg);
		return;
	}

	case RDS_DemoPlaying: {
		PSPause_StartMenuOff();
		gameSystem->setPause(false, "repay-done", 3);
		return;
	}
	}

	// Don't continue if in cave/geyser menus
	if (game->mOpenMenuFlags && !game->updateCaveMenus()) {
		return;
	}

	Screen::gGame2DMgr->setGamePad(game->mControllerP1);

	// Check status of opened pause menu
	int menustate = Screen::gGame2DMgr->check_SMenu();
	switch (menustate) {
	case Screen::Game2DMgr::CHECK2D_SMenu_Opened:
		break;
	case Screen::Game2DMgr::CHECK2D_SMenu_Cancel: // cancel menu
		gameSystem->setMoviePause(false, "sm-canc");
		gameSystem->setPause(false, "sm-canc", 3);
		break;
	case Screen::Game2DMgr::CHECK2D_SMenu_GoToSunset:
		gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		gameSystem->setMoviePause(false, "sm-ugot");
		gameSystem->setPause(false, "sm-ugot", 3);
		DayEndArg arg(DayEndState::DETYPE_Normal);
		transit(game, SGS_DayEnd, &arg);
		return;
	case Screen::Game2DMgr::CHECK2D_SMenu_ReturnToFileSelect:
		P2ASSERTLINE(1304, Screen::gGame2DMgr->mScreenMgr->reset() == 1);
		playData->mDeadNaviID = 0;
		naviMgr->clearDeadCount();
		gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		gameSystem->setMoviePause(false, "sm-zenk");
		gameSystem->setPause(false, "sm-zenk", 3);
		game->clearHeap();
		transit(game, SGS_File, nullptr);
		return;
	case Screen::Game2DMgr::CHECK2D_SMenu_EscapeCave:
		JUT_PANICLINE(1318, "smenu_escape\n");
		break;
	default:
		// Check open pause menu
		if (!gameSystem->isFlag(GAMESYS_DisablePause) && moviePlayer->mDemoState == DEMOSTATE_Inactive && !gameSystem->paused()
		    && game->mControllerP1->getButtonDown() & Controller::PRESS_START) {
			og::Screen::DispMemberSMenuAll disp;
			game->setDispMemberSMenu(disp);
			if (Screen::gGame2DMgr->open_SMenu(disp)) {
				gameSystem->setPause(true, "open-sm", 3);
				gameSystem->setMoviePause(true, "open-sm");
			}
		}
	}

	// Check if the day time is up
	if (!gameSystem->paused_soft() && !gameSystem->isFlag(GAMESYS_DisablePause) && gameSystem->mTimeMgr->isDayOver()
	    && !game->mOpenMenuFlags && gameSystem->isFlag(GAMESYS_IsGameWorldActive)) {
		gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		DayEndArg arg(DayEndState::DETYPE_Normal);
		transit(game, SGS_DayEnd, &arg);
		return;
	}

	// Check need pikmin extinction to occur
	if (!mIsPostExtinct && moviePlayer->mDemoState == DEMOSTATE_Inactive) {
		if (GameStat::getAllPikmins(AllPikminCalcs) - GameStat::getZikatuPikmins(AllPikminCalcs) == 0 && playData->hasBootContainer(Red)) {
			gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
			MoviePlayArg moviePlayArg("s05_pikminzero", nullptr, game->mMovieFinishCallback, 0);
			Navi* navi = naviMgr->getActiveNavi();
			if (!navi) {
				int id = 1;
				if (!gameSystem->mSection->mPrevNaviIdx)
					id = 0;
				navi = naviMgr->getAt(id);
			}
			moviePlayArg.mDelegateStart = game->mMovieStartCallback;
			moviePlayArg.mOrigin        = navi->getPosition();
			moviePlayArg.mAngle         = navi->getFaceDir();
			moviePlayer->play(moviePlayArg);
			return;
		}
	}

	PSM::PikminNumberDirector* director = PSMGetPikminNumD();
	int pikis                           = GameStat::getMapPikmins_exclude_Me(AllPikminCalcs);
	if (pikis < 10 && DeathMgr::mSoundDeathCount > 0) {
		if (director) {
			director->directOn();
		}
	} else {
		if (director) {
			director->directOff();
		}
	}

	// @P2GZ: teleport menu
	Navi* navi = naviMgr->getActiveNavi();
	if (navi != nullptr) {
		PlayCamera* camera = cameraMgr->mCameraObjList[navi->getNaviID()];

		Graphics* gfx = sys->getGfx();
		gfx->initPerspPrintf(gfx->mCurrentViewport);
		gfx->initPrimDraw(nullptr);
		gfx->mOrthoGraph.setPort();

		J2DPrint inactive(JFWSystem::systemFont, 0.0f);
		inactive.initiate();
		inactive.mCharColor.set(JUtility::TColor(255, 255, 255, 128));
		inactive.mGradientColor.set(JUtility::TColor(255, 255, 255, 128));
		inactive.mGlyphWidth = 16.0f;
		inactive.mGlyphHeight = 16.0f;

		J2DPrint active(JFWSystem::systemFont, 0.0f);
		active.initiate();
		active.mCharColor.set(JUtility::TColor(255, 255, 255, 255));
		active.mGradientColor.set(JUtility::TColor(255, 255, 255, 255));
		active.mGlyphWidth = 16.0f;
		active.mGlyphHeight = 16.0f;

		if (p2gz->mIsScrollingCamera && p2gz->mIsSaveLoadPosition) {
			active.print(82, 92, "Save Position");
			active.print(82, 116, "Load Position");
			active.print(82, 140, "Go Back");
			
			for (int i = 0; i < 4; i++) {
				char position[32];
				sprintf(position, "(%d, %d)", int(p2gz->mSavedPositions[i].x), int(p2gz->mSavedPositions[i].z));
				if (p2gz->mSelectedPositionSlot == i) {
					active.print(50, 172 + (24 * i), "Slot %d - %s", i + 1, p2gz->mSavedPositions[i] == Vector3f::zero ? "unused" : position);
				} else {
					inactive.print(50, 172 + (24 * i), "Slot %d - %s", i + 1, p2gz->mSavedPositions[i] == Vector3f::zero ? "unused" : position);
				}
			}

			// NB: drawing J2DPictures must occur after all text is printed
			p2gz->mAButtonPicture->draw(50.0f, 76.0f, 16, 16, false, false, false);
			p2gz->mXButtonPicture->draw(50.0f, 100.0f, 16, 16, false, false, false);
			p2gz->mBButtonPicture->draw(50.0f, 124.0f, 16, 16, false, false, false);

			camera->mGoalPosition = camera->mGoalPosition;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.0f;

			if (navi->mController1->getButtonDown() & Controller::PRESS_UP || navi->mController1->getButtonDown() & Controller::PRESS_DPAD_UP) {
				p2gz->mSelectedPositionSlot = (p2gz->mSelectedPositionSlot - 1 + 4) % 4;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_DOWN || navi->mController1->getButtonDown() & Controller::PRESS_DPAD_DOWN) {
				p2gz->mSelectedPositionSlot = (p2gz->mSelectedPositionSlot + 1) % 4;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_A) {
				Vector3f naviPos = camera->mGoalPosition;
				naviPos.y = mapMgr->getMinY(camera->mGoalPosition);
				p2gz->mSavedPositions[p2gz->mSelectedPositionSlot] = naviPos;
				og::ogSound->setDecide();
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_B) {
				p2gz->mIsSaveLoadPosition = false;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_X) {
				// don't allow warping to an unused slot
				if (p2gz->mSavedPositions[p2gz->mSelectedPositionSlot] == Vector3f::zero) {
					og::ogSound->setError();
					return;
				}

				// exit scroll state
				p2gz->mIsScrollingCamera = false;
				p2gz->mIsSaveLoadPosition = false;
				gameSystem->setPause(false, "cameraScroll", 3);

				// reset camera parameters
				camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
				camera->mGoalVerticalAngle = PI / 8;
				camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

				// set navi position
				navi->setPosition(p2gz->mSavedPositions[p2gz->mSelectedPositionSlot], false);

				// set squad position
				Iterator<Piki> iterator(pikiMgr);
				CI_LOOP(iterator)
				{
					Piki* piki = *iterator;
					if (piki->mNavi == navi) {
						piki->setPosition(p2gz->mSavedPositions[p2gz->mSelectedPositionSlot], false);
					}
				}

				og::ogSound->setDecide();
			}

			return;
		}

		// enter scroll state
		if (!p2gz->mIsScrollingCamera && !gameSystem->paused() && navi->getStateID() != NSID_ThrowWait && navi->mController1->getButtonDown() & Controller::PRESS_DPAD_LEFT) {
			p2gz->mIsScrollingCamera = true;
			gameSystem->setPause(true, "cameraScroll", 3);

			// set camera parameters
			camera->mGoalPosition += Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle = PI / 2;

			og::ogSound->setOpen();
			return;
		}

		// switch captains
		if (p2gz->mIsScrollingCamera && naviMgr->getActiveNavi() != nullptr && navi->mController1->getButtonDown() & Controller::PRESS_Y) {
			Navi* otherNavi = naviMgr->getAt(GET_OTHER_NAVI(navi));
			int otherNaviID = otherNavi->getStateID();

			if (otherNavi->isAlive() && otherNaviID != NSID_Nuku && otherNaviID != NSID_NukuAdjust && otherNaviID != NSID_Punch) {
				// play captain switch sound effect
				if (navi->mNaviIndex == NAVIID_Olimar) {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_LUI, 0);
				} else if (playData->isStoryFlag(STORY_DebtPaid)) {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_SHACHO, 0);
				} else {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_ORIMA, 0);
				}

				// reset camera position for current captain
				camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);

				gameSystem->mSection->pmTogglePlayer();

				// update parameters for other navi's camera
				cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mGoalPosition += Vector3f(0, p2gz->mScrollCameraZoom, 0);
				cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mGoalVerticalAngle = PI / 2;
				cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;
				return;
			}
		}

		// cancel
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_B) {
			// exit scrolling state
			p2gz->mIsScrollingCamera = false;
			gameSystem->setPause(false, "cameraScroll", 3);

			// reset camera parameters
			camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle = PI / 8;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

			og::ogSound->setCancel();
		}

		// warp
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_A) {
			// exit scrolling state
			p2gz->mIsScrollingCamera = false;
			gameSystem->setPause(false, "cameraScroll", 3);

			// reset camera parameters
			camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle = PI / 8;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

			// set navi position
			Vector3f naviPos = camera->mGoalPosition;
			naviPos.y = mapMgr->getMinY(camera->mGoalPosition);
			navi->setPosition(naviPos, false);

			// set squad position
			Iterator<Piki> iterator(pikiMgr);
			CI_LOOP(iterator)
			{
				Piki* piki = *iterator;
				if (piki->mNavi == navi) {
					piki->setPosition(naviPos, false);
				}
			}

			og::ogSound->setDecide();
		}

		// open saved positions menu
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_R) {
			p2gz->mIsSaveLoadPosition = true;
			return;
		}

		// calculate new camera position
		if (p2gz->mIsScrollingCamera) {
			// set camera parameters
			camera->mGoalVerticalAngle = PI / 2;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;
			
			// calculate control stick vector
			f32 ax = 0.0f;
			f32 az = ax;
			if (navi->mController1) {
				ax = -navi->mController1->getMainStickX();
				az = navi->mController1->getMainStickY();
			}
			Vector3f inputPos(ax, 0.0f, az);
			navi->reviseController(inputPos);

			f32 x = inputPos.x;
			f32 z = inputPos.z;

			Vector3f side = camera->getSideVector();
			Vector3f up   = camera->getUpVector();
			Vector3f view = camera->getViewVector();
			side.y        = 0.0f;

			side.qNormalise();

			if (up.y > view.y) {
				view.x = view.x;
				view.z = view.z;
			} else {
				view.x = up.x;
				view.z = up.z;
			}
			Vector3f view2D(view.x, 0.0f, view.z);
			view2D.qNormalise();

			Vector3f result(side * x + view2D * z);

			if (result != Vector3f::zero) {
				active.print(82, 92, "Move");
			} else {
				inactive.print(82, 92, "Move");
			}
			f32 cStickX = navi->mController1->getSubStickX();
			f32 cStickY = navi->mController1->getSubStickY();
			if (cStickX != 0 || cStickY != 0) {
				active.print(82, 116, "Camera");
			} else {
				inactive.print(82, 116, "Camera");
			}

			inactive.print(82, 140, "Warp");
			inactive.print(82, 164, "Cancel");
			inactive.print(82, 188, "Switch");

			if (navi->mController1->getButton() & Controller::PRESS_L) {
				active.print(82, 212, "Move Faster");
			} else {
				inactive.print(82, 212, "Move Faster");
			}

			inactive.print(82, 236, "Saved Positions");

			p2gz->mControlStickPicture->draw(50.0f, 76.0f, 16, 16, false, false, false);
			p2gz->mCStickPicture->draw(50.0f, 100.0f, 16, 16, false, false, false);
			p2gz->mAButtonPicture->draw(50.0f, 124.0f, 16, 16, false, false, false);
			p2gz->mBButtonPicture->draw(50.0f, 148.0f, 16, 16, false, false, false);
			p2gz->mYButtonPicture->draw(50.0f, 172.0f, 16, 16, false, false, false);
			p2gz->mLButtonPicture->draw(50.0f, 196.0f, 16, 16, false, false, false);
			p2gz->mRButtonPicture->draw(50.0f, 220.0f, 16, 16, false, false, false);

			// calculate and set new position, unless there is no collision
			int speed = (navi->mController1->getButton() & Controller::PRESS_L) ? 32 : 16;
			Vector3f goalPosition(camera->mGoalPosition.x + result.x * speed, camera->mGoalPosition.y, camera->mGoalPosition.z + result.z * speed);

			// circle color
			int rgb = p2gz->getAnimationCoefficient() * 255;
			Color4 color = Color4(rgb, rgb, rgb, 255);

			if (mapMgr->getMinY(goalPosition) > -1000) {
				camera->mGoalPosition = goalPosition;
			} else {
				gfx->mOrthoGraph.setPort();
				J2DPrint print(JFWSystem::systemFont, 0.0f);
				print.initiate();
				print.mCharColor.set(JUtility::TColor(rgb, 0, 0, 255));
				print.mGradientColor.set(JUtility::TColor(rgb, 0, 0, 255));
				print.mGlyphWidth = 16.0f;
				print.mGlyphHeight = 16.0f;
				print.print(100, 200, "Can't move out of bounds!");
				color = Color4(rgb, 0, 0, 255);
			}
			
			// change camera angle with dampening on c-stick value
			camera->mCameraAngleTarget -= cStickX * 0.05f;

			// set zoom level with clamping
			if (p2gz->mScrollCameraZoom > 0 && p2gz->mScrollCameraZoom - cStickY * 25 > 0 && p2gz->mScrollCameraZoom < 1700 && p2gz->mScrollCameraZoom - cStickY * 25 < 1700) {
				p2gz->mScrollCameraZoom -= cStickY * 25;
				camera->mGoalPosition -= Vector3f(0, cStickY * 25, 0);
				if (cStickY > 0) {
					og::ogSound->setZoomIn();
				} else if (cStickY < 0) {
					og::ogSound->setZoomOut();
				}
			}
			
			// draw circle
			gfx->initPerspPrintf(gfx->mCurrentViewport);

			f32 radius = 4 + 4 * p2gz->getAnimationCoefficient();
			Vector3f naviPos = camera->mGoalPosition;
			Vector3f vertices[3];
			vertices[0] = naviPos;
			
			for (int i = 0; i < 32; i++) {
				f32 theta = -HALF_PI - (TAU * i / 32);
				vertices[1] = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + naviPos;
				
				f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
				vertices[2] = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + naviPos;

				GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
				for (int j = 0; j < 3; j++) {
					GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
					GXColor4u8(color.r, color.g, color.b, 255);
				}
				GXEnd();
			}
		}
	}
}

/**
 * @note Address: 0x802154D4
 * @note Size: 0xD4
 */
void GameState::onOrimaDown(SingleGameSection* game, int id)
{
	MoviePlayArg moviePlayArg("s03_orimadown", nullptr, game->mMovieFinishCallback, id);
	moviePlayArg.mDelegateStart = game->mMovieStartCallback;
	Navi* navi                  = naviMgr->getAt(id);
	moviePlayer->mTargetNavi    = navi;
	if (id == 0) {
		moviePlayer->mActingCamera = game->mOlimarCamera;
	} else {
		moviePlayer->mActingCamera = game->mLouieCamera;
	}
	moviePlayer->play(moviePlayArg);
}

/**
 * @note Address: 0x802155A8
 * @note Size: 0xDC
 */
void GameState::onHoleIn(SingleGameSection* game, ItemCave::Item* item)
{
	MoviePlayArg moviePlayArg("s09_holein", nullptr, game->mMovieFinishCallback, 0);
	moviePlayArg.mDelegateStart = game->mMovieStartCallback;
	moviePlayArg.mOrigin        = item->getPosition();
	moviePlayArg.mAngle         = item->getFaceDir();
	moviePlayer->mTargetObject  = item;
	moviePlayArg.mNaviID        = item->mCaveID.getID();
	moviePlayer->play(moviePlayArg);
}

/**
 * @note Address: 0x80215684
 * @note Size: 0x4
 */
void GameState::onMovieCommand(SingleGameSection*, int) { }

/**
 * @note Address: 0x80215688
 * @note Size: 0x158
 */
void GameState::onMovieStart(SingleGameSection* game, MovieConfig* config, u32, u32 naviID)
{
	Screen::gGame2DMgr->startFadeBG_CourseName();
	Screen::gGame2DMgr->startCount_CourseName();

	if (config->is("s0E_return_cave")) {
		game->createFallPikminSound();
	}

	if (config->is("s05_pikminzero")) {
		Screen::gGame2DMgr->open_GameOver(Screen::Game2DMgr::GOTITLE_PikminZero);
	}

	if (config->is("s03_orimadown")) {
		Screen::Game2DMgr::GameOverTitle naviType;
		if (naviID == 0) {
			naviType = Screen::Game2DMgr::GOTITLE_OlimarDown;
			game->setPlayerMode(0);
		} else {
			naviType = (playData->mStoryFlags & STORY_DebtPaid) ? Screen::Game2DMgr::GOTITLE_PresidentDown
			                                                    : Screen::Game2DMgr::GOTITLE_LouieDown;
			game->setPlayerMode(1);
		}
		Screen::gGame2DMgr->open_GameOver(naviType);
	}

	if (config->is("s09_holein")) {
		bool isSC = false;
		if (naviID == 'y_04')
			isSC = true;
		game->saveMainMapSituation(isSC);
		Vector3f pos = game->mCurrentCave->getPosition();
		game->prepareHoleIn(pos, false);
	}
}

/**
 * @note Address: 0x802157E0
 * @note Size: 0x1A7C
 */
void GameState::onMovieDone(SingleGameSection* game, MovieConfig* config, u32, u32 id)
{
	bool repay = needRepayDemo();

	// Check first treasure collected day end
	if (config->is("s10_suck_treasure")) {
		playData->getGroundOtakaraNum(0);

		// Check valley of repose treasures collected, if 1 now, and 0 at day start, play day end cutscene
		if (playData->getGroundOtakaraNum(0) == 1 && playData->getGroundOtakaraNum_Old(0) == 0) {
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("x07_first_recovery", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}
	}

	// End day after first treasure day end cutscene
	if (config->is("x07_first_recovery")) {
		DayEndArg arg(DayEndState::DETYPE_Normal);
		transit(game, SGS_DayEnd, &arg);
		return;
	}

	// After returning from a cave for the first time cutscene
	if (config->is("g08_first_return")) {
		if (!playData->isDemoFlag(DEMO_First_Globe_Day_End) && playData->mOlimarData[0].hasItem(OlimarData::ODII_SphericalAtlas)) {
			playData->setDemoFlag(DEMO_First_Globe_Day_End);
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("g32_get_map", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}
		if (repay && playData->getRepayLevel() == 9) {
			startRepayDemo();
			return;
		}
		if (check_DemoInout(game)) {
			return;
		}
		if (mIsPostExtinct) {
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("s13_pikmin_supply", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}
		gameStart(game);
	}

	// After finding the red onion
	if (config->is("x03_find_red_onyon")) {
		if (!playData->isDemoFlag(DEMO_Pluck_First_Pikmin)) {
			game->enableTimer(20.0f, DEMOTIMER_Piki_Seed_In_Ground);
		}
	}

	// After first globe day end (paying the debt overpowers its day ending... with day ending)
	if (config->is("g32_get_map")) {
		if (repay && playData->getRepayLevel() == 9) {
			startRepayDemo();
		} else {
			DayEndArg arg(DayEndState::DETYPE_Normal);
			transit(game, SGS_DayEnd, &arg);
		}
		return;
	}

	// After start of day 1, or first cave return
	if (config->is("x01_gamestart") || config->is("g08_first_return")) {
		if (config->is("x01_gamestart")) {
			game->mWeatherEfx->create(nullptr);
		}
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		gameStart(game);
	}

	// After start of day 1 cutscene
	if (config->is("x01_gamestart")) {
		// Le funny hardcoded louie day 1 position
		Navi* louie = naviMgr->getAt(NAVIID_Louie);
		Vector3f pos(LOUIE_START_X, LOUIE_START_Y, LOUIE_START_Z);
		louie->mFaceDir = roundAng(LOUIE_START_DIR);
		pos.y           = mapMgr->getMinY(pos);
		louie->setPosition(pos, false);
		louie->mFsm->start(louie, 0, nullptr);
	}

	// After first time using president (start gameplay if no other cutscenes)
	if (config->is("g35_president_gamestart")) {
		if (mIsPostExtinct) {
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("s13_pikmin_supply", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}
		gameStart(game);
		if (check_DemoInout(game)) {
			return;
		}
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		return;
	}

	// Regular/first time course landing, check usual stuff after it
	if (config->is("s00_coursein") || config->is("x01_coursein_forest") || config->is("x01_coursein_yakushima")
	    || config->is("x01_coursein_last")) {
		if ((playData->mStoryFlags & STORY_DebtPaid) && !playData->isDemoFlag(DEMO_President_Start)) {
			playData->setDemoFlag(DEMO_President_Start);
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("g35_president_gamestart", name, game->mMovieFinishCallback, 0);
			moviePlayArg.mDelegateStart = game->mMovieStartCallback;
			moviePlayer->play(moviePlayArg);
			Screen::gGame2DMgr->close_CourseName();
			return;
		}

		if (mIsPostExtinct) {
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("s13_pikmin_supply", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}

		if (check_DemoInout(game)) {
			Screen::gGame2DMgr->close_CourseName();
			return;
		}

		gameStart(game);
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		return;
	}

	// After entering a cave hole
	if (config->is("s09_holein")) {
		moviePlayer->clearSuspendedDemo();
		og::Screen::DispMemberSave disp;
		disp.mDoSound = true;
		PSMCancelToPauseOffMainBgm();
		Screen::gGame2DMgr->open_Save(disp);
		mInSaveScreen = true;
		return;
	}

	// After post pikmin extinction cutscene
	if (config->is("s13_pikmin_supply")) {
		mIsPostExtinct = false;
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		gameStart(game);
		return;
	}

	// regular cave returning (post extinction has the highest priority, which allows the first globe cutscene to be skipped)
	if (config->is("s0E_return_cave")) {
		if (mIsPostExtinct) {
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("s13_pikmin_supply", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}

		// @intns: only remaining regswaps are in this loop - Piki* piki should load into r25 not r28.
		Iterator<Piki> iterator(pikiMgr);
		CI_LOOP(iterator)
		{
			FakePiki* piki = *iterator;
			Vector3f pos   = piki->getPosition();
			pos.y          = mapMgr->getMinY(pos);
			piki->setPosition(pos, false);
		}

		if (!playData->isDemoFlag(DEMO_First_Cave_Return)) {
			playData->setDemoFlag(DEMO_First_Cave_Return);
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("g08_first_return", name, game->mMovieFinishCallback, 0);
			moviePlayArg.mDelegateStart = game->mMovieStartCallback;
			moviePlayer->play(moviePlayArg);
			return;
		}

		if (!playData->isDemoFlag(DEMO_First_Globe_Day_End) && playData->mOlimarData[0].hasItem(OlimarData::ODII_SphericalAtlas)) {
			playData->setDemoFlag(DEMO_First_Globe_Day_End);
			char* name = const_cast<char*>(game->mCurrentCourseInfo->mName);
			MoviePlayArg moviePlayArg("g32_get_map", name, game->mMovieFinishCallback, 0);
			moviePlayer->play(moviePlayArg);
			return;
		}

		if (check_DemoInout(game)) {
			return;
		}

		gameStart(game);
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		return;
	}

	// After one of the three "pikmin are in onion/ship" cutscenes
	if (config->is("g26_inout_black") || config->is("g29_inout_white") || config->is("g2C_inout_red")) {
		gameStart(game);
		og::Screen::DispMemberGround disp;
		Screen::gGame2DMgr->open_GameGround(disp);
		return;
	}

	if (config->is("s03_orimadown")) {
		Screen::gGame2DMgr->close_GameOver();
		naviMgr->getAt(id)->setDeadLaydown();
		if (naviMgr->mDeadNavis != 2) {
			if ((int)id == NAVIID_Olimar) {
				gameSystem->mSection->setPlayerMode(1);
			} else {
				gameSystem->mSection->setPlayerMode(0);
			}
		} else {
			gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
			DayEndArg arg(DayEndState::DETYPE_CaptainsDown);
			transit(game, SGS_DayEnd, &arg);
		}
		return;
	}

	if (config->is("s05_pikminzero")) {
		Screen::gGame2DMgr->close_GameOver();
		gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		DayEndArg arg(DayEndState::DETYPE_PikminZero);
		transit(game, SGS_DayEnd, &arg);
		return;
	}
}

/**
 * @note Address: 0x8021725C
 * @note Size: 0x74
 */
bool GameState::needRepayDemo()
{
	if (mCheckRepay || gameSystem->paused()) { // @P2GZ: return false if game is paused
		return false;
	}

	bool result = false;
	playData->getRepayLevel();
	if (playData->checkRepayLevelFirstClear()) {
		result = true;
	}

	if (playData->isCompletePelletTrigger()) {
		return true;
	}

	return result;
}

/**
 * @note Address: 0x802172D0
 * @note Size: 0x140
 */
void GameState::startRepayDemo()
{
	if (!mCheckRepay) {
		mCheckRepay = true;
		gameSystem->setPause(true, "start-repay", 3);
		gameSystem->setMoviePause(true, "start-repay");
		mRepayTimer = 5.0f;

		int level = playData->getRepayLevel();
		f32 rate  = playData->getRepayLevelPercent(level);
		if (playData->isCompletePelletTrigger()) {
			kh::Screen::DispPayDept disp(kh::Screen::DispPayDept::PAYDEPT_Complete, 0);
			Screen::gGame2DMgr->open_PayDept(disp);
			gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		} else if (level < 9) {
			kh::Screen::DispPayDept disp(kh::Screen::DispPayDept::PAYDEPT_Percent, rate);
			Screen::gGame2DMgr->open_PayDept(disp);
		} else {
			kh::Screen::DispPayDept disp(kh::Screen::DispPayDept::PAYDEPT_DebtPayed, 0);
			Screen::gGame2DMgr->open_PayDept(disp);
			gameSystem->resetFlag(GAMESYS_IsGameWorldActive);
		}
		PSPause_StartMenuOn();
	}
}

/**
 * @note Address: 0x80217410
 * @note Size: 0xA8
 */
GameState::RepayDemoState GameState::updateRepayDemo()
{
	if (mCheckRepay) {
		if ((u8)Screen::gGame2DMgr->check_PayDept()) {
			gameSystem->setMoviePause(false, "check-repay");
			mCheckRepay = false;
			if (playData->isCompletePelletTrigger()) {
				return RDS_GameComplete;
			}
			playData->experienceRepayLevelFirstClear();
			if (playData->getRepayLevel() >= 9) {
				return RDS_GoToPayDebt;
			}
			return RDS_DemoPlaying;
		}
		return RDS_Started;
	}
	return RDS_Inactive;
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void GameState::drawRepayDemo(Graphics&)
{
	// UNUSED FUNCTION
}

// @P2GZ
void GameState::drawTimer() {
	s64 currentTime = OSTicksToMilliseconds(OSGetTime());
	s64 timerMs = currentTime - p2gz->history->peek()->startTime;

    Graphics* gfx = sys->getGfx();
    gfx->initPerspPrintf(gfx->mCurrentViewport);
    gfx->initPrimDraw(nullptr);
    gfx->mOrthoGraph.setPort();

    J2DPrint timerText(gP2JMEMgr->mFont, 0.0f);
    timerText.initiate();
    timerText.mCharColor.set(JUtility::TColor(255, 255, 255, 128));
    timerText.mGradientColor.set(JUtility::TColor(255, 255, 255, 128));
    timerText.mGlyphWidth = 16.0f;
    timerText.mGlyphHeight = 16.0f;

	s64 minutes = timerMs / (60 * 1000);
	s64 seconds = (timerMs / 1000) % 60;
	s64 tenths = (timerMs / 100) % 10;
    timerText.print(16, 16, "%lld:%.2lld.%.1lld", minutes, seconds, tenths);
}

/**
 * @note Address: 0x802174B8
 * @note Size: 0x78
 */
void GameState::draw(SingleGameSection* game, Graphics& gfx)
{
	if (mDoExit) {
		return;
	}

	if (mInSaveScreen) {
		game->draw_Ogawa2D(gfx);
		return;
	}

	game->BaseGameSection::doDraw(gfx);
	game->drawMainMapScreen();
	game->test_draw_treasure_detector();

	drawTimer(); // @P2GZ
}

/**
 * @note Address: 0x80217530
 * @note Size: 0x68
 */
void GameState::cleanup(SingleGameSection* game)
{
	gameSystem->resetFlag(GAMESYS_IsPlaying);
	gameSystem->setMoviePause(false, "gamestate::cleanup");
	if (game->mTheExpHeap) {
		PSMCancelToPauseOffMainBgm();
	}
	game->setDraw2DCreature(nullptr);
}

} // namespace SingleGame
} // namespace Game
