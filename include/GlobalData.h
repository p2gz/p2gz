#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"

struct SeedRecord {
	SeedRecord() {}

	u32 seed;
	int floorIndex;
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
	RingBuffer<64, SeedRecord>* seedHistory;

	u32 bugPokosCollectedSinceLoad;
	u32 treasurePokosCollectedSinceLoad;
};

extern P2GZ* p2gz;

#endif