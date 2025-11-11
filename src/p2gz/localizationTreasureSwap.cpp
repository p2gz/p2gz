
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
	//////////////////// VALLEY OF REPOSE ////////////////////
	//// Above Ground - 1
	// Courage Reactor - different in JP
	{
	    // Courage Reactor
	    System::LANG_English,           // region
	    "denchi_1_red",                 // internal name
	    "denchi_1_red.szs",             // archive filename
	    "us_denchi_1.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    8.0,                            // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    280,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    7.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    159,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Courage Reactor
	    System::LANG_French,            // region
	    "denchi_1_red",                 // internal name
	    "denchi_1_red.szs",             // archive filename
	    "us_denchi_1.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    8.0,                            // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    280,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    7.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    157,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Love and Courage Reactor
	    System::LANG_Japanese,          // region
	    "denchi_1_red",                 // internal name
	    "denchi_1_red.szs",             // archive filename
	    "denchi_1_red.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    8.0,                            // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    280,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    7.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    159,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Subterranean Complex - 1
	// Thirst Activator (SCx7) - different for all three
	{
	    // Thirst Activator
	    System::LANG_English,           // region
	    "denchi_1_black",               // internal name
	    "denchi_1_black.szs",           // archive filename
	    "us_futa_treetop.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    35,                             // max carry
	    "lod",                          // dynamics
	    300,                            // poko value
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
	    171,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Perfect Container
	    System::LANG_French,            // region
	    "denchi_1_black",               // internal name
	    "denchi_1_black.szs",           // archive filename
	    "us_kan_beachcliff.bmd",        // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    57.0,                           // radius
	    57.0,                           // pradius
	    10.0,                           // height
	    500.0,                          // inertial scaling
	    "simple",                       // particle type
	    16,                             // number of particle
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
	    20.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    160,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Alternative Reactor
	    System::LANG_Japanese,          // region
	    "denchi_1_black",               // internal name
	    "denchi_1_black.szs",           // archive filename
	    "denchi_1_black.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    10.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    35,                             // max carry
	    "lod",                          // dynamics
	    300,                            // poko value
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
	    160,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Frontier Cavern - None

	////////////////////////////////////////////////////////

	//////////////////// AWAKENING WOOD ////////////////////
	//// Above Ground - 1
	// Healing Cask - different in all three
	{
	    // Healing Cask
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
	    // Hypnotic Platter
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
	    // Seat of Enlightenment
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

	//// Hole of Beasts - None

	//// White Flower Garden - 2
	// Alien Billboard - different in PAL
	{
	    // Alien Billboard
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
	    // Nutrient Silo
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
	    // Alien Billboard
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
	// Survival Ointment (WFG4) - different for all three
	{
	    // Survival Ointment
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
	    // Container of Sea Bounty
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
	    // Paper Slider
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

	//// Bulblax Kingdom - None

	//// Snagret Hole - 2
	// Stupendous Lens (SH6) - different in all three
	{
	    // Stupendous Lens
	    System::LANG_English,            // region
	    "bey_goma",                      // internal name
	    "bey_goma.szs",                  // archive filename
	    "us_musi_lens.bmd",              // bmd filename
	    "animmgr.txt",                   // anmmgr filename
	    "collInfo.txt",                  // collinfo filename
	    25.0,                            // radius
	    25.0,                            // pradius
	    12.0,                            // height
	    250.0,                           // inertial scaling
	    "simple",                        // particle type
	    8,                               // number of particle
	    1.0,                             // particle size
	    0.1,                             // friction
	    10,                              // min carry
	    15,                              // max carry
	    "lod",                           // dynamics
	    120,                             // poko value
	    "yes",                           // unqiue
	    "yes",                           // indirect
	    0,                               // num of pmotions
	    0.0,                             // depth (for burying stuff)
	    12.0,                            // max depth (for burying stuff)
	    10.0,                            // depth A (for burying stuff)
	    50.0,                            // depth B (for burying stuff)
	    50.0,                            // depth C (for burying stuff)
	    50.0,                            // depth D (for burying stuff)
	    0,                               // code (special properties)
	    172,                             // index in piklopedia
	    Game::PelletConfig::Indirect_Yes // indirect state
	},
	{
	    // Mysterious Remains
	    System::LANG_French,            // region
	    "bey_goma",                     // internal name
	    "bey_goma.szs",                 // archive filename
	    "us_kani.bmd",                  // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    18.0,                           // pradius
	    15.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    150,                            // poko value
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
	    136,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Ultimate Spinner
	    System::LANG_Japanese,          // region
	    "bey_goma",                     // internal name
	    "bey_goma.szs",                 // archive filename
	    "bey_goma.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    18.0,                           // radius
	    18.0,                           // pradius
	    12.0,                           // height
	    100.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    120,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    12.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    136,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Salivatrix (SH6) - different in JP
	{
	    // Salivtrix
	    System::LANG_English,           // region
	    "g_futa_kyodo",                 // internal name
	    "g_futa_kyodo.szs",             // archive filename
	    "us_futa_dannon.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    42.0,                           // radius
	    42.0,                           // pradius
	    10.0,                           // height
	    300.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    173,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Salivtrix
	    System::LANG_French,            // region
	    "g_futa_kyodo",                 // internal name
	    "g_futa_kyodo.szs",             // archive filename
	    "us_futa_dannon.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    42.0,                           // radius
	    42.0,                           // pradius
	    10.0,                           // height
	    300.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    30,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    173,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Highly Logical Money
	    System::LANG_Japanese,          // region
	    "g_futa_kyodo",                 // internal name
	    "g_futa_kyodo.szs",             // archive filename
	    "g_futa_kyodo.bmd",             // bmd filename
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
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    168,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	////////////////////////////////////////////////////////

	/////////////////// PERPLEXING POOL ////////////////////
	//// Above Ground - 3
	// Impediment Scourge - different in JP
	{
	    // Impediment Scourge
	    System::LANG_English,           // region
	    "yoyo_blue",                    // internal name
	    "yoyo_blue.szs",                // archive filename
	    "us_sen_nuki.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    10.0,                           // radius
	    10.0,                           // pradius
	    5.0,                            // height
	    130.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    31.0,                           // max depth (for burying stuff)
	    37.0,                           // depth A (for burying stuff)
	    37.0,                           // depth B (for burying stuff)
	    37.0,                           // depth C (for burying stuff)
	    37.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    74,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Impediment Scourge
	    System::LANG_French,            // region
	    "yoyo_blue",                    // internal name
	    "yoyo_blue.szs",                // archive filename
	    "us_sen_nuki.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    10.0,                           // radius
	    10.0,                           // pradius
	    5.0,                            // height
	    130.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    31.0,                           // max depth (for burying stuff)
	    37.0,                           // depth A (for burying stuff)
	    37.0,                           // depth B (for burying stuff)
	    37.0,                           // depth C (for burying stuff)
	    37.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    74,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Lightning Bolt
	    System::LANG_Japanese,          // region
	    "yoyo_blue",                    // internal name
	    "yoyo_blue.szs",                // archive filename
	    "yoyo_blue.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    10.0,                           // radius
	    10.0,                           // pradius
	    5.0,                            // height
	    90.0,                           // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    3,                              // min carry
	    5,                              // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    31.0,                           // max depth (for burying stuff)
	    37.0,                           // depth A (for burying stuff)
	    37.0,                           // depth B (for burying stuff)
	    37.0,                           // depth C (for burying stuff)
	    37.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    73,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Optical Illusion - different in all three
	{
	    // Optical Illusion
	    System::LANG_English,           // region
	    "denchi_2_red",                 // internal name
	    "denchi_2_red.szs",             // archive filename
	    "us_futa_ragu.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    140,                            // poko value
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
	    170,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Abstract Masterpiece
	    System::LANG_French,            // region
	    "denchi_2_red",                 // internal name
	    "denchi_2_red.szs",             // archive filename
	    "pal_cap_pscttitt.bmd",         // bmd filename
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
	    179,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Yell Battery
	    System::LANG_Japanese,          // region
	    "denchi_2_red",                 // internal name
	    "denchi_2_red.szs",             // archive filename
	    "denchi_2_red.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    18.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    140,                            // poko value
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
	    157,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Gherkin Gate - different in JP
	{
	    // Gherkin Gate
	    System::LANG_English,           // region
	    "g_futa_daisen",                // internal name
	    "g_futa_daisen.szs",            // archive filename
	    "us_futa_vlasic.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    10.0,                           // height
	    250.0,                          // inertial scaling
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
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    174,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Gherkin Gate
	    System::LANG_French,            // region
	    "g_futa_daisen",                // internal name
	    "g_futa_daisen.szs",            // archive filename
	    "us_futa_vlasic.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    10.0,                           // height
	    250.0,                          // inertial scaling
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
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    175,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Idea Assistant
	    System::LANG_Japanese,          // region
	    "g_futa_daisen",                // internal name
	    "g_futa_daisen.szs",            // archive filename
	    "g_futa_daisen.bmd",            // bmd filename
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
	    177,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Citadel of Spiders - 4
	// Creative Inspiration (CoS1) - different in all three
	{
	    // Creative Inspiration
	    System::LANG_English,            // region
	    "g_futa_titiyas",                // internal name
	    "g_futa_titiyas.szs",            // archive filename
	    "us_cap_rccola.bmd",             // bmd filename
	    "animmgr.txt",                   // anmmgr filename
	    "collInfo.txt",                  // collinfo filename
	    17.0,                            // radius
	    17.0,                            // pradius
	    4.0,                             // height
	    150.0,                           // inertial scaling
	    "complex",                       // particle type
	    8,                               // number of particle
	    1.0,                             // particle size
	    0.1,                             // friction
	    4,                               // min carry
	    8,                               // max carry
	    "lod",                           // dynamics
	    100,                             // poko value
	    "yes",                           // unqiue
	    "use",                           // indirect
	    0,                               // num of pmotions
	    0.0,                             // depth (for burying stuff)
	    14.0,                            // max depth (for burying stuff)
	    10.0,                            // depth A (for burying stuff)
	    10.0,                            // depth B (for burying stuff)
	    10.0,                            // depth C (for burying stuff)
	    10.0,                            // depth D (for burying stuff)
	    0,                               // code (special properties)
	    183,                             // index in piklopedia
	    Game::PelletConfig::Indirect_Use // indirect state
	},
	{
	    // Activity Arouser
	    System::LANG_French,            // region
	    "g_futa_titiyas",               // internal name
	    "g_futa_titiyas.szs",           // archive filename
	    "pal_cap_sinalco.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    22.0,                           // radius
	    15.0,                           // pradius
	    15.0,                           // height
	    250.0,                          // inertial scaling
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
	    181,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Milky Figure
	    System::LANG_Japanese,          // region
	    "g_futa_titiyas",               // internal name
	    "g_futa_titiyas.szs",           // archive filename
	    "g_futa_titiyas.bmd",           // bmd filename
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
	    169,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Lip Service (CoS2) - different in JP
	{
	    // Lip Service
	    System::LANG_English,           // region
	    "kan_b_gold",                   // internal name
	    "kan_b_gold.szs",               // archive filename
	    "us_kuchi_beni.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    15.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    110.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    45.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    77,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Lip Service
	    System::LANG_French,            // region
	    "kan_b_gold",                   // internal name
	    "kan_b_gold.szs",               // archive filename
	    "us_kuchi_beni.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    15.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    110.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    4,                              // min carry
	    8,                              // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    45.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    76,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Family Raft
	    System::LANG_Japanese,          // region
	    "kan_b_gold",                   // internal name
	    "kan_b_gold.szs",               // archive filename
	    "dust_kan_futa_b_gold.bmd",     // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    12.0,                           // radius
	    8.0,                            // pradius
	    5.0,                            // height
	    100.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    2,                              // min carry
	    3,                              // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    45.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    74,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Patience Tester (CoS3) - different in US
	{
	    // Patience Tester
	    System::LANG_English,           // region
	    "kan_nichiro",                  // internal name
	    "kan_nichiro.szs",              // archive filename
	    "us_kan_sunluck.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    41.0,                           // radius
	    41.0,                           // pradius
	    48.0,                           // height
	    500.0,                          // inertial scaling
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
	    48.0,                           // max depth (for burying stuff)
	    41.0,                           // depth A (for burying stuff)
	    41.0,                           // depth B (for burying stuff)
	    41.0,                           // depth C (for burying stuff)
	    41.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    162,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Open Architecture
	    System::LANG_French,            // region
	    "kan_nichiro",                  // internal name
	    "kan_nichiro.szs",              // archive filename
	    "kan_nichiro.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    41.0,                           // radius
	    41.0,                           // pradius
	    48.0,                           // height
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
	    24.0,                           // depth (for burying stuff)
	    48.0,                           // max depth (for burying stuff)
	    41.0,                           // depth A (for burying stuff)
	    41.0,                           // depth B (for burying stuff)
	    41.0,                           // depth C (for burying stuff)
	    41.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    165,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Open Architecture
	    System::LANG_Japanese,          // region
	    "kan_nichiro",                  // internal name
	    "kan_nichiro.szs",              // archive filename
	    "kan_nichiro.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    41.0,                           // radius
	    41.0,                           // pradius
	    48.0,                           // height
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
	    24.0,                           // depth (for burying stuff)
	    48.0,                           // max depth (for burying stuff)
	    41.0,                           // depth A (for burying stuff)
	    41.0,                           // depth B (for burying stuff)
	    41.0,                           // depth C (for burying stuff)
	    41.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    162,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Flame of Tomorrow (CoS4) - different in JP
	{
	    // Flame of Tomorrow
	    System::LANG_English,           // region
	    "tape_blue",                    // internal name
	    "tape_blue.szs",                // archive filename
	    "us_match.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    15.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    33.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    30.0,                           // depth B (for burying stuff)
	    30.0,                           // depth C (for burying stuff)
	    30.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    73,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Flame of Tomorrow
	    System::LANG_French,            // region
	    "tape_blue",                    // internal name
	    "tape_blue.szs",                // archive filename
	    "us_match.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    15.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    33.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    30.0,                           // depth B (for burying stuff)
	    30.0,                           // depth C (for burying stuff)
	    30.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    73,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Flame of Tomorrow
	    System::LANG_Japanese,          // region
	    "tape_blue",                    // internal name
	    "tape_blue.szs",                // archive filename
	    "vinyl_tape_blue.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    15.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    33.0,                           // max depth (for burying stuff)
	    30.0,                           // depth A (for burying stuff)
	    30.0,                           // depth B (for burying stuff)
	    30.0,                           // depth C (for burying stuff)
	    30.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    72,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Glutton's Kitchen - 3
	// Massive Lid (GK2) - different in all three
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
	    // Survival Container
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
	    // Milk Cover
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

	// Happiness Emblem (GK4) - different in all three
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
	    // Drone Supplies
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
	{
	    // Good Old Memories
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

	//// Shower Room - 7
	// Merciless Extractor (SR1) - different in US
	{
	    // Merciless Extractor
	    System::LANG_English,           // region
	    "otama",                        // internal name
	    "otama.szs",                    // archive filename
	    "us_lemon_sibori.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    50.0,                           // radius
	    50.0,                           // pradius
	    21.0,                           // height
	    530.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    19.0,                           // max depth (for burying stuff)
	    35.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    61,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Divine Cooking Tool
	    System::LANG_French,            // region
	    "otama",                        // internal name
	    "otama.szs",                    // archive filename
	    "otama.bmd",                    // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    19.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    19.0,                           // max depth (for burying stuff)
	    35.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    62,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Divine Cooking Tool
	    System::LANG_Japanese,          // region
	    "otama",                        // internal name
	    "otama.szs",                    // archive filename
	    "otama.bmd",                    // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    19.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    19.0,                           // max depth (for burying stuff)
	    35.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    61,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Durable Energy Cell (SR2) - different in JP
	{
	    // Durable Energy Cell
	    System::LANG_English,           // region
	    "denchi_2_black",               // internal name
	    "denchi_2_black.szs",           // archive filename
	    "us_denchi_2.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    18.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    160,                            // poko value
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
	    158,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Durable Energy Cell
	    System::LANG_French,            // region
	    "denchi_2_black",               // internal name
	    "denchi_2_black.szs",           // archive filename
	    "us_denchi_2.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    18.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    160,                            // poko value
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
	    156,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Revised Eternal Fuel Dynamo
	    System::LANG_Japanese,          // region
	    "denchi_2_black",               // internal name
	    "denchi_2_black.szs",           // archive filename
	    "denchi_2_black.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    18.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    260.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    15,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    160,                            // poko value
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
	    158,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Arboreal Frippery (SR4) - different in PAL
	{
	    // Arboreal Frippery
	    System::LANG_English,           // region
	    "momiji_normal",                // internal name
	    "momiji_normal.szs",            // archive filename
	    "momiji_nomal.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    5.0,                            // radius
	    5.0,                            // pradius
	    5.0,                            // height
	    100.0,                          // inertial scaling
	    "simple",                       // particle type
	    0,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    1,                              // min carry
	    1,                              // max carry
	    "never",                        // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    19,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Arboreal Frippery
	    System::LANG_French,            // region
	    "momiji_normal",                // internal name
	    "momiji_normal.szs",            // archive filename
	    "pal_momiji_red.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    5.0,                            // radius
	    5.0,                            // pradius
	    5.0,                            // height
	    100.0,                          // inertial scaling
	    "simple",                       // particle type
	    0,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    1,                              // min carry
	    1,                              // max carry
	    "never",                        // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    19,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Arboreal Frippery
	    System::LANG_Japanese,          // region
	    "momiji_normal",                // internal name
	    "momiji_normal.szs",            // archive filename
	    "momiji_nomal.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    5.0,                            // radius
	    5.0,                            // pradius
	    5.0,                            // height
	    100.0,                          // inertial scaling
	    "simple",                       // particle type
	    0,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    1,                              // min carry
	    1,                              // max carry
	    "never",                        // dynamics
	    10,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    19,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Broken Food Master (SR5) - different in all three
	{
	    // Broken Food Master
	    System::LANG_English,           // region
	    "toy_lady",                     // internal name
	    "toy_lady.szs",                 // archive filename
	    "otama.bmd",                    // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    19.0,                           // height
	    400.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    19.0,                           // max depth (for burying stuff)
	    35.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    62,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Merciless Extractor
	    System::LANG_French,            // region
	    "toy_lady",                     // internal name
	    "toy_lady.szs",                 // archive filename
	    "us_lemon_sibori.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    50.0,                           // radius
	    50.0,                           // pradius
	    21.0,                           // height
	    530.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    90,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    19.0,                           // max depth (for burying stuff)
	    35.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    61,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Broken Cooking God
	    System::LANG_Japanese,          // region
	    "toy_lady",                     // internal name
	    "toy_lady.szs",                 // archive filename
	    "funny_toy_lady.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    23.0,                           // pradius
	    13.0,                           // height
	    200.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    16,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    14.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    35.0,                           // depth B (for burying stuff)
	    35.0,                           // depth C (for burying stuff)
	    35.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    60,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Endless Repository (SR5) - different in US
	{
	    // Endless Repository
	    System::LANG_English,           // region
	    "kan_iwate",                    // internal name
	    "kan_iwate.szs",                // archive filename
	    "us_kan_beachcliff.bmd",        // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    57.0,                           // radius
	    57.0,                           // pradius
	    23.0,                           // height
	    500.0,                          // inertial scaling
	    "simple",                       // particle type
	    16,                             // number of particle
	    5.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    30,                             // max carry
	    "lod",                          // dynamics
	    130,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    20.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    163,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Permanent Container
	    System::LANG_French,            // region
	    "kan_iwate",                    // internal name
	    "kan_iwate.szs",                // archive filename
	    "kan_iwate.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    20.0,                           // height
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
	    20.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    166,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Permanent Container
	    System::LANG_Japanese,          // region
	    "kan_iwate",                    // internal name
	    "kan_iwate.szs",                // archive filename
	    "kan_iwate.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    20.0,                           // height
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
	    20.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    163,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Pondering Emblem (SR5) - different in all three
	{
	    // Pondering Emblem
	    System::LANG_English,           // region
	    "g_futa_kitaama",               // internal name
	    "g_futa_kitaama.szs",           // archive filename
	    "us_cap_yoohoo_choco.bmd",      // bmd filename
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
	    179,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Happiness Emblem
	    System::LANG_French,            // region
	    "g_futa_kitaama",               // internal name
	    "g_futa_kitaama.szs",           // archive filename
	    "pal_cap_fristi.bmd",           // bmd filename
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
	    // Milky Cradle
	    System::LANG_Japanese,          // region
	    "g_futa_kitaama",               // internal name
	    "g_futa_kitaama.szs",           // archive filename
	    "g_futa_kitaama.bmd",           // bmd filename
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
	    179,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Abstract Masterpiece (SR6) - different in all three
	{
	    // Abstract Masterpiece
	    System::LANG_English,           // region
	    "milk_cap",                     // internal name
	    "milk_cap.szs",                 // archive filename
	    "us_futa_snapple.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    6,                              // min carry
	    12,                             // max carry
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
	    169,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Plentiful Tank
	    System::LANG_French,            // region
	    "milk_cap",                     // internal name
	    "milk_cap.szs",                 // archive filename
	    "pal_kan_breitsamer.bmd",       // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    25.0,                           // pradius
	    12.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    16,                             // max carry
	    "lod",                          // dynamics
	    50,                             // poko value
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
	    171,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Estimated Object GF
	    System::LANG_Japanese,          // region
	    "milk_cap",                     // internal name
	    "milk_cap.szs",                 // archive filename
	    "g_futa_ohayo.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    20.0,                           // pradius
	    5.0,                            // height
	    200.0,                          // inertial scaling
	    "simple",                       // particle type
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
	    167,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Submerged Castle - 3
	// Activity Arouser (SmC3) - different in all three
	{
	    // Activity Arouser
	    System::LANG_English,           // region
	    "g_futa_nihonraku",             // internal name
	    "g_futa_nihonraku.szs",         // archive filename
	    "us_futa_wilson.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    42.0,                           // radius
	    42.0,                           // pradius
	    10.0,                           // height
	    300.0,                          // inertial scaling
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
	    176,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Pondering Emblem
	    System::LANG_French,            // region
	    "g_futa_nihonraku",             // internal name
	    "g_futa_nihonraku.szs",         // archive filename
	    "pal_cap_tizer.bmd",            // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    22.0,                           // radius
	    15.0,                           // pradius
	    15.0,                           // height
	    250.0,                          // inertial scaling
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
	    180,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Anywhere Floater
	    System::LANG_Japanese,          // region
	    "g_futa_nihonraku",             // internal name
	    "g_futa_nihonraku.szs",         // archive filename
	    "g_futa_nihon_raku.bmd",        // bmd filename
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
	    180,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Drone Supplies (SmC4) - different in US
	{
	    // Drone Supplies
	    System::LANG_English,           // region
	    "kan_imuraya",                  // internal name
	    "kan_imuraya.szs",              // archive filename
	    "us_kan_deviled.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    37.0,                           // radius
	    37.0,                           // pradius
	    13.0,                           // height
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
	    53.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    161,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Open Archive
	    System::LANG_French,            // region
	    "kan_imuraya",                  // internal name
	    "kan_imuraya.szs",              // archive filename
	    "kan_imuraya.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    53.0,                           // height
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
	    25.0,                           // depth (for burying stuff)
	    53.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    169,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Open Archive
	    System::LANG_Japanese,          // region
	    "kan_imuraya",                  // internal name
	    "kan_imuraya.szs",              // archive filename
	    "kan_imuraya.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    53.0,                           // height
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
	    25.0,                           // depth (for burying stuff)
	    53.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    166,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Proton AA (SmC4) - different in JP
	{
	    // Proton AA
	    System::LANG_English,           // region
	    "denchi_3_red",                 // internal name
	    "denchi_3_red.szs",             // archive filename
	    "us_denchi_3.bmd",              // bmd filename
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
	    157,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Proton AA
	    System::LANG_French,            // region
	    "denchi_3_red",                 // internal name
	    "denchi_3_red.szs",             // archive filename
	    "us_denchi_3.bmd",              // bmd filename
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
	    155,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Proton X
	    System::LANG_Japanese,          // region
	    "denchi_3_red",                 // internal name
	    "denchi_3_red.szs",             // archive filename
	    "denchi_3_red.bmd",             // bmd filename
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
	    155,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	////////////////////////////////////////////////////////

	///////////////////// WISTFUL WILD /////////////////////
	//// Above Ground - None

	//// Cavern of Chaos - 4
	// Mysterious Remains (CoC4) - different in all three
	{
	    // Mysterious Remains
	    System::LANG_English,           // region
	    "g_futa_koiwai",                // internal name
	    "g_futa_koiwai.szs",            // archive filename
	    "us_kani.bmd",                  // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    18.0,                           // pradius
	    15.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    150,                            // poko value
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
	    51,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Container of Knowledge
	    System::LANG_French,            // region
	    "g_futa_koiwai",                // internal name
	    "g_futa_koiwai.szs",            // archive filename
	    "pal_kan_connetable.bmd",       // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    50.0,                           // radius
	    50.0,                           // pradius
	    25.0,                           // height
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
	    159,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Understood Person Symbol
	    System::LANG_Japanese,          // region
	    "g_futa_koiwai",                // internal name
	    "g_futa_koiwai.szs",            // archive filename
	    "g_futa_koiwai.bmd",            // bmd filename
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
	    173,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Fuel Reservoir (CoC7) - different in JP
	{
	    // Fuel Reservoir
	    System::LANG_English,           // region
	    "denchi_3_black",               // internal name
	    "denchi_3_black.szs",           // archive filename
	    "us_denchi_9v.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    160.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    120,                            // poko value
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
	    160,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Fuel Reservoir
	    System::LANG_French,            // region
	    "denchi_3_black",               // internal name
	    "denchi_3_black.szs",           // archive filename
	    "us_denchi_9v.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    15.0,                           // pradius
	    10.0,                           // height
	    160.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    120,                            // poko value
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
	    158,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Fuel Reservoir
	    System::LANG_Japanese,          // region
	    "denchi_3_black",               // internal name
	    "denchi_3_black.szs",           // archive filename
	    "denchi_3_black.bmd",           // bmd filename
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
	    120,                            // poko value
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
	    156,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Fruit Guard (CoC8) - different in US
	{
	    // Fruit Guard
	    System::LANG_English,           // region
	    "kan_kyokuyo",                  // internal name
	    "kan_kyokuyo.szs",              // archive filename
	    "us_kan_treetop.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    22.0,                           // radius
	    22.0,                           // pradius
	    12.0,                           // height
	    300.0,                          // inertial scaling
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
	    48.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    164,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Empty Space Container
	    System::LANG_French,            // region
	    "kan_kyokuyo",                  // internal name
	    "kan_kyokuyo.szs",              // archive filename
	    "kan_kyokuyo.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    48.0,                           // height
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
	    48.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    167,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Empty Space Container
	    System::LANG_Japanese,          // region
	    "kan_kyokuyo",                  // internal name
	    "kan_kyokuyo.szs",              // archive filename
	    "kan_kyokuyo.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    48.0,                           // height
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
	    48.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    164,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Silencer (CoC10) - different in JP (value only)
	{
	    // Silencer
	    System::LANG_English,           // region
	    "doll",                         // internal name
	    "doll.szs",                     // archive filename
	    "doll.bmd",                     // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    25.0,                           // pradius
	    55.0,                           // height
	    800.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    670,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    25.0,                           // depth A (for burying stuff)
	    26.0,                           // depth B (for burying stuff)
	    26.0,                           // depth C (for burying stuff)
	    21.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    131,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Silencer
	    System::LANG_French,            // region
	    "doll",                         // internal name
	    "doll.szs",                     // archive filename
	    "doll.bmd",                     // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    25.0,                           // pradius
	    55.0,                           // height
	    800.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    670,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    25.0,                           // depth A (for burying stuff)
	    26.0,                           // depth B (for burying stuff)
	    26.0,                           // depth C (for burying stuff)
	    21.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    130,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Silencer
	    System::LANG_Japanese,          // region
	    "doll",                         // internal name
	    "doll.szs",                     // archive filename
	    "doll.bmd",                     // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    25.0,                           // pradius
	    55.0,                           // height
	    800.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    20,                             // min carry
	    25,                             // max carry
	    "lod",                          // dynamics
	    666,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    25.0,                           // depth A (for burying stuff)
	    26.0,                           // depth B (for burying stuff)
	    26.0,                           // depth C (for burying stuff)
	    21.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    129,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Hole of Heroes - 2
	// Nutrient Silo (HoH6) - different in US
	{
	    // Nutrient Silo
	    System::LANG_English,           // region
	    "kan_meidiya",                  // internal name
	    "kan_meidiya.szs",              // archive filename
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
	    165,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Stringent Container
	    System::LANG_French,            // region
	    "kan_meidiya",                  // internal name
	    "kan_meidiya.szs",              // archive filename
	    "kan_meidiya.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    40.0,                           // radius
	    40.0,                           // pradius
	    45.0,                           // height
	    1000.0,                         // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    12,                             // min carry
	    20,                             // max carry
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
	    168,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Stringent Container
	    System::LANG_Japanese,          // region
	    "kan_meidiya",                  // internal name
	    "kan_meidiya.szs",              // archive filename
	    "kan_meidiya.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    40.0,                           // radius
	    40.0,                           // pradius
	    45.0,                           // height
	    1000.0,                         // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    12,                             // min carry
	    20,                             // max carry
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
	    168,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Dimensional Slicer (HoH9) - different in all three
	{
	    // Dimensional Slicer
	    System::LANG_English,           // region
	    "g_futa_hirosima",              // internal name
	    "g_futa_hirosima.szs",          // archive filename
	    "us_kankiri.bmd",               // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    20.0,                           // radius
	    15.0,                           // pradius
	    12.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    8,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    75,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Patience Tester
	    System::LANG_French,            // region
	    "g_futa_hirosima",              // internal name
	    "g_futa_hirosima.szs",          // archive filename
	    "us_kan_sunluck.bmd",           // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    44.0,                           // radius
	    44.0,                           // pradius
	    48.0,                           // height
	    500.0,                          // inertial scaling
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
	    48.0,                           // max depth (for burying stuff)
	    41.0,                           // depth A (for burying stuff)
	    41.0,                           // depth B (for burying stuff)
	    41.0,                           // depth C (for burying stuff)
	    41.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    161,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Flying Saucer
	    System::LANG_Japanese,          // region
	    "g_futa_hirosima",              // internal name
	    "g_futa_hirosima.szs",          // archive filename
	    "g_futa_hirosima.bmd",          // bmd filename
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
	    10.0,                           // max depth (for burying stuff)
	    20.0,                           // depth A (for burying stuff)
	    20.0,                           // depth B (for burying stuff)
	    20.0,                           // depth C (for burying stuff)
	    20.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    174,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Dream Den - 3
	// Yellow Taste Tyrant (DD11) - different in JP
	{
	    // Yellow Taste Tyrant
	    System::LANG_English,           // region
	    "g_futa_kyosin",                // internal name
	    "g_futa_kyosin.szs",            // archive filename
	    "us_futa_frenchs.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    10.0,                           // height
	    250.0,                          // inertial scaling
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
	{
	    // Yellow Taste Tyrant
	    System::LANG_French,            // region
	    "g_futa_kyosin",                // internal name
	    "g_futa_kyosin.szs",            // archive filename
	    "us_futa_frenchs.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    35.0,                           // radius
	    35.0,                           // pradius
	    10.0,                           // height
	    250.0,                          // inertial scaling
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
	    173,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Universe Art
	    System::LANG_Japanese,          // region
	    "g_futa_kyosin",                // internal name
	    "g_futa_kyosin.szs",            // archive filename
	    "g_futa_kyosin.bmd",            // bmd filename
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
	    175,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Stringent Container (DD12) - different in US
	{
	    // Stringent Container
	    System::LANG_English,           // region
	    "kan_maruha",                   // internal name
	    "kan_maruha.szs",               // archive filename
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
	    166,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Endless Repository
	    System::LANG_French,            // region
	    "kan_maruha",                   // internal name
	    "kan_maruha.szs",               // archive filename
	    "kan_maruha.bmd",               // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    48.0,                           // height
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
	    60.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    164,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Endless Repository
	    System::LANG_Japanese,          // region
	    "kan_maruha",                   // internal name
	    "kan_maruha.szs",               // archive filename
	    "kan_maruha.bmd",               // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    55.0,                           // radius
	    55.0,                           // pradius
	    48.0,                           // height
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
	    60.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    161,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Hypnotic Platter (DD13) - different in all three
	{
	    // Hypnotic Platter
	    System::LANG_English,           // region
	    "g_futa_sakotani",              // internal name
	    "g_futa_sakotani.szs",          // archive filename
	    "us_cap_rootbeer.bmd",          // bmd filename
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
	    177,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Alien Billboard
	    System::LANG_French,            // region
	    "g_futa_sakotani",              // internal name
	    "g_futa_sakotani.szs",          // archive filename
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
	    176,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Fake Emblem Thing
	    System::LANG_Japanese,          // region
	    "g_futa_sakotani",              // internal name
	    "g_futa_sakotani.szs",          // archive filename
	    "g_futa_sakotani.bmd",          // bmd filename
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
	    176,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	////////////////////////////////////////////////////////
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
				if (global_language_swap->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name, return PAL name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
						return "g_futa_sikoku";
					}
				} else {
					// If the current treasure name is the PAL internal name, return US/JP name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						return "g_futa_kyusyu";
					}
				}
			}
			// If GK4
			else if (section->getCurrFloor() + 1 == 4) {
				if (global_language_swap->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name, return PAL name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						return "g_futa_kyusyu";
					}
				}
				// Otherwise (US/JP version)
				else {
					// If the current treasure name is the PAL internal name, return US/JP name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
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
