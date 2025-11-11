#ifndef _GZ_Localization_H
#define _GZ_Localization_H

#include <System.h>
#include "Game/pelletMgr.h"

namespace gz {

enum TreasureRegion {
	Treasure_US,  // NTSC-U
	Treasure_JP,  // NTSC-J
	Treasure_PAL, // PAL
};

struct Localization {
public:
	Localization(u8 randVal)
	    : next_treasure_region(Treasure_US)
	    , active_treasure_region(Treasure_US)
	    , pending_update(false)
	{
		funniMenuTextSetting = randVal == 0;
	}

	void init_menu();
	TreasureRegion get_treasure_region() { return next_treasure_region; }
	bool get_funni_text() { return funniMenuTextSetting; }

	void set_treasure_region(size_t new_treasure_region_id);
	void set_funni_text(u8 setting_) { funniMenuTextSetting = setting_; }

	bool require_update() { return pending_update; }
	void update_region();

private:
	// Used to determine which treasure to spawn if it's different in a region (US vs. PAL vs. JP)
	TreasureRegion active_treasure_region; // region since most recent update
	TreasureRegion next_treasure_region;   // region to apply on next update (i.e. current selection in menu)
	bool pending_update;                   // basically active != next
	u8 funniMenuTextSetting;               // Funni check; 0 means menu text is localisation, 1 is menu text means localization
};

struct LocalizationTreasureSwap {
	// NB: there are other variables in PelletConfig but they're unused or unchanged

	TreasureRegion region; // determine which region all the settings below are for
	char* internal_name;   // mName
	char* archive_name;    // mArchive
	char* bmd_name;        // mBmd
	char* animMgr_name;    // mAnimMgr
	char* collTree_name;   // mColltree
	f32 bottom_radius;     // mRadius
	f32 pick_radius;       // mPRadius
	f32 cyl_height;        // mHeight
	f32 inertia_scaling;   // mInertiaScaling
	char* ptcl_type;       // mParticleType
	int num_ptcls;         // mNumParticles
	f32 ptcl_size;         // mParticleSize, unused
	f32 friction;          // mFriction (always 0.1?)
	int min_carriers;      // mMin
	int max_carriers;      // mMax
	char* dyn_type;        // mDynamics (lod or never)
	int pokos;             // mMoney
	char* unique;          // mUnique ("yes" or "no")
	char* indirect_mats;   // mIndirect ("yes", "use" or "no")
	int num_pmotions;      // mNumPMotions (never used)
	f32 bury_depth;        // mDepth (depth actually buried)
	f32 bury_depth_max;    // mDepthMax (buried this deep = completely hidden)
	f32 bury_radius_A;     // mDepthA (A-D = work radius at each reveal stage)
	f32 bury_radius_B;     // mDepthB
	f32 bury_radius_C;     // mDepthC
	f32 bury_radius_D;     // mDepthD
	u16 render_code;       // mCode (0=normal, 1=carry shadow/no breadbug, 2-3=post-tex)
	u16 piklopedia_num;    // mDictionary
	u8 indirect_state;     // mIndirectState (numeric version of indirect_mats)
};

void updatePelletConfig(Game::PelletConfig* thisConfig);

extern const u32 TREASURE_MAP_COUNT;
extern const LocalizationTreasureSwap treasureMap[];

} // namespace gz

#endif
