#include "GlobalData.h"
#include "utilityU.h"
#include "DefaultPresets.h"
#include "GZMacros.h"
#include "Game/Piki.h"
#include "Game/PikiMgr.h"
#include "PikiAI.h"
#include "Game/PikiState.h"
#include "Game/MapMgr.h"
#include "Game/NaviState.h"
#include "Game/SingleGame.h"
#include "Game/MoviePlayer.h"
#include "Game/generalEnemyMgr.h"
#include "Game/Entities/ItemCave.h"
#include "Game/Entities/PelletCarcass.h"
#include "Game/Entities/PelletFruit.h"
#include "Game/Entities/PelletItem.h"
#include "Game/Entities/PelletOtakara.h"
#include "JSystem/J2D/J2DPrint.h"

using namespace Game;

P2GZ* p2gz;

P2GZ::P2GZ()
{
    mIsScrollingCamera = false;
    mIsSaveLoadPosition = false;
    mScrollCameraZoom = 768;

    mSelectedPositionSlot = 0;
    for (int i = 0; i < 4; i++) {
        mSavedPositions[i] = Vector3f::zero;
    }
    mControlStickPicture = nullptr;
    mCStickPicture = nullptr;
    mAButtonPicture = nullptr;
    mBButtonPicture = nullptr;
    mXButtonPicture = nullptr;
    mYButtonPicture = nullptr;
    mLButtonPicture = nullptr;
    mRButtonPicture = nullptr;
    mZButtonPicture = nullptr;

    mAnimationCoefficient = 0.0f;
    mDirection = 1.0f;

    mSetCustomNextSeed = false;
    mNextSeed = 0;
    mHistory = new gzCollections::RingBuffer<64, SegmentRecord>;
    mBugPokosCollectedSinceLoad = 0;
    mTreasurePokosCollectedSinceLoad = 0;

    mSelectedArea = 0;
    mSelectedDestination = 0;
    mSublevelNumber = 1;
    mSelectedPresetIndex = 0;

    mShowTimer = true;
    mCaveStartTimeMs = 0;

    mPresets = getDefaultPresets();
}

void P2GZ::init()
{
    JKRArchive* arc = JKRMountArchive("/new_screen/eng/res_s_menu_controller.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);

    ResTIMG* controlStickTimg = JKRGetImageResource("c_anim_otah_32.9.bti", arc);
    ResTIMG* cStickTimg = JKRGetImageResource("c_stick_anim_otah_32.5.bti", arc);
    ResTIMG* aButtonTimg = JKRGetImageResource("ia4_abtn.bti", arc);
    ResTIMG* bButtonTimg = JKRGetImageResource("ia4_bbtn.bti", arc);
    ResTIMG* xButtonTimg = JKRGetImageResource("x_btn.bti", arc);
    ResTIMG* yButtonTimg = JKRGetImageResource("y_btn.bti", arc);
    ResTIMG* lButtonTimg = JKRGetImageResource("l_btn.bti", arc);
    ResTIMG* rButtonTimg = JKRGetImageResource("r_btn.bti", arc);
    ResTIMG* zButtonTimg = JKRGetImageResource("z_btn.bti", arc);

    mControlStickPicture = new J2DPicture(new JUTTexture(controlStickTimg));
    mCStickPicture = new J2DPicture(new JUTTexture(cStickTimg));
    mAButtonPicture = new J2DPicture(new JUTTexture(aButtonTimg));
    mBButtonPicture = new J2DPicture(new JUTTexture(bButtonTimg));
    mXButtonPicture = new J2DPicture(new JUTTexture(xButtonTimg));
    mYButtonPicture = new J2DPicture(new JUTTexture(yButtonTimg));
    mLButtonPicture = new J2DPicture(new JUTTexture(lButtonTimg));
    mRButtonPicture = new J2DPicture(new JUTTexture(rButtonTimg));
    mZButtonPicture = new J2DPicture(new JUTTexture(zButtonTimg));
}

void P2GZ::update()
{
    if (mAnimationCoefficient >= 1.0f) {
        mDirection = -1.0f;
    } else if (mAnimationCoefficient <= 0.0f) {
        mDirection = 1.0f;
    }
    mAnimationCoefficient += mDirection / 30;
}

f32 P2GZ::getAnimationCoefficient()
{
    return mAnimationCoefficient;
}

void P2GZ::warpToSelectedCave(PikiContainer* squad) {
    Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
    Iterator<Game::Piki> iterator(Game::pikiMgr);
    CI_LOOP(iterator)
    {
        Game::Piki* piki = *iterator;
        if (piki->isAlive() && piki->getCurrActionID() == PikiAI::ACT_Formation) {
            int state = piki->getStateID();
            if (state != Game::PIKISTATE_Flying && state != Game::PIKISTATE_HipDrop && piki->mNavi
                //    Check if we're in SmC
                && (!(mSelectedArea == 2 && mSelectedDestination != 3) || piki->getKind() == Game::Blue)) {
                Game::playData->mCaveSaveData.mCavePikis(piki)++;
            }
        }
    }

    ID32 caveID(Game::stageList->getCourseInfo(mSelectedArea)->getCaveID_FromIndex(mSelectedDestination - 1));
    Game::ItemCave::Item* cave = new Game::ItemCave::Item;
    cave->mCaveID = caveID;
    cave->mCaveFilename = Game::stageList->getCourseInfo(mSelectedArea)->getCaveinfoFilename_FromID(caveID);

    Game::playData->mCaveSaveData.mTime = Game::gameSystem->mTimeMgr->mCurrentTimeOfDay;
    Game::playData->mCaveSaveData.mCourseIdx = Game::stageList->getCourseInfo(mSelectedArea)->mCourseIndex;
    Game::playData->mCaveSaveData.mCurrentCaveID = caveID;

    if (!Game::gameSystem->mIsInCave) {
        game->saveToGeneratorCache(game->mCurrentCourseInfo);
    }

    game->mCurrentCourseInfo = Game::stageList->getCourseInfo(mSelectedArea);
    game->mCurrentCave = cave;
    game->mCaveID = caveID;
    game->mCaveIndex = caveID.getID();
    game->mCurrentFloor = mSublevelNumber - 1;
    strcpy(game->mCaveFilename, cave->mCaveFilename);

    if (squad != nullptr) {
        Preset preset = mPresets[mSelectedPresetIndex];
        preset.mSquad = *squad;
        applyPreset(preset);
    }
    else {
        applyPreset(mPresets[mSelectedPresetIndex]);
    }

    Game::SingleGame::LoadArg arg(100, true, false, false);
    game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

void P2GZ::warpToSelectedArea() {
    Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);

    // TODO: Probably not all of this is necessary - copy-paste from DayEndState::exec()
    Game::gameSystem->resetFlag(Game::GAMESYS_IsGameWorldActive);
    Game::gameSystem->setFlag(Game::GAMESYS_DisableDeathCounter);
    Game::moviePlayer->reset();
    Game::moviePlayer->clearSuspendedDemo();

    if (game->mTheExpHeap != nullptr) {
        PSMCancelToPauseOffMainBgm();
    }

    Iterator<Game::Onyon> iOnyon(Game::ItemOnyon::mgr);
    CI_LOOP(iOnyon)
    {
        (*iOnyon)->setSpotEffectActive(false);
        (*iOnyon)->mSuckTimer = 4.0f;
        (*iOnyon)->forceClose();
    }

    if (!Game::gameSystem->mIsInCave) {
        game->saveToGeneratorCache(game->mCurrentCourseInfo);
    }

    Game::PelletIterator iPellet;
    CI_LOOP(iPellet)
    {
        Game::Pellet* pellet = *iPellet;
        if (pellet->isAlive() && pellet->mCaptureMatrix == nullptr) {
            pellet->kill(nullptr);
        }
    }

    Game::PelletCarcass::mgr->resetMgr();
    Game::PelletFruit::mgr->resetMgr();
    Game::PelletItem::mgr->resetMgrAndResources();
    Game::PelletOtakara::mgr->resetMgrAndResources();

    Game::Navi* navi = Game::naviMgr->getAt(NAVIID_Olimar);
    if (navi->isAlive()) {
        navi->mFsm->transit(navi, Game::NSID_Walk, nullptr);
        efx::TNaviEffect* effectsObj = navi->mEffectsObj;
        effectsObj->mFlags.unset(efx::NAVIFX_InWater);
        effectsObj->killHamonA_();
        effectsObj->killHamonB_();
    }

    navi = Game::naviMgr->getAt(NAVIID_Louie);
    if (navi->isAlive()) {
        navi->mFsm->transit(navi, Game::NSID_Walk, nullptr);
        efx::TNaviEffect* effectsObj = navi->mEffectsObj;
        effectsObj->mFlags.unset(efx::NAVIFX_InWater);
        effectsObj->killHamonA_();
        effectsObj->killHamonB_();
    }

    Game::pikiMgr->forceEnterPikmins(false);
    Game::gameSystem->mTimeMgr->setStartTime();

    if (mSublevelNumber % 30 == 0) {
        for (int i = 0; i < 4; i++) {
            Game::playData->mLimitGen[i].mLoops.all_zero();
        }
    }

    Game::gameSystem->mTimeMgr->mDayCount = mSublevelNumber;
    Game::gameSystem->detachObjectMgr(generalEnemyMgr);
    Game::gameSystem->detachObjectMgr(mapMgr);

    game->mIsGameStarted = false;
    game->mCurrentCourseInfo = Game::stageList->getCourseInfo(mSelectedArea);

    applyPreset(mPresets[mSelectedPresetIndex]);

    Game::SingleGame::LoadArg arg(0, false, false, false);
    game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

Preset& P2GZ::getPresetByMsgId(s64 msgId) {
    for (size_t i = 0; i < mPresets.len(); i++) {
        Preset& preset = mPresets[i];
        if (preset.mMsgId == msgId) return preset;
    }
    GZASSERTLINE(0);
}

s64 P2GZ::getDefaultPresetId(int area, int destination, int sublevel) {
    switch (area) {
    case 0: // VoR
        switch (destination) {
        case 0: // AG
            if (sublevel < 7) return '3215_00';
            else return '3216_00';
        case 1: // EC
            return '3200_00';
        case 2: // SCx
            if (sublevel < 5) return '3217_00';
            else return '3219_00';
        case 3: // FC
            return '3221_00';
        }
    case 1: // AW
        switch (destination) {
        case 0: // AG
            return '3209_00'; // hardcoded for Enter BK
        case 1: // HoB
            if (sublevel < 3) return '3202_00';
            else if (sublevel < 5) return '3203_00';
            else return '3204_00';
        case 2: // WFG
            if (sublevel < 4) return '3204_00';
            else return '3205_00';
        case 3: // BK
            return '3213_00';
        case 4: // SH
            if (sublevel < 3) return '3206_00';
            else return '3207_00';
        }
    case 2: // PP
        switch (destination) {
        case 0: // AG
            return '3223_00';
        case 1: // CoS
        case 2: // GK
            return '3224_00';
        }
    }

    return '3299_00'; // testing preset
}

void P2GZ::applyPreset(Preset& preset) {
    // GameStat::mePikis.clear(); // clear sprouts
    playData->mCaveSaveData.mCavePikis = preset.mSquad;
    playData->mPikiContainer = preset.mOnionPikis;

    playData->mSprayCount[0] = preset.mNumSpicies;
    playData->mSprayCount[1] = preset.mNumBitters;

    // Set flags for having each pikmin type's onion
    for (int pikiColor = White; pikiColor >= 0; pikiColor--) {
        if (preset.mSquad.getColorSum(pikiColor) > 0 || preset.mOnionPikis.getColorSum(pikiColor) > 0) {
            if (pikiColor <= LastOnyon) {
                playData->setBootContainer(pikiColor);
            }
            playData->setContainer(pikiColor);
            playData->setMeetPikmin(pikiColor);
        }
    }

    // Set cutscene flags
    for (size_t i = 0; i < preset.mCutsceneFlags.len(); i++) {
        playData->mDemoFlags.setFlag(preset.mCutsceneFlags[i]);
    }
}

Preset::Preset() {
    mNumBitters = 0;
    mNumSpicies = 0;
}

Preset& Preset::setMsgId(s64 msgId) {
    mMsgId = msgId;
    return *this;
}

Preset& Preset::setPikmin(int happa, int color, int amount) {
    mSquad.getCount(color, happa) += amount;
    return *this;
}

Preset& Preset::setOnionPikmin(int happa, int color, int amount) {
    mOnionPikis.getCount(color, happa) += amount;
    return *this;
}

Preset& Preset::setSprays(int spicies, int bitters) {
    mNumSpicies = spicies;
    mNumBitters = bitters;
    return *this;
}

Preset& Preset::addCutsceneFlags(u16 flags[], size_t numFlags) {
    mCutsceneFlags.expandCapacityTo(mCutsceneFlags.len() + numFlags);
    for (size_t i = 0; i < numFlags; i++) {
        mCutsceneFlags.push(flags[i]);
    }
    return *this;
}
