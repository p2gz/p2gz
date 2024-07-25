#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"

struct Preset {
	Preset() {
		mNumBitters = 0;
		mNumSpicies = 0;
	}
	Preset(const char* presetString);

	void _addByCharCode(char code, u32 count, bool inOnion);

	u64 mMsgId;
	Game::PikiContainer mSquad;
	Game::PikiContainer mOnionPikis;
	u16 mNumBitters;
	u16 mNumSpicies;
	// TODO: upgrades
};

struct SegmentRecord {
	SegmentRecord() {}

	u32 seed;
	int floorIndex;
	Game::PikiContainer squad;
	s64 startTime;
	s64 endTime;
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
	u32 getDefaultPresetId(int area, int destination, int sublevel);
	void applyPreset(Preset&);

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
	gzCollections::RingBuffer<64, SegmentRecord>* history;

	u32 bugPokosCollectedSinceLoad;
	u32 treasurePokosCollectedSinceLoad;

	int mSelectedArea;
	int mSelectedDestination;
	int mSublevelNumber;

	bool showTimer;
	
	int mSelectedPreset;
	gzCollections::Vec<Preset> mPresets;
};

extern P2GZ* p2gz;

#endif
