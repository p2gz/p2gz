#ifndef _OG_NEWSCREEN_SMENU_H
#define _OG_NEWSCREEN_SMENU_H

#include "Screen/screenObj.h"
#include "Screen/Enums.h"
#include "JSystem/J2D/J2DTextBox.h"
#include "P2DScreen.h"

// @P2GZ
#include "Game/Entities/ItemPikihead.h"
#include "Game/gamePlayData.h"
#include "Game/gameStat.h"
#include "Game/Navi.h"
#include "Game/PikiMgr.h"
#include "Game/SingleGame.h"
#include "Game/SingleGameSection.h"
#include "og/Screen/callbackNodes.h"
#include "og/Sound.h"
#include "PikiAI.h"

#define MAX_CAVEDISP_NAME 5
#define MAX_RADAR_COUNT   200

namespace Game {
struct Navi;
} // namespace Game

namespace og {
namespace Screen {
struct ArrowAlphaBlink;
struct AnimGroup;
struct MapCounter;
struct AnimText_Screen;
struct MenuMgr;
} // namespace Screen

namespace newScreen {
enum PauseMenuState {
	PAUSEMENU_Normal  = 1,
	PAUSEMENU_Yuugata = 3, // 'sunset'
	PAUSEMENU_Zenkai  = 4, // 'main'
	PAUSEMENU_YesNo   = 6,
};

enum CurrSelPause {
	SELPAUSE_Continue = 0,
	SELPAUSE_Min      = SELPAUSE_Continue, // 0

	SELPAUSE_GoToSunset = 1,
	SELPAUSE_Return     = 2,               // unused?
	SELPAUSE_Max        = SELPAUSE_Return, // 2

	SELPAUSE_Count, // 3
};

enum CurrSelSunset {
	SELSUNSET_Yes = 0,
	SELSUNSET_Min = SELSUNSET_Yes, // 0

	SELSUNSET_No  = 1,
	SELSUNSET_Max = SELSUNSET_No, // 1
	SELSUNSET_Count,              // 2
};

enum CurrSelReturn {
	SELRETURN_Yes = 0,
	SELRETURN_No  = 1,
};

struct SceneSMenuBase : public ::Screen::SceneBase {
	inline SceneSMenuBase()
	    : mFinishState(1)
	{
	}

	virtual int doGetFinishState() { return mFinishState; } // _4C (weak)

	// _00      = VTBL
	// _00-_220 = Screen::SceneBase
	int mFinishState; // _220
};

// @P2GZ
struct SMenuSquad : public SceneSMenuBase {
	SMenuSquad();

	virtual const char* getResName() const { return "res_s_menu_squad.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_P2GZ_SQUAD; }    // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                  // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_P2GZ_SQUAD; }        // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                      // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                    // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                   // _24
	virtual void doUpdateActive();                                            // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                   // _30
	virtual void doSetBackupScene(::Screen::SetSceneArg&);                    // _48

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuCont : public SceneSMenuBase {
	SMenuCont();

	virtual const char* getResName() const { return "res_s_menu_controller.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU_CONTROLS; }         // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                       // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_CONTROLS; }    // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                           // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                         // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                        // _24
	virtual void doUpdateActive();                                                 // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                        // _30
	virtual void doSetBackupScene(::Screen::SetSceneArg&);                         // _48

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuItem : public SceneSMenuBase {
	SMenuItem();

	virtual const char* getResName() const { return "res_s_menu_item.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU_ITEMS; }      // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                 // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_ITEM; }  // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                     // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                   // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                  // _24
	virtual void doUpdateActive();                                           // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                  // _30
	virtual void doSetBackupScene(::Screen::SetSceneArg&);                   // _48

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuMap : public SceneSMenuBase {
	SMenuMap();

	virtual const char* getResName() const { return "res_s_menu_map.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU_MAP; }       // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_MAP; }  // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                    // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                  // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                 // _24
	virtual void doUpdateActive();                                          // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                 // _30
	virtual void doSetBackupScene(::Screen::SetSceneArg&);                  // _48

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuPause : public SceneSMenuBase {
	SMenuPause();

	virtual const char* getResName() const { return "res_s_menu_pause.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU; }             // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                  // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_PAUSE; }  // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                      // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                    // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                   // _24
	virtual void doUpdateActive();                                            // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                   // _30

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuPauseDoukutu : public SceneSMenuBase {
	SMenuPauseDoukutu();

	virtual const char* getResName() const { return "res_s_menu_pause_doukutu.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU_DOUKUTU; }             // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                          // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_PAUSE_DOUKUTU; }  // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                              // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                            // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                           // _24
	virtual void doUpdateActive();                                                    // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                           // _30
	virtual int doGetFinishState();                                                   // _4C

	// _00      = VTBL
	// _00-_224 = SceneSMenuBase
};

struct SMenuPauseVS : public ::Screen::SceneBase {
	SMenuPauseVS();

	virtual const char* getResName() const { return "res_s_menu_pause_VS.szs"; } // _1C (weak)
	virtual SceneType getSceneType() { return SCENE_PAUSE_MENU_VS; }             // _08 (weak)
	virtual ScreenOwnerID getOwnerID() { return OWNER_OGA; }                     // _0C (weak)
	virtual ScreenMemberID getMemberID() { return MEMBER_START_MENU_PAUSE_VS; }  // _10 (weak)
	virtual bool isUseBackupSceneInfo() { return true; }                         // _14 (weak)
	virtual void doCreateObj(JKRArchive*);                                       // _20
	virtual void doUserCallBackFunc(Resource::MgrCommand*);                      // _24
	virtual void doUpdateActive();                                               // _2C
	virtual bool doConfirmSetScene(::Screen::SetSceneArg&);                      // _30
	virtual void doSetBackupScene(::Screen::SetSceneArg&);                       // _48
	virtual int doGetFinishState();                                              // _4C

	// _00      = VTBL
	// _00-_220 = Screen::SceneBase
};

struct ObjSMenuBase : public ::Screen::ObjBase {

	enum MenuState { MENUSTATE_OpenL, MENUSTATE_OpenR, MENUSTATE_CloseL, MENUSTATE_CloseR, MENUSTATE_Default };

	enum MenuExitState { MENUCLOSE_None, MENUCLOSE_Finish, MENUCLOSE_L, MENUCLOSE_R };

	ObjSMenuBase();

	virtual ~ObjSMenuBase() { }              // _08
	virtual bool doUpdateFadein();           // _50
	virtual void doUpdateFadeinFinish();     // _54
	virtual bool doUpdate();                 // _58
	virtual void doUpdateFinish();           // _5C
	virtual void doUpdateFadeoutFinish();    // _64
	virtual void in_L()  = 0;                // _78
	virtual void in_R()  = 0;                // _7C
	virtual void wait()  = 0;                // _80
	virtual void out_L() = 0;                // _84
	virtual void out_R() = 0;                // _88
	virtual void loop();                     // _8C
	virtual void doUpdateCancelAction() = 0; // _90
	virtual void doUpdateRAction()      = 0; // _94
	virtual void doUpdateLAction()      = 0; // _98
	virtual bool updateFadeIn();             // _9C
	virtual bool updateFadeOut();            // _A0
	virtual void commonUpdate() = 0;         // _A4

	void setFinishState(s32);
	void setSMenuScale(f32, f32);
	void doCreateAfter(JKRArchive*, P2DScreen::Mgr*);
	void commonUpdateBase();
	void startBackupScene();
	void close_L();
	void jump_L(::Screen::SetSceneArg&);
	void jump_R(::Screen::SetSceneArg&);
	void setYajiName(u64, u64, u64);
	void stopYaji();
	void updateYaji();
	void drawYaji(Graphics&);
	bool start_LR(::Screen::StartSceneArg const*);

	// unused/inlined
	void registSMenuScreen(P2DScreen::Mgr*);
	void jump_LR(::Screen::SetSceneArg&, bool);
	void startYaji();

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_38 = Screen::ObjBase
	int mState;                           // _38, see MenuState enum
	int mCancelToState;                   // _3C, see MenuExitState enum
	f32 mMovePos;                         // _40
	f32 mFadeLevel;                       // _44
	bool mExiting;                        // _48
	f32 mAngle;                           // _4C
	u8 mIsDay1;                           // _50
	J2DPictureEx* mPanePeffect;           // _54
	u32 mButtonStates[2];                 // _58, 0 = L button, 1 = R button
	P2DScreen::Mgr_tuning* mScreenLR;     // _60, manager screen for LR button panes
	J2DPane* mNyaji_l;                    // _64, overall L button pane
	J2DPane* mNyaji_r;                    // _68, overall R button pane
	J2DTextBoxEx* mTyaji_l;               // _6C, textbox for L button pane
	J2DTextBoxEx* mTyaji_r;               // _70, textbox for R button pane
	bool mEnableYaji;                     // _74, are LR panes displayed?
	Vector2f mYajiLpos;                   // _78, position for L button pane
	Vector2f mYajiRpos;                   // _80, position for R button pane
	f32 mUnused88;                        // _88, set to 0 and never used
	u8 mAlpha;                            // _8C
	Screen::ArrowAlphaBlink* mArrowBlink; // _90
	J2DPictureEx* mPanePeffect1;          // _94
	J2DPictureEx* mPanePeffect2;          // _98
	P2DScreen::Mgr* mScreenMain;          // _9C, manager screen for current menu pane (map/item/pause)
	J2DPane* mPaneNsize;                  // _A0
	f32 mUnusedA4;                        // _A4, set to 1.0 and never used

	static struct StaticValues {
		inline StaticValues()
		{
			mAnimSpeed           = 1.0f;
			_04                  = 1.0f;
			_14                  = 255;
			mFadeInOutTime       = 0.3f;
			mLRArrowMoveDistance = 30.0f;
			mLRArrowMoveSpeed    = 0.1f;
			_15                  = 64;
			_16                  = 64;
			_17                  = 0;
			mUseControlMenu      = true; // @P2GZ: set to true to enable controller menu (to repurpose)
			mUpdateYaji          = 0;
		}

		f32 mAnimSpeed;           // _00
		f32 _04;                  // _04
		f32 mFadeInOutTime;       // _08
		f32 mLRArrowMoveDistance; // _0C
		f32 mLRArrowMoveSpeed;    // _10
		u8 _14;                   // _14
		u8 _15;                   // _15
		u8 _16;                   // _16
		u8 _17;                   // _17
		bool mUseControlMenu;     // _18
		bool mUpdateYaji;         // _19
	} msBaseVal;
};

// @P2GZ
struct MenuOption {
	MenuOption(J2DTextBoxEx* text, og::Screen::CallBack_CounterRV* counter, u32* value, int maxDigits, int minValue, int maxValue)
		: mText(text)
		, mCounter(counter)
		, mValue(value)
		, mMaxDigits(maxDigits)
		, mMinValue(minValue)
		, mMaxValue(maxValue)
	{
		mCurrentDigit = 0;
		mText->setAlpha(128);
		mCounter->mIsP2GZCounter = true;
		
		for (int i = 0; i < mMaxDigits; i++) {
			mCounter->getKetaPicture(i)->setAlpha(128);
		}
	}

	virtual void up() {
		if (*mValue + pow(10, mCurrentDigit) > mMaxValue) {
			ogSound->setError();
			return;
		}

		*mValue += pow(10, mCurrentDigit);
		if (*mValue == pow(10, mCurrentDigit + 1)) {
			left();
		}
		mCounter->update();
		ogSound->setPlusMinus(false);
	};

	virtual void down() {
		if (*mValue - pow(10, mCurrentDigit) < mMinValue) {
			ogSound->setError();
			return;
		}

		*mValue -= pow(10, mCurrentDigit);
		if (*mValue < pow(10, mCurrentDigit) && mCurrentDigit > 0) {
			right();
		}
		mCounter->update();
		ogSound->setPlusMinus(false);
	};

	virtual void left() {
		if (mCurrentDigit == mMaxDigits - 1) {
			ogSound->setError();
			return;
		}

		disableCurrentDigit();
		mCurrentDigit++;
		if (*mValue < pow(10, mCurrentDigit)) {
			*mValue += pow(10, mCurrentDigit);
			mCounter->update();
		}
		enableCurrentDigit();

		ogSound->setPlusMinus(false);
	};
	
	virtual void right() {
		if (mCurrentDigit == 0) {
			ogSound->setError();
			return;
		}

		disableCurrentDigit();
		mCurrentDigit--;
		enableCurrentDigit();

		ogSound->setPlusMinus(false);
	};

	void enableText() {
		mText->setAlpha(255);
		mText->update();
	}

	void disableText() {
		mText->setAlpha(128);
		mText->update();
	}

	void enableCurrentDigit() {
		mCounter->getKetaPicture(mCurrentDigit)->setAlpha(255);
		mCounter->getKetaPicture(mCurrentDigit)->update();
	}

	void disableCurrentDigit() {
		mCounter->getKetaPicture(mCurrentDigit)->setAlpha(128);
		mCounter->getKetaPicture(mCurrentDigit)->update();
	}

	J2DTextBoxEx* mText;
	og::Screen::CallBack_CounterRV* mCounter;
	u32* mValue;
	int mMaxDigits;
	int mMinValue;
	int mMaxValue;

	int mCurrentDigit;
};

// @P2GZ
struct BitterSprayMenuOption : public MenuOption {
	BitterSprayMenuOption(J2DTextBoxEx* text, og::Screen::CallBack_CounterRV* counter, u32* value, int maxDigits, int minValue, int maxValue)
		: MenuOption(text, counter, value, maxDigits, minValue, maxValue)
	{
	}

	void up() {
		if (*mValue == 0) {
			Game::playData->setDemoFlag(Game::DEMO_First_Bitter_Spray_Made);
			Game::playData->setDemoFlag(Game::DEMO_BITTER_ENABLED);
		}
		MenuOption::up();
	}

	void down() {
		MenuOption::down();
		if (*mValue == 0) {
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Bitter_Spray_Made);
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_BITTER_ENABLED);
		}
	}

	void left() {
		if (*mValue == 0) {
			Game::playData->setDemoFlag(Game::DEMO_First_Bitter_Spray_Made);
			Game::playData->setDemoFlag(Game::DEMO_BITTER_ENABLED);
		}
		MenuOption::left();
	}

	void right() {
		MenuOption::right();
		if (*mValue == 0) {
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Bitter_Spray_Made);
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_BITTER_ENABLED);
		}
	}
};

// @P2GZ
struct SpicySprayMenuOption : public MenuOption {
	SpicySprayMenuOption(J2DTextBoxEx* text, og::Screen::CallBack_CounterRV* counter, u32* value, int maxDigits, int minValue, int maxValue)
		: MenuOption(text, counter, value, maxDigits, minValue, maxValue)
	{
	}

	void up() {
		if (*mValue == 0) {
			Game::playData->setDemoFlag(Game::DEMO_First_Spicy_Spray_Made);
			Game::playData->setDemoFlag(Game::DEMO_SPICY_ENABLED);
		}
		MenuOption::up();
	}

	void down() {
		MenuOption::down();
		if (*mValue == 0) {
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Spicy_Spray_Made);
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_SPICY_ENABLED);
		}
	}

	void left() {
		if (*mValue == 0) {
			Game::playData->setDemoFlag(Game::DEMO_First_Spicy_Spray_Made);
			Game::playData->setDemoFlag(Game::DEMO_SPICY_ENABLED);
		}
		MenuOption::left();
	}

	void right() {
		MenuOption::right();
		if (*mValue == 0) {
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Spicy_Spray_Made);
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_SPICY_ENABLED);
		}
	}
};

// @P2GZ
struct PokoCountMenuOption : public MenuOption {
	PokoCountMenuOption(J2DTextBoxEx* text, og::Screen::CallBack_CounterRV* counter, u32* value, int maxDigits, int minValue, int maxValue)
		: MenuOption(text, counter, value, maxDigits, minValue, maxValue)
	{
	}
};

// @P2GZ
struct ObjSMenuSquad : public ObjSMenuBase {
	ObjSMenuSquad(const char*);

	virtual ~ObjSMenuSquad();                             // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction();                  // _90 (weak)
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuSquad* mDisp; // _A8
	P2DScreen::Mgr_tuning* mScreenSquad;     // _AC
	og::Screen::AnimGroup* mAnimGroup;       // _B0
	
	J2DPictureEx* mIcons[5][3];
	og::Screen::CallBack_CounterRV* mCounters[5][3];
	u32 mPikminCounts[5][3];

	int mRow;
	int mCol;
	int mNumRows;
	int mNumCols;
	int mSelectedDigit;
	bool mIsEditingPikminCount;

	static struct StaticValues {
		inline StaticValues()
		{
			mScaleX = 1.0f;
			mScaleY = 1.0f;
		}

		f32 mScaleX; // _00
		f32 mScaleY; // _04
	} msVal;
};

// size: 0xBC
struct ObjSMenuCont : public ObjSMenuBase {
	ObjSMenuCont(const char*);

	virtual ~ObjSMenuCont();                              // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction();                  // _90 (weak)
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuCont* mDisp; // _A8
	P2DScreen::Mgr_tuning* mScreenCont;     // _AC
	og::Screen::AnimGroup* mAnimGroup;      // _B0
	u8 _B4[0x8];                            // _B4, unknown

	// @P2GZ start
	MenuOption* mOptions[3];
	int mNumOptions;
	int mSelectedOption;
	bool mIsEditingOption;
	// @P2GZ end

	static struct StaticValues {
		inline StaticValues()
		{
			mScaleX = 1.0f;
			mScaleY = 1.0f;
		}

		f32 mScaleX; // _00
		f32 mScaleY; // _04
	} msVal;
};

struct ObjSMenuItem : public ObjSMenuBase {
	ObjSMenuItem(const char*);

	virtual ~ObjSMenuItem();                              // _08
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction() { }               // _90 (weak)
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuItem* mDisp; // _A8
	P2DScreen::Mgr_tuning* mScreenItems;    // _AC, main screen for items and sprays
	P2DScreen::Mgr* mScreenSprays;          // _B0, subscreen for sprays
	og::Screen::AnimGroup* mItemAnims;      // _B4, anims for everything else
	og::Screen::AnimGroup* mSprayAnims;     // _B8, spray-specific anims
	J2DPane* mBitterSprayPane;              // _BC, Nspray0
	J2DPane* mSpicySprayPane;               // _C0, Nspray1
	J2DPane* mBitterSpraySubPane;           // _C4, Nspray0
	J2DPane* mSpicySpraySubPane;            // _C8, Nspray1
	u8 _CC[0x8];                            // _CC, unknown

	static struct StaticValues {
		inline StaticValues()
		{
			mItemColorWhite.r = 0;
			mItemColorWhite.g = 110;
			mItemColorWhite.b = 176;
			mItemColorWhite.a = 255;

			mItemColorBlack.r = 0;
			mItemColorBlack.g = 0;
			mItemColorBlack.b = 100;
			mItemColorBlack.a = 255;

			mBitterSprayOffset = Vector2f(-27.0f);
			mSpicySprayOffset  = Vector2f(-27.0f);
			mItemMenuScale     = Vector2f(0.95f, 0.9f);
		}

		JUtility::TColor mItemColorWhite; // _00
		JUtility::TColor mItemColorBlack; // _04
		Vector2f mBitterSprayOffset;      // _08
		Vector2f mSpicySprayOffset;       // _10
		Vector2f mItemMenuScale;          // _18
	} msVal;
};

struct ObjSMenuMap : public ObjSMenuBase {
	ObjSMenuMap(const char*);

	virtual ~ObjSMenuMap();                               // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction() { }               // _90 (weak)
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	void setMapTexture();
	void tuningIcon();
	void initMapIcon(JKRArchive*);
	void appendCaveName(J2DPane*, u16, u64);
	void transMap();
	void updateMap();
	void drawMap(Graphics&);
	void drawRectZ(Graphics&, Rectf&, Color4&, f32);
	void drawVecZ(Graphics&, Vec&, Vec&, Vec&, Vec&, Color4&, f32);

	// unused/inlined
	void calcMapScale();
	void calcMapPos(Vector2f pos, Vector2f* outPos);
	void setMapPos();
	void setCompass();
	void rotateMap();
	void scaleMap();
	void setMapColor();
	u8 calcCaveNameAlpha();

	f32 getMapAdjustVal(f32 stickMag, f32 factor) { return stickMag * (factor * (1.0f / mCurrentZoom)); }

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuMap* mDisp;              // _A8
	og::Screen::MapCounter* mMapCounter;                // _AC
	og::Screen::AnimGroup* mAnimGroup;                  // _B0
	J2DPictureEx* mMapAreaPane;                         // _B4, pane mapcent, rectangular area for map to sit in
	u32 _B8;                                            // _B8, unknown
	P2DScreen::Mgr_tuning* mIconScreen;                 // _BC, manages the icon textures that go on the radar map
	JUTTexture* mRadarMapTexture;                       // _C0, the actual texture
	J2DPane* mIconRootPane;                             // _C4, for icon_screen
	J2DPictureEx* mMapTexPane;                          // _C8
	J2DPane*** mRadarPaneList;                          // _CC
	J2DPictureEx* mOlimarArrow;                         // _D0, ic orima?
	Game::Navi* mOlimarObj;                             // _D4
	J2DPictureEx* mLouieArrow;                          // _D8, ic louzy?
	Game::Navi* mLouieObj;                              // _DC, or president
	Vector2f mMapPosition;                              // _E0
	f32 mCurrentZoom;                                   // _E8
	f32 mMapAngle;                                      // _EC
	Vector2f mMapTexScale;                              // _F0
	Vector2f mMapTextureDimensions;                     // _F8
	Vector2f mMapBounds;                                // _100
	Vector2f _108;                                      // _108, seems to be unused
	Vector2f mMapRotationOrigin;                        // _110
	Controller* mController;                            // _118
	int mMapIconNum;                                    // _11C
	u8 mUpdateCaveTex;                                  // _120
	P2DScreen::Mgr_tuning* mIconScreen2;                // _124, not sure why the game loads two of this screen file
	J2DPane* mCompassPane;                              // _128, pane Ncompas
	J2DPictureEx* mCompassPic;                          // _12C, pane compass
	J2DPictureEx* mOlimarGlow;                          // _130, the glow behind the olimar arrow pane
	J2DPictureEx* mLouieGlow;                           // _134, same for louie
	f32 mStartZoom;                                     // _138
	u8 mZoomCaveTextAlpha;                              // _13C
	J2DTextBox* mCaveLabelTextBoxes[MAX_CAVEDISP_NAME]; // _140
	int mCaveLabelCount;                                // _154

	static struct StaticValues {
		inline StaticValues()
		    : _08(255)
		    , _09(255)
		    , _0A(255)
		    , mMapTexColorWhite(210, 220, 255, 255)
		    , mMapTexColorBlack(0, 0, 50, 0)
		    , mItemPelletWhiteColor(255, 132, 0, 255)
		    , mItemPelletBlackColor(255, 0, 0, 0)
		    , mMapMoveRate(5.5f)
		    , mMapMoveRate2(2.0f)
		    , mMapMoveInputReduction(1.8f)
		{
			_1B                   = 220;
			mMinZoom              = 0.5f;
			mMaxZoom              = 8.0f;
			mMapIconScaleBase     = 0.7f;
			mMapNaviArrowScaleMod = 1.5f;
			mGroundZoom           = 1.8f;
			mCaveZoom             = 1.8f;
			mMapTexOffset.x       = 22.0f;
			mMapTexOffset.y       = 12.0f;
			mMapScreenScale.x     = 1.0f;
			mMapScreenScale.y     = 0.92f;
			mTempPikiColorWhite.r = 0;
			mTempPikiColorWhite.g = 128;
			mTempPikiColorWhite.b = 255;
			mTempPikiColorWhite.a = 255;
			mTempPikiColorBlack.r = 0;
			mTempPikiColorBlack.g = 255;
			mTempPikiColorBlack.b = 255;
			mTempPikiColorBlack.a = 0;
		}

		f32 mMinZoom;                           // _00
		f32 mMaxZoom;                           // _04
		u8 _08;                                 // _08
		u8 _09;                                 // _09
		u8 _0A;                                 // _0A
		JUtility::TColor mMapTexColorWhite;     // _0B
		JUtility::TColor mMapTexColorBlack;     // _0F
		JUtility::TColor mItemPelletWhiteColor; // _13
		JUtility::TColor mItemPelletBlackColor; // _17
		u8 _1B;                                 // _1B
		f32 mMapMoveRate;                       // _1C
		f32 mMapMoveRate2;                      // _20
		f32 mMapIconScaleBase;                  // _24
		f32 mMapNaviArrowScaleMod;              // _28
		f32 mGroundZoom;                        // _2C
		f32 mCaveZoom;                          // _30
		Vector2f mMapTexOffset;                 // _34
		f32 mMapMoveInputReduction;             // _3C
		Vector2f mMapScreenScale;               // _40
		JUtility::TColor mTempPikiColorWhite;   // _48
		JUtility::TColor mTempPikiColorBlack;   // _4C
	} msVal;
};

struct ObjSMenuPause : public ObjSMenuBase {

	static struct ObjHIOVal {
		ObjHIOVal()
		{
			mOptionBaseDelayTime = 0.3f;

			mMenuBodyBlink1.set(253, 145, 21, 39); // orange
			mMenuBodyBlink2.set(255, 107, 0, 255); // orange
			mMenuBodyWhite.set(255, 255, 255, 49); // white

			mMenuBodyBlack.set(253, 255, 255, 0); // white
			mMenuBgWhite.set(0, 73, 128, 200);    // dark blue
			mMenuBgBlack.set(175, 175, 255, 0);   // white (light purple)

			mTitleBodyWhite.set(0, 0, 255, 255);   // blue
			mTitleBodyBlack.set(255, 255, 255, 0); // white
			mTitleBgWhite.set(255, 255, 255, 255); // white
			mTitleBgBlack.set(0, 255, 255, 0);     // white (teal)

			mGreyedOutBodyWhite.set(0, 0, 255, 64);    // blue
			mGreyedOutBodyBlack.set(255, 255, 187, 0); // white (yellow)

			mGreyedOutBgWhite.set(76, 74, 0, 255);   // khaki
			mGreyedOutBgBlack.set(255, 255, 255, 0); // white

			mPauseScreenScale = Vector2f(1.0f);
		}

		static void getMenuColor(JUtility::TColor*, JUtility::TColor*, JUtility::TColor*, JUtility::TColor*, JUtility::TColor*,
		                         JUtility::TColor*, JUtility::TColor*, JUtility::TColor*, JUtility::TColor*, JUtility::TColor*,
		                         JUtility::TColor*, JUtility::TColor*, JUtility::TColor*, JUtility::TColor*);

		f32 mOptionBaseDelayTime;             // _00, delay time in seconds for first menu option to display
		JUtility::TColor mMenuBodyBlink1;     // _04
		JUtility::TColor mMenuBodyBlink2;     // _08
		JUtility::TColor mMenuBodyWhite;      // _0C
		JUtility::TColor mGreyedOutBodyWhite; // _10
		JUtility::TColor mGreyedOutBodyBlack; // _14
		JUtility::TColor mMenuBodyBlack;      // _18
		JUtility::TColor mMenuBgWhite;        // _1C
		JUtility::TColor mMenuBgBlack;        // _20
		JUtility::TColor mGreyedOutBgWhite;   // _24
		JUtility::TColor mGreyedOutBgBlack;   // _28
		JUtility::TColor mTitleBodyWhite;     // _2C
		JUtility::TColor mTitleBodyBlack;     // _30
		JUtility::TColor mTitleBgWhite;       // _34
		JUtility::TColor mTitleBgBlack;       // _38
		Vector2f mPauseScreenScale;           // _3C
	} msVal;

	ObjSMenuPause(const char*);

	virtual ~ObjSMenuPause();                             // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doUpdateFadeoutFinish();                 // _64
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction();                  // _90
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	void blink_TopMenu(int);
	bool menu_pause();
	bool menu_yuugata();
	bool menu_zenkai();
	void killCursorAll();
	bool menu();

	// unused/inline
	void open_TopMenu();
	void close_TopMenu();
	void open_Yuugata();
	void close_Yuugata();
	void blink_Yuugata(int);
	void open_Zenkai();
	void close_Zenkai();
	void blink_Zenkai(int);

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuPause* mDisp;    // _A8
	int mCurrSelPause;                          // _AC, see CurrSelPause enum
	int mCurrSelSunset;                         // _B0, see CurrSelSunset enum
	int mCurrSelReturn;                         // _B4, see CurrSelReturn enum
	int mMenuState;                             // _B8, see PauseMenuState enum
	s16 _BC;                                    // _BC
	P2DScreen::Mgr_tuning* mScreenPause;        // _C0
	og::Screen::MenuMgr* mMenuPause;            // _C4
	og::Screen::MenuMgr* mMenuSunset;           // _C8
	og::Screen::MenuMgr* mMenuReturn;           // _CC
	u32 _D0;                                    // _D0, unknown
	u32 _D4;                                    // _D4, unknown
	og::Screen::AnimText_Screen* mTextContinue; // _D8, h_00?
	og::Screen::AnimText_Screen* mTextGoSunset; // _DC, h_01?
	og::Screen::AnimText_Screen* mTextReturn;   // _E0, h_02?
	og::Screen::AnimText_Screen* mTextSunsetQ;  // _E4, h_03?
	og::Screen::AnimText_Screen* mTextSunsetY;  // _E8, h_04?
	og::Screen::AnimText_Screen* mTextSunsetN;  // _EC, h_05?
	og::Screen::AnimText_Screen* mTextReturnQ;  // _F0, h_06?
	og::Screen::AnimText_Screen* mTextReturnY;  // _F4, h_07?
	og::Screen::AnimText_Screen* mTextReturnN;  // _F8, h_08?
	og::Screen::AnimGroup* mAnims;              // _FC
	bool mPauseOpened;                          // _100
	f32 mMenuPauseTimer;                        // _104
	bool mSunsetOpened;                         // _108
	f32 mMenuSunsetTimer;                       // _10C
	bool mReturnOpened;                         // _110
	f32 mMenuReturnTimer;                       // _114
};

struct ObjSMenuPauseDoukutu : public ObjSMenuBase {
	ObjSMenuPauseDoukutu(const char*);

	virtual ~ObjSMenuPauseDoukutu();                      // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doUpdateFadeoutFinish();                 // _64
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88
	virtual void doUpdateCancelAction();                  // _90
	virtual void doUpdateRAction();                       // _94
	virtual void doUpdateLAction();                       // _98
	virtual void commonUpdate();                          // _A4

	bool menu_pause();
	bool menu_giveup();
	void finishPause();
	bool menu();
	void set_Blink_Normal();
	void set_Blink_YesNo();
	void set_Menu_Normal();
	void set_Menu_YesNo();

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	og::Screen::DispMemberSMenuPauseDoukutu* mDisp; // _A8
	int mMenuState;                                 // _AC
	int mCurrPauseSel;                              // _B0
	int mCurrGiveupSel;                             // _B4
	u16 _B8;                                        // _B8, unknown
	P2DScreen::Mgr_tuning* mScreenPause;            // _BC
	og::Screen::MenuMgr* mMenuPause;                // _C0
	og::Screen::MenuMgr* mMenuGiveup;               // _C4
	J2DPane* mMenuPane;                             // _C8
	og::Screen::AnimText_Screen* mTextContinue;     // _CC
	og::Screen::AnimText_Screen* mTextDoGiveup;     // _D0
	og::Screen::AnimText_Screen* mTextGiveupQ;      // _D4
	og::Screen::AnimText_Screen* mTextGiveupY;      // _D8
	og::Screen::AnimText_Screen* mTextGiveupN;      // _DC
	og::Screen::AnimGroup* mAnims;                  // _E0
	u32 mPokos;                                     // _E4
	bool mPauseOpened;                              // _E8
	f32 mMenuPauseTimer;                            // _EC
	bool mGiveupOpened;                             // _F0
	f32 mMenuGiveupTimer;                           // _F4
	f32 mWarningTimer;                              // _F8

	static struct StaticValues {
		inline StaticValues()
		{
			mMenuScaleX      = 1.0f;
			mMenuScaleY      = 0.91f;
			mPikiWarningTime = 0.5f;
		}

		f32 mMenuScaleX; // _00
		f32 mMenuScaleY; // _04
		f32 mPikiWarningTime;
	} msVal;
};

struct ObjSMenuPauseVS : public ObjSMenuBase {
	ObjSMenuPauseVS(const char*);

	virtual ~ObjSMenuPauseVS();                           // _08 (weak)
	virtual bool doStart(const ::Screen::StartSceneArg*); // _44
	virtual bool doEnd(const ::Screen::EndSceneArg*);     // _48
	virtual void doCreate(JKRArchive*);                   // _4C
	virtual bool doUpdateFadein();                        // _50
	virtual void doUpdateFadeinFinish();                  // _54
	virtual bool doUpdate();                              // _58
	virtual void doUpdateFinish();                        // _5C
	virtual bool doUpdateFadeout();                       // _60
	virtual void doUpdateFadeoutFinish();                 // _64
	virtual void doDraw(Graphics& gfx);                   // _68
	virtual void in_L();                                  // _78
	virtual void in_R();                                  // _7C (weak)
	virtual void wait();                                  // _80
	virtual void out_L();                                 // _84
	virtual void out_R();                                 // _88 (weak)
	virtual void doUpdateCancelAction();                  // _90
	virtual void doUpdateRAction();                       // _94 (weak)
	virtual void doUpdateLAction();                       // _98 (weak)
	virtual void commonUpdate();                          // _A4
	virtual void out_cancel();                            // _A8
	virtual void out_menu_0();                            // _AC
	virtual void out_menu_1();                            // _B0
	virtual int getResult();                              // _B4

	void blink_Menu(int);
	bool menu();

	// _00     = VTBL1
	// _18     = VTBL2
	// _00-_A8 = ObjSMenuBase
	Screen::DispMemberSMenuPauseVS* mDisp; // _A8
	int mCurrMenuSel;                      // _AC
	P2DScreen::Mgr_tuning* mScreenPause;   // _B0
	Screen::MenuMgr* mMenuMgr;             // _B4
	Screen::AnimText_Screen* mAnimText1;   // _B8
	Screen::AnimText_Screen* mAnimText2;   // _BC
	bool mMenuOpen;                        // _C0
	f32 mMenuTimer;                        // _C4
	u8 mType;                              // _C8
};

struct StartSceneArgSMenu : public ::Screen::StartSceneArgTemplate<StartSceneArgSMenu> {
	virtual SceneType getSceneType() const // _08 (weak)
	{
		return mSceneType;
	}

	// _00     = VTBL
	// _00-_08 = StartSceneArgTemplate
};
} // namespace newScreen
} // namespace og

#endif
