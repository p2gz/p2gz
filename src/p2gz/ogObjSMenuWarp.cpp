#include "Game/Entities/ItemCave.h"
#include "Game/Entities/PelletCarcass.h"
#include "Game/Entities/PelletFruit.h"
#include "Game/Entities/PelletItem.h"
#include "Game/Entities/PelletOtakara.h"
#include "Game/gamePlayData.h"
#include "Game/generalEnemyMgr.h"
#include "Game/MapMgr.h"
#include "Game/MoviePlayer.h"
#include "Game/NaviState.h"
#include "Game/PikiState.h"
#include "Game/SingleGame.h"
#include "og/newScreen/SMenu.h"
#include "og/Screen/anime.h"
#include "og/Screen/ogScreen.h"
#include "og/Sound.h"
#include "utilityU.h"
#include "GlobalData.h"

namespace og {
namespace newScreen {

static void _Print(char* format, ...) { OSReport(format, __FILE__); }

static u64 areaIDs[4] = { '8390_00', '8391_00', '8392_00', '8393_00' }; // Valley of Repose, Awakening Wood, Perplexing Pool, Wistful Wild

static u64 warpIDs[4][5] = {
	{ '3016_00', '8395_00', '8399_00', '8400_00', 'NULL' },    // Above Ground, Emergence Cave, Subterranean Complex, Frontier Cavern
	{ '3016_00', '8396_00', '8398_00', '8401_00', '8410_00' }, // Above Ground, Hole of Beasts, White Flower Garden, Bulblax Kingdom, Snagret Hole
	{ '3016_00', '8397_00', '8402_00', '8403_00', '8411_00' }, // Above Ground, Citadel of Spiders, Glutton's Kitchen, Shower Room, Submerged Castle
	{ '3016_00', '8412_00', '8413_00', '8414_00', 'NULL' }     // Above Ground, Cavern of Chaos, Hole of Heroes, Dream Den
};

static u64 caveIDs[4][5] = {
	{ 'NULL', 't_01', 't_02', 't_03', 'NULL' },
	{ 'NULL', 'f_01', 'f_02', 'f_03', 'f_04' },
	{ 'NULL', 'y_01', 'y_02', 'y_03', 'y_04' },
	{ 'NULL', 'l_01', 'l_02', 'l_03', 'NULL' }
};

static int maxSublevel[4][5] = {
	{ 99, 2, 9, 8, -1 },
	{ 99, 5, 5, 7, 7 },
	{ 99, 5, 6, 7, 5 },
	{ 99, 10, 15, 14, -1 }
};

static u64 sublevelID = '8382_00'; // Sublevel
static u64 dayID = '5040_00'; // Day

ObjSMenuWarp::ObjSMenuWarp(char const* name)
{
	mDisp       = nullptr;
	mScreenWarp = nullptr;
	mAnimGroup  = nullptr;
	mName       = name;

	mAreaLabel = nullptr;
	mAreaName = nullptr;
	mSelectedArea = p2gz->mSelectedArea;

	mDestinationLabel = nullptr;
	mDestinationName = nullptr;
	mSelectedDestination = p2gz->mSelectedDestination;

	mSublevelLabel = nullptr;
	mSublevelCounter = nullptr;
	mSublevelNumber = p2gz->mSublevelNumber;

	mPresetLabel = nullptr;
	mPresetName = nullptr;
	mSelectedPresetIndex = 0;

	mGoText = nullptr;

	mIsEditingSetting = false;
	mSelectedRow = 0;
}

ObjSMenuWarp::~ObjSMenuWarp() { }

void ObjSMenuWarp::doCreate(JKRArchive* arc)
{
	og::Screen::DispMemberSMenuAll* dispfull = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	mDisp = static_cast<og::Screen::DispMemberSMenuWarp*>(dispfull->getSubMember(OWNER_OGA, MEMBER_P2GZ_WARP));
	if (!mDisp) {
		og::Screen::DispMemberSMenuAll* newdisp = new og::Screen::DispMemberSMenuAll;
		mDisp = static_cast<og::Screen::DispMemberSMenuWarp*>(newdisp->getSubMember(OWNER_OGA, MEMBER_P2GZ_WARP));
	}

	mScreenWarp = new P2DScreen::Mgr_tuning;
	mScreenWarp->set("s_menu_warp.blo", 0x1040000, arc);

	mAnimGroup = new og::Screen::AnimGroup(2);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenWarp, "s_menu_controller.btk", msBaseVal.mAnimSpeed);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenWarp, "s_menu_controller_02.btk", msBaseVal.mAnimSpeed);

	mAreaLabel = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tarea'));
	mAreaName = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tarea_n'));

	mDestinationLabel = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tdest'));
	mDestinationName = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tdest_n'));

	mSublevelLabel = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tsub'));
	mSublevelCounter = og::Screen::setCallBack_CounterRV(mScreenWarp, 'Tsub01', &mSublevelNumber, 2, false, true, arc);
	mSublevelNumber = p2gz->mSublevelNumber = mSublevelNumber;

	mPresetLabel = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tpreset'));
	mPresetName = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tprese_n'));
	mSelectedPresetIndex = p2gz->mSelectedPresetIndex;

	mGoText = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenWarp, 'Tgo'));

	mIsEditingSetting = false;
	mSelectedRow = 0;

	mLabels[0] = mAreaLabel;
	mLabels[1] = mDestinationLabel;
	mLabels[2] = mSublevelLabel;
	mLabels[3] = mPresetLabel;
	mLabels[4] = mGoText;

	mSettings[0] = mAreaName;
	mSettings[1] = mDestinationName;
	mSettings[2] = nullptr;
	mSettings[3] = mPresetName;
	mSettings[4] = nullptr;

	mAreaName->setAlpha(128);
	mDestinationLabel->setAlpha(128);
	mDestinationName->setAlpha(128);
	mSublevelLabel->setAlpha(128);
	mSublevelCounter->getMotherPane()->setAlpha(128);
	mPresetLabel->setAlpha(128);
	mPresetName->setAlpha(128);
	mGoText->setAlpha(128);

	mAreaName->setMsgID(areaIDs[mSelectedArea]);
	mDestinationName->setMsgID(warpIDs[mSelectedArea][mSelectedDestination]);
	mPresetName->setMsgID(p2gz->mPresets[mSelectedPresetIndex].mMsgId);
	mLabels[2]->setMsgID(mSelectedDestination == 0 ? dayID : sublevelID);

	doCreateAfter(arc, mScreenWarp);
}

void ObjSMenuWarp::doUpdateLAction()
{
	// LEFT = PAUSE MENU
	og::Screen::DispMemberSMenuAll* disp = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	if (disp->mSMenuMap.mInCave) {
		::Screen::SetSceneArg arg(SCENE_PAUSE_MENU_DOUKUTU, getDispMember());
		jump_L(arg);
	} else {
		::Screen::SetSceneArg arg(SCENE_PAUSE_MENU, getDispMember());
		jump_L(arg);
	}
}

void ObjSMenuWarp::doUpdateRAction()
{
	// RIGHT = SQUAD
	::Screen::SetSceneArg arg(SCENE_P2GZ_SQUAD, getDispMember());
	jump_R(arg);
}

void ObjSMenuWarp::commonUpdate()
{
	commonUpdateBase();
	setSMenuScale(msVal.mScaleX, msVal.mScaleY);
	mAnimGroup->update();
	mScreenWarp->setXY(mMovePos, 0.0f);
	mScreenWarp->update();
}

// @P2GZ
void ObjSMenuWarp::updatePreset() {
	s64 defaultPresetId = p2gz->getDefaultPresetId(mSelectedArea, mSelectedDestination, mSublevelNumber);
	for (size_t i = 0; i < p2gz->mPresets.len(); i++) {
		if (p2gz->mPresets[i].mMsgId == defaultPresetId) {
			mSelectedPresetIndex = i;
			p2gz->mSelectedPresetIndex = i;
			break;
		}
	}
	mPresetName->setMsgID(defaultPresetId);
}

bool ObjSMenuWarp::doUpdate()
{
	commonUpdate();

	Controller* pad = getGamePad();
	u32 input = pad->getButtonDown();

	if (input & Controller::PRESS_UP) {
		if (mIsEditingSetting) {
			if (mSelectedRow == 2) {
				if (mSublevelNumber == maxSublevel[mSelectedArea][mSelectedDestination]) {
					ogSound->setError();
				} else {
					mSublevelNumber++;
					p2gz->mSublevelNumber = mSublevelNumber;
					mSublevelCounter->update();
					updatePreset();
				}
			}
		} else {
			mLabels[mSelectedRow]->setAlpha(128);
			mLabels[mSelectedRow]->update();
			mSelectedRow = (mSelectedRow - 1 + 5) % 5;
			mLabels[mSelectedRow]->setAlpha(255);
			mLabels[mSelectedRow]->update();
			ogSound->setPlusMinus(false);
		}
	} else if (input & Controller::PRESS_DOWN) {
		if (mIsEditingSetting) {
			if (mSelectedRow == 2) {
				if (mSublevelNumber == 1) {
					ogSound->setError();
				} else {
					mSublevelNumber--;
					p2gz->mSublevelNumber = mSublevelNumber;
					mSublevelCounter->update();
					updatePreset();
				}
			}
		} else {
			mLabels[mSelectedRow]->setAlpha(128);
			mLabels[mSelectedRow]->update();
			mSelectedRow = (mSelectedRow + 1) % 5;
			mLabels[mSelectedRow]->setAlpha(255);
			mLabels[mSelectedRow]->update();
			ogSound->setPlusMinus(false);
		}
	} else if (input & Controller::PRESS_LEFT) {
		if (mIsEditingSetting) {
			switch (mSelectedRow) {
				case 0:
				mSelectedArea = (mSelectedArea - 1 + 4) % 4;
				p2gz->mSelectedArea = mSelectedArea;
				mAreaName->setMsgID(areaIDs[mSelectedArea]);
				mDestinationName->setMsgID(warpIDs[mSelectedArea][0]);
				mSelectedDestination = 0;
				p2gz->mSelectedDestination = 0;
				mLabels[2]->setMsgID(dayID);
				ogSound->setPlusMinus(false);
				updatePreset();
				break;

				case 1:
				if (mSelectedDestination == 0) {
					mLabels[2]->setMsgID(sublevelID);
				}
				int numDestinations = (mSelectedArea == 0 || mSelectedArea == 3) ? 4 : 5;
				mSelectedDestination = (mSelectedDestination - 1 + numDestinations) % numDestinations;
				p2gz->mSelectedDestination = mSelectedDestination;
				mDestinationName->setMsgID(warpIDs[mSelectedArea][mSelectedDestination]);
				if (mSelectedDestination == 0) {
					mLabels[2]->setMsgID(dayID);
				}
				if (mSublevelNumber > maxSublevel[mSelectedArea][mSelectedDestination]) {
					mSublevelNumber = maxSublevel[mSelectedArea][mSelectedDestination];
					p2gz->mSublevelNumber = mSublevelNumber;
					mSublevelCounter->update();
				}
				ogSound->setPlusMinus(false);
				updatePreset();
				break;

				case 3:
				int numPresets = p2gz->mPresets.len();
				mSelectedPresetIndex = (mSelectedPresetIndex - 1 + numPresets) % numPresets;
				p2gz->mSelectedPresetIndex = mSelectedPresetIndex;
				mPresetName->setMsgID(p2gz->mPresets[mSelectedPresetIndex].mMsgId);
				ogSound->setPlusMinus(false);
				break;
			}
		}
	} else if (input & Controller::PRESS_RIGHT) {
		if (mIsEditingSetting) {
			switch (mSelectedRow) {
				case 0:
				mSelectedArea = (mSelectedArea + 1) % 4;
				p2gz->mSelectedArea = mSelectedArea;
				mAreaName->setMsgID(areaIDs[mSelectedArea]);
				mDestinationName->setMsgID(warpIDs[mSelectedArea][0]);
				mSelectedDestination = 0;
				p2gz->mSelectedDestination = 0;
				mLabels[2]->setMsgID(dayID);
				ogSound->setPlusMinus(false);
				updatePreset();
				break;

				case 1:
				if (mSelectedDestination == 0) {
					mLabels[2]->setMsgID(sublevelID);
				}
				int numDestinations = (mSelectedArea == 0 || mSelectedArea == 3) ? 4 : 5;
				mSelectedDestination = (mSelectedDestination + 1) % numDestinations;
				p2gz->mSelectedDestination = mSelectedDestination;
				mDestinationName->setMsgID(warpIDs[mSelectedArea][mSelectedDestination]);
				if (mSelectedDestination == 0) {
					mLabels[2]->setMsgID(dayID);
				}
				if (mSublevelNumber > maxSublevel[mSelectedArea][mSelectedDestination]) {
					mSublevelNumber = maxSublevel[mSelectedArea][mSelectedDestination];
					p2gz->mSublevelNumber = mSublevelNumber;
					mSublevelCounter->update();
				}
				ogSound->setPlusMinus(false);
				updatePreset();
				break;

				case 3:
				mSelectedPresetIndex = (mSelectedPresetIndex + 1) % p2gz->mPresets.len();
				p2gz->mSelectedPresetIndex = mSelectedPresetIndex;
				mPresetName->setMsgID(p2gz->mPresets[mSelectedPresetIndex].mMsgId);
				ogSound->setPlusMinus(false);
				break;
			}
		}
	} else if (input & Controller::PRESS_A) {
		if (mSelectedRow == 4) {
			Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);

			if (mSelectedDestination == 0) {
				p2gz->warpToSelectedArea();
			} else {
				p2gz->warpToSelectedCave(nullptr);
			}
			return true;
		}

		if (mIsEditingSetting) {
			mIsEditingSetting = false;
			if (mSelectedRow == 2) {
				mSublevelCounter->getMotherPane()->setAlpha(128);
				mSublevelCounter->update();
			} else {
				mSettings[mSelectedRow]->setAlpha(128);
				mSettings[mSelectedRow]->update();
			}
			ogSound->setDecide();
		} else {
			mIsEditingSetting = true;
			if (mSelectedRow == 2) {
				mSublevelCounter->getMotherPane()->setAlpha(255);
				mSublevelCounter->update();
			} else {
				mSettings[mSelectedRow]->setAlpha(255);
				mSettings[mSelectedRow]->update();
			}
			ogSound->setOpen();
		}
	} else if (input & Controller::PRESS_B) {
		if (mIsEditingSetting) {
			mIsEditingSetting = false;
			if (mSelectedRow == 2) {
				mSublevelCounter->getMotherPane()->setAlpha(128);
				mSublevelCounter->update();
			} else {
				mSettings[mSelectedRow]->setAlpha(128);
				mSettings[mSelectedRow]->update();
			}
			ogSound->setDecide();

			mScreenWarp->animation();
			return false;
		}
	}

	bool ret = ObjSMenuBase::doUpdate();
	mScreenWarp->animation();
	return ret;
}

void ObjSMenuWarp::doDraw(Graphics& gfx)
{
	J2DPerspGraph* graf = &gfx.mPerspGraph;
	graf->setPort();
	if (mScreenWarp) {
		mScreenWarp->draw(gfx, *graf);
	}
	drawYaji(gfx);
}

void ObjSMenuWarp::in_L()
{
	mState = MENUSTATE_OpenL;
	mAngle = 15.0;
}

void ObjSMenuWarp::in_R()
{
	mState = MENUSTATE_OpenR;
	mAngle = 15.0;
}

void ObjSMenuWarp::wait() { mState = MENUSTATE_Default; }

void ObjSMenuWarp::out_L()
{
	mState = MENUSTATE_CloseL;
	ogSound->setSMenuLR();
}

void ObjSMenuWarp::out_R()
{
	mState = MENUSTATE_CloseR;
	ogSound->setSMenuLR();
}

bool ObjSMenuWarp::doStart(::Screen::StartSceneArg const* arg)
{
	mAnimGroup->setFrame(0.0f);
	mAnimGroup->setRepeat(true);
	mAnimGroup->setSpeed(1.0f);
	mAnimGroup->start();
	setYajiName('6052_00', '3003_00', '3013_00'); // @P2GZ: change to "Menu" "Squad" "Warp"
	stopYaji();
	return start_LR(arg);
}

bool ObjSMenuWarp::doEnd(::Screen::EndSceneArg const*) { return true; }

void ObjSMenuWarp::doUpdateFinish() { ObjSMenuBase::doUpdateFinish(); }

bool ObjSMenuWarp::doUpdateFadeout()
{
	commonUpdate();
	return updateFadeOut();
}

void ObjSMenuWarp::doUpdateCancelAction() { }

ObjSMenuWarp::StaticValues ObjSMenuWarp::msVal;

} // namespace newScreen
} // namespace og
