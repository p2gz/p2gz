#ifndef _GZ_Localization_H
#define _GZ_Localization_H

#include <System.h>
#include "Game/pelletMgr.h"

struct Localization {

public:
	Localization(u8 randVal)
	    : treasure_region_id(System::LANG_English)
	{
		funniMenuTextSetting = randVal == 0;
	}

	void init_menu();
	System::LanguageID get_treasure_region() { return treasure_region_id; }
	bool get_funni_text() { return funniMenuTextSetting; }

	void set_treasure_region(size_t new_treasure_region_id);
	void set_funni_text(u8 setting_) { funniMenuTextSetting = setting_; }

private:
	// Used to determine which treasure to spawn if it's different in a region (US vs. PAL vs. JP)
	System::LanguageID treasure_region_id;
	// Funni check; 0 means menu text is localisation, 1 is menu text means localization
	u8 funniMenuTextSetting;
};

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

void updatePelletConfig(Game::PelletConfig* thisConfig);

extern const u32 TREASURE_MAP_COUNT;
extern const LocalizationTreasureSwap treasureMap[];

#endif
