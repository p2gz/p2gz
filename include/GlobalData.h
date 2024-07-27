#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"
#include "JSystem/JUtility/JUTGamePad.h"

struct ControllerInput {
	ControllerInput() {}
	ControllerInput(PADStatus status, JUTGamePad::CButton button, JUTGamePad::CStick mStick, JUTGamePad::CStick sStick) {
		mPadStatus = status;
		mPadButton = button;
		mPadMStick = mStick;
		mPadSStick = sStick;
	}

	PADStatus mPadStatus;
	JUTGamePad::CButton mPadButton;
	JUTGamePad::CStick mPadMStick;
	JUTGamePad::CStick mPadSStick;
};

struct SegmentRecord {
	SegmentRecord() {}

	u32 seed;
	int floorIndex;
	Game::PikiContainer squad;
};

struct P2GZ {
	P2GZ();

	~P2GZ() { }

    void init();
	void update();
	f32 getAnimationCoefficient();

	bool isCameraScroll();
	void setCameraScroll(bool);

	bool mIsScrollingCamera; // controlling camera for warping
	bool mIsSaveLoadPosition;
	int mScrollCameraZoom;

	int mSelectedPositionSlot;
	Vector3f mSavedPositions[4];

	f32 mAnimationCoefficient;
	f32 mDirection;
	
	J2DPicture* mControlStickPicture;
	J2DPicture* mCStickPicture;
	J2DPicture* mAButtonPicture;
	J2DPicture* mBButtonPicture;
	J2DPicture* mXButtonPicture;
	J2DPicture* mYButtonPicture;
	J2DPicture* mLButtonPicture;
	J2DPicture* mRButtonPicture;
	J2DPicture* mZButtonPicture;

	bool setCustomNextSeed; // whether to apply nextSeed next time a sublevel is generated
	u32 nextSeed;           // the seed to use for the next sublevel if setCustomNextSeed is true
	bool usePreviousSquad;
	RingBuffer<64, SegmentRecord>* history;

	u32 bugPokosCollectedSinceLoad;
	u32 treasurePokosCollectedSinceLoad;

	int mSelectedArea;
	int mSelectedDestination;
	int mSublevelNumber;

	ControllerInput mControllerInputs[5400]; // 30fps * 60s * 3m
	int mCurrentInputFrame;
	bool mIntendToRecordInputs;
	bool mIsRecordingInputs;
	bool mIntendToReplayInputs;
	bool mIsReplayingInputs;
	bool mShit;
	int mNumCaveStateExecs;
	int mNumGamePadReads;
};

extern P2GZ* p2gz;

#endif
