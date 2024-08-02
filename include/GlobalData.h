#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"
#include "gzCollections.h"
#include "Game/PikiContainer.h"

struct Preset {
	Preset();

	Preset& setMsgId(s64 msgId);
	Preset& setPikmin(int happa, int color, int amount);
	Preset& setOnionPikmin(int happa, int color, int amount);
	Preset& setSprays(int spicies, int bitters);
	Preset& addCutsceneFlags(u16 flags[], size_t numFlags);

	s64 mMsgId;
	Game::PikiContainer mSquad;
	Game::PikiContainer mOnionPikis;
	u16 mNumBitters;
	u16 mNumSpicies;
	gzCollections::Vec<u16> mCutsceneFlags;
	// TODO: upgrades
};

struct SegmentRecord {
	SegmentRecord() {}

	u32 mSeed;
	int mFloorIndex;
	int mAreaIndex;
	int mDestinationIndex;
	Game::PikiContainer mSquad;
	s64 mStartTime;
	s64 mEndTime;
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
	Preset& getPresetByMsgId(s64 msgId);
	s64 getDefaultPresetId(int area, int destination, int sublevel);
	void applyPreset(Preset&);

	void warpToSelectedCave(Game::PikiContainer* squad);
	void warpToSelectedArea();

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

	bool mSetCustomNextSeed; // whether to apply nextSeed next time a sublevel is generated
	u32 mNextSeed;           // the seed to use for the next sublevel if setCustomNextSeed is true
	gzCollections::RingBuffer<64, SegmentRecord>* mHistory;

	u32 mBugPokosCollectedSinceLoad;
	u32 mTreasurePokosCollectedSinceLoad;

	int mSelectedArea;
	int mSelectedDestination;
	int mSublevelNumber;
	int mSelectedPresetIndex;

	bool mShowTimer;
	s64 mCaveStartTimeMs;

	gzCollections::Vec<Preset> mPresets;
};

extern P2GZ* p2gz;

#endif
