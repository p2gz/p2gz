
#include <p2gz/Localization.h>
#include "Game/pelletMgr.h"

namespace gz {

const LocalizedTreasureConfig treasure_region_map[] = {
	//////////////////// VALLEY OF REPOSE ////////////////////
	//// Above Ground - 1
	// Courage Reactor - different in JP
	{
	    // Courage Reactor
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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

	//// Emergence Cave - 1
	// Spherical Atlas (EC2) - different in all three
	{
	    // Spherical Atlas
	    Treasure_US,                    // region
	    "map01",                        // internal name
	    "map01.szs",                    // archive filename
	    "us_eq_map01.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    53.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    50.0,                           // max depth (for burying stuff)
	    53.0,                           // depth A (for burying stuff)
	    53.0,                           // depth B (for burying stuff)
	    53.0,                           // depth C (for burying stuff)
	    53.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    184,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Spherical Atlas
	    Treasure_PAL,                   // region
	    "map01",                        // internal name
	    "map01.szs",                    // archive filename
	    "pal_eq_map01.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    53.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    50.0,                           // max depth (for burying stuff)
	    53.0,                           // depth A (for burying stuff)
	    53.0,                           // depth B (for burying stuff)
	    53.0,                           // depth C (for burying stuff)
	    53.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    184,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Spherical Atlas
	    Treasure_JP,                    // region
	    "map01",                        // internal name
	    "map01.szs",                    // archive filename
	    "eq_map01.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    53.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    50.0,                           // max depth (for burying stuff)
	    53.0,                           // depth A (for burying stuff)
	    53.0,                           // depth B (for burying stuff)
	    53.0,                           // depth C (for burying stuff)
	    53.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    184,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Subterranean Complex - 1
	// Thirst Activator (SCx7) - different for all three
	{
	    // Thirst Activator
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	//// Above Ground - 3
	// Healing Cask - different in all three
	{
	    // Healing Cask
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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

	// Decorative Goo - different in JP
	{
	    // Decorative Goo
	    Treasure_US,                    // region
	    "kan",                          // internal name
	    "kan.szs",                      // archive filename
	    "us_enogu.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    24.0,                           // radius
	    24.0,                           // pradius
	    35.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    64,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Decorative Goo
	    Treasure_PAL,                   // region
	    "kan",                          // internal name
	    "kan.szs",                      // archive filename
	    "us_enogu.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    24.0,                           // radius
	    24.0,                           // pradius
	    35.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    64,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Decorative Goo
	    Treasure_JP,                    // region
	    "kan",                          // internal name
	    "kan.szs",                      // archive filename
	    "kan.bmd",                      // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    24.0,                           // radius
	    24.0,                           // pradius
	    35.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    20,                             // max carry
	    "lod",                          // dynamics
	    80,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    55.0,                           // max depth (for burying stuff)
	    55.0,                           // depth A (for burying stuff)
	    55.0,                           // depth B (for burying stuff)
	    55.0,                           // depth C (for burying stuff)
	    55.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    63,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Geographic Projection - different in all three
	{
	    // Geographic Projection
	    Treasure_US,                    // region
	    "map02",                        // internal name
	    "map02.szs",                    // archive filename
	    "us_eq_map02.bmd",              // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    52.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    3.0,                            // depth (for burying stuff)
	    60.0,                           // max depth (for burying stuff)
	    52.0,                           // depth A (for burying stuff)
	    52.0,                           // depth B (for burying stuff)
	    52.0,                           // depth C (for burying stuff)
	    52.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    185,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Geographic Projection
	    Treasure_PAL,                   // region
	    "map02",                        // internal name
	    "map02.szs",                    // archive filename
	    "pal_eq_map02.bmd",             // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    52.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    3.0,                            // depth (for burying stuff)
	    60.0,                           // max depth (for burying stuff)
	    52.0,                           // depth A (for burying stuff)
	    52.0,                           // depth B (for burying stuff)
	    52.0,                           // depth C (for burying stuff)
	    52.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    185,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Geographic Projection
	    Treasure_JP,                    // region
	    "map02",                        // internal name
	    "map02.szs",                    // archive filename
	    "eq_map02.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    52.0,                           // pradius
	    50.0,                           // height
	    750.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    101,                            // min carry
	    101,                            // max carry
	    "lod",                          // dynamics
	    200,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    3.0,                            // depth (for burying stuff)
	    60.0,                           // max depth (for burying stuff)
	    52.0,                           // depth A (for burying stuff)
	    52.0,                           // depth B (for burying stuff)
	    52.0,                           // depth C (for burying stuff)
	    52.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    185,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Hole of Beasts - None

	//// White Flower Garden - 2
	// Alien Billboard - different in PAL
	{
	    // Alien Billboard
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
	    "g_futa_kajiwara",              // internal name
	    "g_futa_kajiwara.szs",          // archive filename
	    "pal_kan_hawesta.bmd",          // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    40.0,                           // radius
	    40.0,                           // pradius
	    35.0,                           // height
	    800.0,                          // inertial scaling
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
	    160,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Paper Slider
	    Treasure_JP,                    // region
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
	    Treasure_US,                     // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                     // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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

	// Time Capsule (CoS4) - different in JP
	{
	    // Time Capsule
	    Treasure_US,                    // region
	    "locket",                       // internal name
	    "locket.szs",                   // archive filename
	    "us_locket.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    20.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    7,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    70,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    22.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    76,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Time Capsule
	    Treasure_PAL,                   // region
	    "locket",                       // internal name
	    "locket.szs",                   // archive filename
	    "us_locket.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    20.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    7,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    70,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    22.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    75,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Time Capsule
	    Treasure_JP,                    // region
	    "locket",                       // internal name
	    "locket.szs",                   // archive filename
	    "locket.bmd",                   // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    25.0,                           // radius
	    22.0,                           // pradius
	    20.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    4,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    7,                              // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    70,                             // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    22.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    75,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Glutton's Kitchen - 4
	// Master's Instrument (GK1) - different in JP
	{
	    // Master's Instrument
	    Treasure_US,                    // region
	    "bane_red",                     // internal name
	    "bane_red.szs",                 // archive filename
	    "us_crayon.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    15.0,                           // radius
	    10.0,                           // pradius
	    13.0,                           // height
	    220.0,                          // inertial scaling
	    "simple",                       // particle type
	    6,                              // number of particle
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
	    73.0,                           // max depth (for burying stuff)
	    33.0,                           // depth A (for burying stuff)
	    33.0,                           // depth B (for burying stuff)
	    33.0,                           // depth C (for burying stuff)
	    33.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    65,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Master's Instrument
	    Treasure_PAL,                   // region
	    "bane_red",                     // internal name
	    "bane_red.szs",                 // archive filename
	    "us_crayon.bmd",                // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    15.0,                           // radius
	    10.0,                           // pradius
	    13.0,                           // height
	    220.0,                          // inertial scaling
	    "simple",                       // particle type
	    6,                              // number of particle
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
	    73.0,                           // max depth (for burying stuff)
	    33.0,                           // depth A (for burying stuff)
	    33.0,                           // depth B (for burying stuff)
	    33.0,                           // depth C (for burying stuff)
	    33.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    65,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Master's Instrument
	    Treasure_JP,                    // region
	    "bane_red",                     // internal name
	    "bane_red.szs",                 // archive filename
	    "bane_red.bmd",                 // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    15.0,                           // radius
	    10.0,                           // pradius
	    13.0,                           // height
	    220.0,                          // inertial scaling
	    "simple",                       // particle type
	    6,                              // number of particle
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
	    73.0,                           // max depth (for burying stuff)
	    33.0,                           // depth A (for burying stuff)
	    33.0,                           // depth B (for burying stuff)
	    33.0,                           // depth C (for burying stuff)
	    33.0,                           // depth D (for burying stuff)
	    1,                              // code (special properties)
	    64,                             // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	// Massive Lid (GK2) - different in all three
	{
	    // Massive Lid
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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

	// Boom Cone (GK5) - different in US
	{
	    // Boom Cone
	    Treasure_US,                    // region
	    "compact",                      // internal name
	    "compact.szs",                  // archive filename
	    "us_cracker.bmd",               // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    27.0,                           // radius
	    20.0,                           // pradius
	    22.0,                           // height
	    250.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    10,                             // min carry
	    15,                             // max carry
	    "lod",                          // dynamics
	    100,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    134,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Magical Stage
	    Treasure_PAL,                   // region
	    "compact",                      // internal name
	    "compact.szs",                  // archive filename
	    "compact.bmd",                  // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    52.0,                           // pradius
	    25.0,                           // height
	    800.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    30,                             // min carry
	    45,                             // max carry
	    "lod",                          // dynamics
	    150,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    133,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},
	{
	    // Magical Stage
	    Treasure_JP,                    // region
	    "compact",                      // internal name
	    "compact.szs",                  // archive filename
	    "compact.bmd",                  // bmd filename
	    "animmgr.txt",                  // anmmgr filename
	    "collInfo.txt",                 // collinfo filename
	    52.0,                           // radius
	    52.0,                           // pradius
	    25.0,                           // height
	    800.0,                          // inertial scaling
	    "simple",                       // particle type
	    8,                              // number of particle
	    1.0,                            // particle size
	    0.1,                            // friction
	    30,                             // min carry
	    45,                             // max carry
	    "lod",                          // dynamics
	    150,                            // poko value
	    "yes",                          // unqiue
	    "no",                           // indirect
	    0,                              // num of pmotions
	    0.0,                            // depth (for burying stuff)
	    10.0,                           // max depth (for burying stuff)
	    10.0,                           // depth A (for burying stuff)
	    50.0,                           // depth B (for burying stuff)
	    50.0,                           // depth C (for burying stuff)
	    50.0,                           // depth D (for burying stuff)
	    0,                              // code (special properties)
	    132,                            // index in piklopedia
	    Game::PelletConfig::Indirect_No // indirect state
	},

	//// Shower Room - 7
	// Merciless Extractor (SR1) - different in US
	{
	    // Merciless Extractor
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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
	    Treasure_US,                    // region
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
	    Treasure_PAL,                   // region
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
	    Treasure_JP,                    // region
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

const u32 TREASURE_MAP_COUNT = ARRAY_SIZE(treasure_region_map);

} // namespace gz
