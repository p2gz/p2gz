#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"

struct SeedHistoryTracker {
	static const u32 SEED_HISTORY_SIZE = 64;

	SeedHistoryTracker();
	~SeedHistoryTracker() {}

	struct SeedRecord {
		SeedRecord() {} // necessary for default initialisation
		SeedRecord(u32 seed, int floorIndex);

		u32 seed;
		int floorIndex;
	};

	void push(u32 seed, int floorIndex);

	/// @brief Removes a seed from the history and returns it
	/// @return The latest seed entry in the history
	SeedRecord pop();

	/// @brief Returns a copy of the latest seed entry in the history without removing it
	/// @return The latest seed entry in the history
	SeedRecord peek();

	int seedBufHead;
	SeedRecord seedRingBuf[SEED_HISTORY_SIZE];
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
	SeedHistoryTracker seedHistory;
};

extern P2GZ* p2gz;

#endif