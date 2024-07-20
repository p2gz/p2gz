#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"

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
	f32 mAnimationCoefficient;
	f32 mDirection;
	Game::NaviWhistle* mWhistle;
	
	J2DPicture* mControlStickPicture;
	J2DPicture* mCStickPicture;
	J2DPicture* mBButtonPicture;
	J2DPicture* mLButtonPicture;

	bool setCustomNextSeed; // whether to apply nextSeed next time a sublevel is generated
	u32 nextSeed;           // the seed to use for the next sublevel if setCustomNextSeed is true
	bool usePreviousSquad;
	RingBuffer<64, SegmentRecord>* history;

	u32 bugPokosCollectedSinceLoad;
	u32 treasurePokosCollectedSinceLoad;
};

extern P2GZ* p2gz;

#endif