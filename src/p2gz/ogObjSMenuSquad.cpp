#include "Game/gamePlayData.h"
#include "og/newScreen/SMenu.h"
#include "og/Screen/anime.h"
#include "og/Screen/ogScreen.h"
#include "og/Sound.h"

namespace og {
namespace newScreen {

static void _Print(char* format, ...) { OSReport(format, __FILE__); }

static const Game::EPikiKind kindOrder[] = { Game::Red, Game::Yellow, Game::Blue, Game::Purple, Game::White };

static const Game::EPikiHappa happaOrder[] = { Game::Flower, Game::Bud, Game::Leaf };

static u64 iconTags[5][3] = {
	{ 'red_f', 'red_b', 'red_l' },
	{ 'yel_f', 'yel_b', 'yel_l' },
	{ 'blu_f', 'blu_b', 'blu_l' },
	{ 'pur_f', 'pur_b', 'pur_l' },
	{ 'whi_f', 'whi_b', 'whi_l' }
};

static u64 counterTags[5][3] = { 
	{ 'red_f1', 'red_b1', 'red_l1' },
	{ 'yel_f1', 'yel_b1', 'yel_l1' },
	{ 'blu_f1', 'blu_b1', 'blu_l1' },
	{ 'pur_f1', 'pur_b1', 'pur_l1' },
	{ 'whi_f1', 'whi_b1', 'whi_l1' }
};

ObjSMenuSquad::ObjSMenuSquad(char const* name)
{
	mDisp       = nullptr;
	mScreenSquad = nullptr;
	mAnimGroup  = nullptr;
	mName       = name;

	mRow = 0;
	mCol = 0;
	mNumRows = 5;
	mNumCols = 3;
	mSelectedDigit = 0;
	mIsEditingPikminCount = false;
}

ObjSMenuSquad::~ObjSMenuSquad() { }

void ObjSMenuSquad::doCreate(JKRArchive* arc)
{
	og::Screen::DispMemberSMenuAll* dispfull = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	mDisp = static_cast<og::Screen::DispMemberSMenuSquad*>(dispfull->getSubMember(OWNER_OGA, MEMBER_P2GZ_SQUAD));
	if (!mDisp) {
		og::Screen::DispMemberSMenuAll* newdisp = new og::Screen::DispMemberSMenuAll;
		mDisp = static_cast<og::Screen::DispMemberSMenuSquad*>(newdisp->getSubMember(OWNER_OGA, MEMBER_P2GZ_SQUAD));
	}

	mScreenSquad = new P2DScreen::Mgr_tuning;
	mScreenSquad->set("s_menu_squad.blo", 0x1040000, arc);

	mAnimGroup = new og::Screen::AnimGroup(2);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenSquad, "s_menu_controller.btk", msBaseVal.mAnimSpeed);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenSquad, "s_menu_controller_02.btk", msBaseVal.mAnimSpeed);

	for (int row = 0; row < mNumRows; row++) {
		for (int col = 0; col < mNumCols; col++) {
			mIcons[row][col] = static_cast<J2DPictureEx*>(og::Screen::TagSearch(mScreenSquad, iconTags[row][col]));
			mIcons[row][col]->setAlpha(128);
			mCounters[row][col] = og::Screen::setCallBack_CounterRV(mScreenSquad, counterTags[row][col], &mPikminCounts[row][col], 3, false, true, arc);
			mCounters[row][col]->mIsP2GZCounter = true;
			for (int i = 0; i < 3; i++) {
				mCounters[row][col]->getKetaPicture(i)->setAlpha(128);
			}
			mCounters[row][col]->update();
			mPikminCounts[row][col] = 0;
		}
	}

	mIcons[0][0]->setAlpha(255);

	for (int i = 0; i < Game::pikiMgr->mMax; i++) {
		if (Game::pikiMgr->mOpenIds[i]) {
			continue;
		}

		Game::Piki* piki = &Game::pikiMgr->mArray[i];
		if (piki->mNavi == nullptr || piki->mPikiKind == Game::Bulbmin) {
			continue;
		}

		int row = 0;
		for (int i = 0; i < mNumRows; i++) {
			if (kindOrder[i] == piki->mPikiKind) {
				row = i;
				break;
			}
		}

		int col = 0;
		for (int i = 0; i < mNumCols; i++) {
			if (happaOrder[i] == piki->mHappaKind) {
				col = i;
				break;
			}
		}

		mPikminCounts[row][col]++;
	}

	doCreateAfter(arc, mScreenSquad);
}

void ObjSMenuSquad::doUpdateLAction()
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

void ObjSMenuSquad::doUpdateRAction()
{
	// RIGHT = COUNTERS
	::Screen::SetSceneArg arg(SCENE_PAUSE_MENU_CONTROLS, getDispMember());
	jump_R(arg);
}

void ObjSMenuSquad::commonUpdate()
{
	commonUpdateBase();
	setSMenuScale(msVal.mScaleX, msVal.mScaleY);
	mAnimGroup->update();
	mScreenSquad->setXY(mMovePos, 0.0f);
	mScreenSquad->update();
}

bool ObjSMenuSquad::doUpdate()
{
	commonUpdate();

	Controller* pad = getGamePad();
	u32 input = pad->getButtonDown();

	if (input & Controller::PRESS_UP) {
		if (!mIsEditingPikminCount) {
			if (mRow == 0) {
				ogSound->setError();
				mScreenSquad->animation();
				return false;
			} else {
				mIcons[mRow][mCol]->setAlpha(128);
				mCounters[mRow][mCol]->update();
				mRow--;
				mIcons[mRow][mCol]->setAlpha(255);
				mCounters[mRow][mCol]->update();
				ogSound->setPlusMinus(false);
			}
		} else {
			if (Game::pikiMgr->mActiveCount + Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount + pow(10, mSelectedDigit) > MAX_PIKI_COUNT) {
				ogSound->setError();
			} else {
				int previousCount = mPikminCounts[mRow][mCol];

				for (int i = 0; i < pow(10, mSelectedDigit); i++) {
					Game::Piki* piki = Game::pikiMgr->birth();

					Game::PikiInitArg arg(-1);
					piki->init(&arg);
					piki->changeShape(kindOrder[mRow]);
					piki->changeHappa(happaOrder[mCol]);

					Vector3f pos = Game::naviMgr->getActiveNavi()->getPosition();
					piki->setPosition(pos, false);

					piki->mNavi = Game::naviMgr->getActiveNavi();
				}

				mPikminCounts[mRow][mCol] += pow(10, mSelectedDigit);
				if (mPikminCounts[mRow][mCol] == pow(10, mSelectedDigit + 1)) {
					mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
					mSelectedDigit++;
					mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(255);
				}
				mCounters[mRow][mCol]->update();

				ogSound->setPlusMinus(false);
			}
		}
	} else if (input & Controller::PRESS_DOWN) {
		if (!mIsEditingPikminCount) {
			if (mRow == mNumRows - 1) {
				ogSound->setError();
				mScreenSquad->animation();
				return false;
			} else {
				mIcons[mRow][mCol]->setAlpha(128);
				mCounters[mRow][mCol]->update();
				mRow++;
				mIcons[mRow][mCol]->setAlpha(255);
				mCounters[mRow][mCol]->update();
				ogSound->setPlusMinus(false);
			}
		} else {
			if (mPikminCounts[mRow][mCol] - pow(10, mSelectedDigit) < 0) {
				ogSound->setError();
			} else {
				int pikminKilled = 0;

				for (int i = 0; i < Game::pikiMgr->mMax; i++) {
					Game::Piki* piki = &Game::pikiMgr->mArray[i];
					if (piki->mNavi != nullptr && piki->mPikiKind == kindOrder[mRow] && piki->mHappaKind == happaOrder[mCol]) {
						Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
						piki->kill(&arg);
						if (++pikminKilled == pow(10, mSelectedDigit)) {
							break;
						}
					}
				}
				
				mPikminCounts[mRow][mCol] -= pow(10, mSelectedDigit);
				if (mPikminCounts[mRow][mCol] < pow(10, mSelectedDigit) && mSelectedDigit > 0) {
					mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
					mSelectedDigit--;
					mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(255);
				}
				mCounters[mRow][mCol]->update();

				ogSound->setPlusMinus(false);
			}
		}
	} else if (input & Controller::PRESS_LEFT) {
		if (!mIsEditingPikminCount) {
			if (mCol == 0) {
				ogSound->setError();
			} else {
				mIcons[mRow][mCol]->setAlpha(128);
				mCol--;
				mIcons[mRow][mCol]->setAlpha(255);
				ogSound->setPlusMinus(false);
			}
		} else {
			if (mSelectedDigit == 2) {
				ogSound->setError();
				mScreenSquad->animation();
				return false;
			} else {
				if (mPikminCounts[mRow][mCol] < pow(10, mSelectedDigit + 1)) {
					if (Game::pikiMgr->mActiveCount + Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount + pow(10, mSelectedDigit + 1) > MAX_PIKI_COUNT) {
						ogSound->setError();
						mScreenSquad->animation();
						return false;
					}
					mPikminCounts[mRow][mCol] += pow(10, mSelectedDigit + 1);

					for (int i = 0; i < pow(10, mSelectedDigit + 1); i++) {
						Game::Piki* piki = Game::pikiMgr->birth();

						Game::PikiInitArg arg(-1);
						piki->init(&arg);
						piki->changeShape(kindOrder[mRow]);
						piki->changeHappa(happaOrder[mCol]);

						Vector3f pos = Game::naviMgr->getActiveNavi()->getPosition();
						piki->setPosition(pos, false);

						piki->mNavi = Game::naviMgr->getActiveNavi();
					}
				}

				mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
				mSelectedDigit++;
				mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(255);
				mCounters[mRow][mCol]->update();
				ogSound->setPlusMinus(false);
			}
		}
	} else if (input & Controller::PRESS_RIGHT) {
		if (!mIsEditingPikminCount) {
			if (mCol == mNumCols - 1) {
				ogSound->setError();
				mScreenSquad->animation();
				return false;
			} else {
				mIcons[mRow][mCol]->setAlpha(128);
				mCol++;
				mIcons[mRow][mCol]->setAlpha(255);
				ogSound->setPlusMinus(false);
			}
		} else {
			if (mSelectedDigit == 0) {
				ogSound->setError();
			} else {
				mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
				mSelectedDigit--;
				mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(255);
				mCounters[mRow][mCol]->update();
				ogSound->setPlusMinus(false);
			}
		}
	} else if (input & Controller::PRESS_A) {
		if (!mIsEditingPikminCount) {
			mIsEditingPikminCount = true;
			ogSound->setOpen();
			mSelectedDigit = 0;
			mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(255);
			mCounters[mRow][mCol]->update();
		} else {
			mIsEditingPikminCount = false;
			ogSound->setDecide();
			mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
			mCounters[mRow][mCol]->update();
		}
	} else if (input & Controller::PRESS_B) {
		if (mIsEditingPikminCount) {
			mIsEditingPikminCount = false;
			ogSound->setDecide();
			mCounters[mRow][mCol]->getKetaPicture(mSelectedDigit)->setAlpha(128);
			mCounters[mRow][mCol]->update();
			
			// return early as ObjSMenuBase::doUpdate() will close the menu if B is pressed
			mScreenSquad->animation();
			return false;
		}
	}

	bool ret = ObjSMenuBase::doUpdate();
	mScreenSquad->animation();
	return ret;
}

void ObjSMenuSquad::doDraw(Graphics& gfx)
{
	J2DPerspGraph* graf = &gfx.mPerspGraph;
	graf->setPort();
	if (mScreenSquad) {
		mScreenSquad->draw(gfx, *graf);
	}
	drawYaji(gfx);
}

void ObjSMenuSquad::in_L()
{
	mState = MENUSTATE_OpenL;
	mAngle = 15.0;
}

void ObjSMenuSquad::in_R()
{
	mState = MENUSTATE_OpenR;
	mAngle = 15.0;
}

void ObjSMenuSquad::wait() { mState = MENUSTATE_Default; }

void ObjSMenuSquad::out_L()
{
	mState = MENUSTATE_CloseL;
	ogSound->setSMenuLR();
}

void ObjSMenuSquad::out_R()
{
	mState = MENUSTATE_CloseR;
	ogSound->setSMenuLR();
}

bool ObjSMenuSquad::doStart(::Screen::StartSceneArg const* arg)
{
	mAnimGroup->setFrame(0.0f);
	mAnimGroup->setRepeat(true);
	mAnimGroup->setSpeed(1.0f);
	mAnimGroup->start();
	setYajiName('6052_00', '3005_00', '3003_00'); // @P2GZ: change to "Menu" "Counters" "Squad"
	stopYaji();
	return start_LR(arg);
}

bool ObjSMenuSquad::doEnd(::Screen::EndSceneArg const*) { return true; }

void ObjSMenuSquad::doUpdateFinish() { ObjSMenuBase::doUpdateFinish(); }

bool ObjSMenuSquad::doUpdateFadeout()
{
	commonUpdate();
	return updateFadeOut();
}

void ObjSMenuSquad::doUpdateCancelAction() { }

ObjSMenuSquad::StaticValues ObjSMenuSquad::msVal;

} // namespace newScreen
} // namespace og
