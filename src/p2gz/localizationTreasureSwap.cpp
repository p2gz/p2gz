
#include <p2gz/LocalizationTreasureSwap.h>

// include for ripped functions from pelletMgr.cpp
#include "Game/GameConfig.h"
#include "Game/gamePlayData.h"
#include "Game/GameSystem.h"
#include "Game/pelletMgr.h"
#include "Game/Stickers.h"
#include "Game/GameMessage.h"
#include "Game/gameStat.h"
#include "Game/DynParticle.h"
#include "Game/PlatInstance.h"
#include "Game/MoviePlayer.h"
#include "Game/AIConstants.h"
#include "Game/BaseHIO.h"
#include "Dolphin/rand.h"
#include "efx/TOtakara.h"
#include "PSM/Otakara.h"
#include "ObjectTypes.h"
#include "CollInfo.h"
#include "VsOtakaraName.h"
#include "JSystem/J3D/J3DModelLoader.h"
#include "nans.h"
#include <Game/SingleGameSection.h>

#define TREASURE_MAP_COUNT ARRAY_SIZE(treasureMap)

LocalizationTreasureSwap treasureMap[] = {
	{
	    System::LANG_Japanese,          // region
	    "g_futa_kajiwara",              // internal name
	    "g_futa_kajiwara.szs",          // archive filename
	    "g_futa_kajiwara.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    0.5,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    172,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_English,           // region
	    "g_futa_kajiwara",              // internal name
	    "g_futa_kajiwara.szs",          // archive filename
	    "us_lip_chapstick.bmd",         // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    14.0,                           // radius
	    10.0,                           // pradius
	    10.0,                           // height
	    160.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    6,                              // min carry
	    12,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    10.0,                           // depth B (for burying stuff)
	    10.0,                           // depth C (for burying stuff)
	    10.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    167,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_French,            // region
	    "g_futa_kajiwara",              // internal name
	    "g_futa_kajiwara.szs",          // archive filename
	    "pal_kan_hawesta.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    40.0,                           // radius
	    40.0,                           // pradius
	    35.0,                           // height
	    1000.0,                         // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    130,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    172,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_English,           // region
	    "fire_helmet",                  // internal name
	    "fire_helmet.szs",              // archive filename
	    "toy_fire_helmet.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    38.0,                           // radius
	    38.0,                           // pradius
	    20.0,                           // height
	    230.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    175,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_French,            // region
	    "fire_helmet",                  // internal name
	    "fire_helmet.szs",              // archive filename
	    "us_kan_skippy.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    30.0,                           // radius
	    30.0,                           // pradius
	    15.0,                           // height
	    350.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    130,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    45.0,                           // max depth (for burying stuff)
	    40.0,                           // depth A (for burying stuff)
	    40.0,                           // depth B (for burying stuff)
	    40.0,                           // depth C (for burying stuff)
	    40.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    183,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_Japanese,          // region
	    "fire_helmet",                  // internal name
	    "fire_helmet.szs",              // archive filename
	    "toy_fire_helmet.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    38.0,                           // radius
	    38.0,                           // pradius
	    20.0,                           // height
	    230.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    183,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_English,           // region
	    "g_futa_hiruzen",               // internal name
	    "g_futa_hiruzen.szs",           // archive filename
	    "us_futa_carmex.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    7.0,                            // height
	    200.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    6,                              // min carry
	    10,                             // max carry
	    "lod",                          // dynamics
	    60,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    168,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_French,            // region
	    "g_futa_hiruzen",               // internal name
	    "g_futa_hiruzen.szs",           // archive filename
	    "pal_cap_vitamalz.bmd",         // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    17.0,                           // radius
	    17.0,                           // pradius
	    4.0,                            // height
	    150.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    10.0,                           // depth B (for burying stuff)
	    10.0,                           // depth C (for burying stuff)
	    10.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    178,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    System::LANG_Japanese,          // region
	    "g_futa_hiruzen",               // internal name
	    "g_futa_hiruzen.szs",           // archive filename
	    "g_futa_hiruzen.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    178,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Massive Lid
	    System::LANG_English,           // region
	    "g_futa_kyusyu",                // internal name
	    "g_futa_kyusyu.szs",            // archive filename
	    "us_cap_yoohoo.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    17.0,                           // radius
	    17.0,                           // pradius
	    4.0,                            // height
	    150.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    10.0,                           // depth B (for burying stuff)
	    10.0,                           // depth C (for burying stuff)
	    10.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    178,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Massive Lid in PAL (this is actually Survival Container)
	    System::LANG_French,            // region
	    "g_futa_sikoku",                // internal name
	    "g_futa_sikoku.szs",            // archive filename
	    "us_kan_clabbergirl.bmd",       // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    25.0,                           // pradius
	    12.0,                           // height
	    300.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    130,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    60.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    163,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Massive Lid in JP
	    System::LANG_Japanese,          // region
	    "g_futa_kyusyu",                // internal name
	    "g_futa_kyusyu.szs",            // archive filename
	    "g_futa_kyusyu.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    170,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Happiness Emblem
	    System::LANG_English,           // region
	    "g_futa_sikoku",                // internal name
	    "g_futa_sikoku.szs",            // archive filename
	    "us_cap_squirt.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    17.0,                           // radius
	    17.0,                           // pradius
	    4.0,                            // height
	    150.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    10.0,                           // depth B (for burying stuff)
	    10.0,                           // depth C (for burying stuff)
	    10.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    180,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Happiness Emblem (JP)
	    System::LANG_Japanese,          // region
	    "g_futa_sikoku",                // internal name
	    "g_futa_sikoku.szs",            // archive filename
	    "g_futa_sikoku.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "complex",                      // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    170,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Happiness Emblem (PAL - This is Drone Supplies)
	    System::LANG_French,            // region
	    "g_futa_kyusyu",                // internal name
	    "g_futa_kyusyu.szs",            // archive filename
	    "pal_sweets_haribo.bmd",        // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    13.0,                           // height
	    600.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    5.0,                            // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    172,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
};

namespace LocalizationTreasureSwapFunc {

// GK2 is dumb for many reasons, but one of them is the internal name for treasues is not the
// Same between regions (unlike every other treasure), so we check for that here
// Replace the US/JP internal name with the PAL version, and vice versa depending on the current version
char* gk2TreasureNameSwap(char* oldTreasureName)
{
	// If not either of the 2 trouble treasures tied to GK, don't bother with anything below
	if (!(!strcmp(oldTreasureName, "g_futa_kyusyu") || !strcmp(oldTreasureName, "g_futa_sikoku"))) {
		return oldTreasureName;
	}
	// Check if we are in GK
	Game::BaseGameSection* section = Game::gameSystem->getSection();
	if (section && section->getCaveID() == 'y_02') { // glutton's kitchen
		Game::RoomMapMgr* roomMgr = static_cast<Game::RoomMapMgr*>(Game::mapMgr);
		if (roomMgr && roomMgr->mCaveInfo) {
			// If GK2
			if (section->getCurrFloor() + 1 == 2) {
				// If PAL version
				if (global_language_swap->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
						// Return PAL name
						return "g_futa_sikoku";
					}
				}
				// Otherwise (US/JP version)
				else {
					// If the current treasure name is the PAL internal name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						// Return US/JP name
						return "g_futa_kyusyu";
					}
				}
			}
			// If GK4
			else if (section->getCurrFloor() + 1 == 4) {
				// If PAL version
				if (global_language_swap->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						// Return PAL name
						return "g_futa_kyusyu";
					}
				}
				// Otherwise (US/JP version)
				else {
					// If the current treasure name is the PAL internal name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
						// Return US/JP name
						return "g_futa_sikoku";
					}
				}
			}
		}
	}
	// Not relevant, don't change name
	return oldTreasureName;
}

void updatePelletConfig(Game::PelletConfig* thisConfig)
{
	OSReport("Cur treasure: %s\n\n", thisConfig->mParams.mName.mData);
	for (int i = 0; i < TREASURE_MAP_COUNT; i++) {
		OSReport("Sys region: %d P2GZ region: %d Cur treasure map region: %d\n", sys->mRegion, global_language_swap->get_treasure_region(),
		         treasureMap[i].region);
		OSReport("gk treasure swap: %s\n", gk2TreasureNameSwap(thisConfig->mParams.mName.mData));
		// Loop through all treasures in the map to find the one we need to edit
		if (!strcmp(gk2TreasureNameSwap(thisConfig->mParams.mName.mData), treasureMap[i].mName)) {
			// This is the treasure we need; now check if it's for the right version
			if (global_language_swap->get_treasure_region() == treasureMap[i].region) {
				OSReport("Region difference detected! Converting...\n");
				// Update config info here
				thisConfig->mParams.mArchive.mData        = treasureMap[i].mArchive;
				thisConfig->mParams.mBmd.mData            = treasureMap[i].mBmd;
				thisConfig->mParams.mAnimMgr.mData        = treasureMap[i].mAnimMgr;
				thisConfig->mParams.mColltree.mData       = treasureMap[i].mCollTree;
				thisConfig->mParams.mRadius.mData         = treasureMap[i].mRadius;
				thisConfig->mParams.mPRadius.mData        = treasureMap[i].mPRadius;
				thisConfig->mParams.mHeight.mData         = treasureMap[i].mHeight;
				thisConfig->mParams.mInertiaScaling.mData = treasureMap[i].mIntertialScaling;
				thisConfig->mParams.mParticleType.mData   = treasureMap[i].mParticleType;
				thisConfig->mParams.mNumParticles.mData   = treasureMap[i].mNumParticles;
				thisConfig->mParams.mParticleSize.mData   = treasureMap[i].mParticleSize;
				thisConfig->mParams.mFriction.mData       = treasureMap[i].mFriction;
				thisConfig->mParams.mMin.mData            = treasureMap[i].mMin;
				thisConfig->mParams.mMax.mData            = treasureMap[i].mMax;
				thisConfig->mParams.mDynamics.mData       = treasureMap[i].mDynamics;
				thisConfig->mParams.mMoney.mData          = treasureMap[i].mMoney;
				thisConfig->mParams.mUnique.mData         = treasureMap[i].mUnique;
				thisConfig->mParams.mIndirect.mData       = treasureMap[i].mIndirect;
				thisConfig->mParams.mNumPMotions.mData    = treasureMap[i].mNumPMotions;
				thisConfig->mParams.mDepth.mData          = treasureMap[i].mDepth;
				thisConfig->mParams.mDepthMax.mData       = treasureMap[i].mDepthMax;
				thisConfig->mParams.mDepthA.mData         = treasureMap[i].mDepthA;
				thisConfig->mParams.mDepthB.mData         = treasureMap[i].mDepthB;
				thisConfig->mParams.mDepthC.mData         = treasureMap[i].mDepthC;
				thisConfig->mParams.mDepthD.mData         = treasureMap[i].mDepthD;
				thisConfig->mParams.mCode.mData           = treasureMap[i].mCode;
				thisConfig->mParams.mDictionary.mData     = treasureMap[i].mDictionary;
				thisConfig->mParams.mIndirectState        = treasureMap[i].mIndirectState;
				// We did what we need to, just end the function
				return;
			}
		}
	}
	// If we make it here, treasure doesn't have a valid map (is same in all versions)
	OSReport("No region difference\n");
}

} // namespace LocalizationTreasureSwapFunc

// Add functions from pelletMgr.cpp below that we need to change for treasure swapping; these functions are equivalent but pelletMgr.cpp is
// not-matching, so yehaw time it is

namespace Game {

void BasePelletMgr::load()
{
	char buffer[512];
	char* file = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(158, 0, global_language_swap->get_treasure_region(), System::LANG_Spanish);
		switch (global_language_swap->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			file = buffer;
			break;
		case System::LANG_English:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			file = buffer;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "pal");
			file = buffer;
			break;
		}
	} else {
		file = "user/Kando/pellet/";
	}

	char buffer2[512];

	for (int i = 0; i < mConfigList->mConfigCnt; i++) {

		PelletConfig* config   = &mConfigList->mConfigs[i];
		JKRArchive* archive    = nullptr;
		config->mParams.mIndex = i;
		char* archiveName      = config->mParams.mArchive.mData;
		if (strcmp("null", archiveName)) {
			sprintf(buffer2, "%s%s", file, config->mParams.mArchive.mData);
			archive = JKRMountArchive(buffer2, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
		}

		J3DModelData* data = nullptr;

		if (strcmp("null", config->mParams.mBmd.mData) == 0) {
			mModelData[i] = nullptr;
		} else {
			sprintf(buffer2, "%s", config->mParams.mBmd.mData);

			void* resource = JKRFileLoader::getGlbResource(buffer2, nullptr);
			if (resource == nullptr) {
				JUT_PANICLINE(4560, "meck ** %s : is not foun !\n", buffer2);
			}

			u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_Material_UseIndirect | J3DMLF_18 | J3DMLF_UseImmediateMtx;
			if (config->mParams.mCode.mData & 2) {
				flags |= J3DMLF_UsePostTexMtx;
			}
			data = J3DModelLoaderDataBase::load(resource, flags);

			if (config->mParams.mCode.mData & 2) {
				for (u16 i = 0; i < data->getShapeNum(); i++) {
					data->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
				}
			}

			mModelData[i] = data;
		}

		if (config->mParams.mAnimMgr.mData) {
			sprintf(buffer2, "%s%s", file, config->mParams.mAnimMgr.mData);
			mAnimMgr[i] = SysShape::AnimMgr::load(buffer2, data, archive);
		}

		if (config->mParams.mColltree.mData) {
			sprintf(buffer2, "%s%s", file, config->mParams.mColltree.mData);
			mCollParts[i] = CollPartFactory::load(buffer2);
		}
	}
}

/**
 * @note Address: 0x8016C0CC
 * @note Size: 0x3E4
 */
// WIP: https://decomp.me/scratch/Ltrad
void BasePelletMgr::load_texArc(char* filename)
{
	char buffer[512];
	char* directory = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(244, 0, global_language_swap->get_treasure_region(), System::LANG_Spanish);
		switch (global_language_swap->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			directory = buffer;
			break;
		case System::LANG_English:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			directory = buffer;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "pal");
			directory = buffer;
			break;
		}
	} else {
		directory = "user/Kando/pellet/";
	}

	char path[512];
	sprintf(path, "%s%s", directory, filename);

	JKRArchive* textArc = openTextArc(path);
	JUT_ASSERTLINE(4728, textArc, "%s: not found !\n", path);

	for (int i = 0; i < mConfigList->mConfigCnt; i++) {
		PelletConfig* config = &mConfigList->mConfigs[i];

		// @P2GZ localization-swap: non-matching inline, we're just gonna plop it in here and assume it works
		P2ASSERTBOUNDSLINE(4425, 0, i, mEntries);
		if (mIsUsedList[i] != false) {
			config->mParams.mIndex = i;

			JKRArchive* archive = nullptr;
			if (strcmp("null", config->mParams.mArchive.mData)) {
				sprintf(path, "%s%s", directory, config->mParams.mArchive.mData);
				archive = JKRMountArchive(path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
			}

			J3DModelData* data = nullptr;

			if (strcmp("null", config->mParams.mBmd.mData) == 0) {
				mModelData[i] = nullptr;
			} else {
				JUT_ASSERTLINE(4776, archive, "archive not found\n");
				sprintf(path, "%s", config->mParams.mBmd.mData);
				void* resourceLoad = JKRFileLoader::getGlbResource(path, nullptr);
				void* resource     = resourceLoad;
				if (resourceLoad == nullptr) {
					resource = archive->getResource(path);
					JUT_PANICLINE(4786, "%s : is not foun !\n", path);
				}

				if (config->mParams.mIndirectState != PelletConfig::Indirect_No) {
					u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_Material_UseIndirect | J3DMLF_18 | J3DMLF_UseImmediateMtx;
					if (config->mParams.mCode.mData & 2) {
						flags |= J3DMLF_UsePostTexMtx;
					}
					data = J3DModelLoaderDataBase::load(resource, flags);
				} else {
					u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_18 | J3DMLF_UseImmediateMtx;
					if (config->mParams.mCode.mData & 2) {
						flags |= J3DMLF_UsePostTexMtx;
					}
					data = J3DModelLoaderDataBase::load(resource, flags);
				}

				mModelData[i] = data;

				if (config->mParams.mCode.mData & 2) {
					for (u16 i = 0; i < data->getShapeNum(); i++) {
						data->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
					}
				}
			}

			if (config->mParams.mAnimMgr.mData != 0) {
				sprintf(path, "%s/%s", LocalizationTreasureSwapFunc::gk2TreasureNameSwap(config->mParams.mName.mData),
				        config->mParams.mAnimMgr.mData);
				mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				if (mAnimMgr[i] == nullptr) {
					mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				}
			}

			if (config->mParams.mColltree.mData != 0) {
				sprintf(path, "%s/%s", LocalizationTreasureSwapFunc::gk2TreasureNameSwap(config->mParams.mName.mData),
				        config->mParams.mColltree.mData);
				mCollParts[i] = CollPartFactory::load(textArc, path);
			}
		}
	}
	closeTextArc(textArc);
}

/**
 * @note Address: 0x8016C4B0
 * @note Size: 0x10C
 */
JKRArchive* BasePelletMgr::openTextArc(char* arc)
{
	char directory[512];
	char* file = nullptr;
	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(350, 0, global_language_swap->get_treasure_region(), System::LANG_Spanish);
		switch (global_language_swap->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(directory, "/user/Abe/Pellet/%s/", "jpn");
			file = directory;
			break;
		case System::LANG_English:
			sprintf(directory, "/user/Abe/Pellet/%s/", "us");
			file = directory;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(directory, "/user/Abe/Pellet/%s/", "pal");
			file = directory;
			break;
		}
	} else {
		file = "user/Kando/pellet/";
	}

	char filePath[512];

	sprintf(filePath, "%s/%s", file, arc);
	return JKRMountArchive(filePath, JKRArchive::EMM_Mem, JKRHeap::sCurrentHeap, JKRArchive::EMD_Tail);
}

} // namespace Game
