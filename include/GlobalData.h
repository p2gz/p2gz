#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"


struct SegmentRecord {
	SegmentRecord() {
		timer = 0;
	}

	u32 seed;
	int floorIndex;
	Game::PikiContainer squad;
	f32 timer;
};

struct P2GZ {
	P2GZ();

	~P2GZ() { }

    void init();
	void update();
	f32 getAnimationCoefficient();

	bool isCameraScroll();
	void setCameraScroll(bool);

	void drawTimer();

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
	
	bool showTimer;
};

extern P2GZ* p2gz;

#endif
