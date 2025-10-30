#ifndef _LOCALIZATIONTREASURESWAP_H
#define _LOCALIZATIONTREASURESWAP_H

#include "Game/pelletConfig.h"
#include <p2gz/p2gz.h>

enum TreasureRegion {
	Treasure_US  = 0,
	Treasure_PAL = 1,
	Treasure_JP  = 2,
};

struct LocalizationTreasureSwap {
	System::LanguageID region; // determine which region all the settings below are for
	char* mName;
	char* mArchive;
	// char* mTxtArchive; this is the same as archive, I think unused?
	char* mBmd;
	char* mAnimMgr;
	char* mCollTree;
	float mRadius;
	float mPRadius;
	float mHeight;
	float mIntertialScaling;
	char* mParticleType;
	int mNumParticles;
	float mParticleSize;
	float mFriction;
	int mMin;
	int mMax;
	// int mPikiCountMax; // treasures never spawn pikmin so we don't worry about these fields
	// int mPikiCountMin;
	char* mDynamics;
	int mMoney;
	char* mUnique;
	char* mIndirect;
	int mNumPMotions;
	float mDepth;
	float mDepthMax;
	float mDepthA;
	float mDepthB;
	float mDepthC;
	float mDepthD;
	// Vector3f mOffset; no treasure uses this either?
	// u64 mMessage; or this?
	u16 mCode;
	u16 mDictionary;
	// s16 mIndex; // this should just be copied, all treasures ahve the same index
	u8 mIndirectState;
};
namespace LocalizationTreasureSwapFunc {

void updatePelletConfig(Game::PelletConfig* thisConfig);

}; // namespace LocalizationTreasureSwapFunc

#endif
