#include <p2gz/Preset.h>
#include <p2gz/PresetsPoD.h>

using namespace gz;
using namespace Game;

// Treasures
static Preset::TreasureGenSpawnOverride t_pod_all[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_Spawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_Spawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_Spawn),  /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_Spawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn), /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
};

static Preset::TreasureGenSpawnOverride t_pod_post_eb[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_Spawn),      /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_Spawn),     /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_Spawn),      /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn),     /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),      /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),      /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn),     /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),      /* ursidae */
};

static Preset::TreasureGenSpawnOverride t_pod_post_enter_bk[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn),     /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),      /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),      /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn),     /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),      /* ursidae */
};

static Preset::TreasureGenSpawnOverride t_pod_post_day_6[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),      /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),      /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn),     /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),      /* ursidae */
};

static Preset::TreasureGenSpawnOverride t_pod_post_enter_scx[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_Ignore),     /* ursidae */
};

static Preset::TreasureGenSpawnOverride t_pod_post_enter_fc[] = {
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
};

// Enemies
static Preset::EnemyGenSpawnOverride e_pod_enter_hob[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(13.00f, 15.0f, 211.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(209.00f, 10.0f, -206.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(14.00f, 15.0f, -263.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-258.00f, 15.0f, -133.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(-36.44f, 15.0f, -104.84f), PSO_Spawn),
};

static Preset::EnemyGenSpawnOverride e_pod_post_hob[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_DontSpawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_DontSpawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(13.00f, 15.0f, 211.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(209.00f, 10.0f, -206.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(14.00f, 15.0f, -263.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-258.00f, 15.0f, -133.00f), PSO_Spawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(-36.44f, 15.0f, -104.84f), PSO_Spawn),
};

static Preset::EnemyGenSpawnOverride e_pod_enter_scx[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-1153.0f, 47.0f, 2231.0f), PSO_DontSpawn),
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-548.0f, 100.0f, 546.0f), PSO_Spawn),
};

// Gates
static const char* g_pod_ec[]             = { "EC gate" };
static const char* g_pod_post_enter_hob[] = { "EC gate", "black gate" };
static const char* g_pod_post_enter_sh[]  = { "EC gate", "black gate", "SH gate" };
static const char* g_pod_post_enter_scx[] = { "EC gate", "black gate", "SH gate", "water gate" };

// Bags
static const char* b_post_ec[] = { "landing area bag (15)", "hubcap bag (35)" };
static const char* b_all[]     = { "landing area bag (15)", "hubcap bag (35)", "WFG bag (200)" };

// Bridges
static const char* br_pod_post_enter_bk[]  = { "globe bridge" };
static const char* br_pod_post_enter_scx[] = { "globe bridge", "water bridge" };
static const char* br_pod_post_enter_fc[]  = { "globe bridge", "water bridge", "FC bridge" };

// Upgrades
static OlimarData::ItemIndex ek_pod_post_vor1[] = { OlimarData::ODII_SphericalAtlas };
static OlimarData::ItemIndex ek_pod_post_hob[]  = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector };
static OlimarData::ItemIndex ek_pod_post_wfg[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector, OlimarData::ODII_FiveManNapsack };
static OlimarData::ItemIndex ek_pod_post_sh[] = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector,
	                                              OlimarData::ODII_FiveManNapsack, OlimarData::ODII_JusticeAlloy };
static OlimarData::ItemIndex ek_pod_post_enter_bk[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector, OlimarData::ODII_FiveManNapsack, OlimarData::ODII_JusticeAlloy,
	    OlimarData::ODII_GeographicProjection };
static OlimarData::ItemIndex ek_pod_post_bk[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector,    OlimarData::ODII_FiveManNapsack,
	    OlimarData::ODII_JusticeAlloy,   OlimarData::ODII_GeographicProjection, OlimarData::ODII_ForgedCourage };
static OlimarData::ItemIndex ek_pod_post_fc5[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_PrototypeDetector,    OlimarData::ODII_FiveManNapsack,
	    OlimarData::ODII_JusticeAlloy,   OlimarData::ODII_GeographicProjection, OlimarData::ODII_ForgedCourage,
	    OlimarData::ODII_BruteKnuckles };

void PresetMgr::init_pod_presets()
{
	init_pod_vor1_presets();
	init_pod_aw_presets();
	init_pod_vor2_presets();
	init_pod_pp_presets();
}

void PresetMgr::init_pod_vor1_presets()
{
	// presets.push((new Preset("Day 1", PoD))->set_day(1));
	// presets.push((new Preset("Day 2", PoD))
	//                  ->set_onion_pikmin(Leaf, Red, 0)
	//                  ->set_day(2)
	//                  ->set_cutscene_flags(NUM_DAY_2_DEMO_FLAGS, (DemoFlags[NUM_DAY_2_DEMO_FLAGS]) { DAY_2_DEMO_FLAGS }));
	// presets.push((new Preset("Day 3", PoD))
	//                  ->set_onion_pikmin(Leaf, Red, 9)
	//                  ->set_day(3)
	//                  ->set_cutscene_flags(NUM_DAY_3_DEMO_FLAGS, (DemoFlags[NUM_DAY_3_DEMO_FLAGS]) { DAY_3_DEMO_FLAGS }));
	// presets.push((new Preset("Day 4", PoD))
	//                  ->set_onion_pikmin(Leaf, Red, 13)
	//                  ->set_day(4)
	//                  ->set_cutscene_flags(NUM_DAY_4_DEMO_FLAGS, (DemoFlags[NUM_DAY_4_DEMO_FLAGS]) { DAY_4_DEMO_FLAGS }));
	presets.push((new Preset("EC1", PoD))
	                 ->set_pikmin(Flower, Red, 46)
	                 ->set_pikmin(Leaf, Red, 6)
	                 ->set_time(8.0f)
	                 ->set_day(4)
	                 ->set_cutscene_flags(NUM_EC1_DEMO_FLAGS, (DemoFlags[NUM_EC1_DEMO_FLAGS]) { EC1_DEMO_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_AW_CAVE_FLAGS, (CaveIndex[NUM_AW_CAVE_FLAGS]) { AW_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_ec), g_pod_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec));
	presets.push((new Preset("EC2", PoD))
	                 ->set_pikmin(Flower, Red, 46)
	                 ->set_pikmin(Leaf, Red, 6)
	                 ->set_time(8.0f)
	                 ->set_day(4)
	                 ->set_cutscene_flags(NUM_EC2_DEMO_FLAGS, (DemoFlags[NUM_EC2_DEMO_FLAGS]) { EC2_DEMO_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_AW_CAVE_FLAGS, (CaveIndex[NUM_AW_CAVE_FLAGS]) { AW_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_ec), g_pod_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_hob), e_pod_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromMap));
}

void PresetMgr::init_pod_aw_presets()
{
	presets.push((new Preset("enter HoB", PoD))
	                 ->set_onion_pikmin(Flower, Red, 36)
	                 ->set_onion_pikmin(Leaf, Red, 6)
	                 ->set_onion_pikmin(Leaf, Purple, 10)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_vor1), ek_pod_post_vor1)
	                 ->set_cutscene_flags(NUM_AW_DEMO_FLAGS, (DemoFlags[NUM_AW_DEMO_FLAGS]) { AW_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_AW_EK_FLAGS, (OlimarData::ItemIndex[NUM_AW_EK_FLAGS]) { AW_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_AW_CAVE_FLAGS, (CaveIndex[NUM_AW_CAVE_FLAGS]) { AW_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_ec), g_pod_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_hob), e_pod_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FirstEnter));
	presets.push((new Preset("HoB1-2", PoD))
	                 ->set_pikmin(Flower, Red, 62)
	                 ->set_pikmin(Flower, Purple, 10)
	                 ->set_time(8.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_vor1), ek_pod_post_vor1)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_AW_EK_FLAGS, (OlimarData::ItemIndex[NUM_AW_EK_FLAGS]) { AW_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoB3-4", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 10)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_time(8.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_vor1), ek_pod_post_vor1)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_AW_EK_FLAGS, (OlimarData::ItemIndex[NUM_AW_EK_FLAGS]) { AW_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoB5", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(8.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_vor1), ek_pod_post_vor1)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_AW_EK_FLAGS, (OlimarData::ItemIndex[NUM_AW_EK_FLAGS]) { AW_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_ec), b_post_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("WFG1-WFG3", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(8.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_hob), ek_pod_post_hob)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_WFG_EK_FLAGS]) { WFG_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("WFG4-WFG5", PoD))
	                 ->set_pikmin(Flower, Red, 37)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(8.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_hob), ek_pod_post_hob)
	                 ->set_cutscene_flags(NUM_WFG_DEMO_FLAGS, (DemoFlags[NUM_WFG_DEMO_FLAGS]) { WFG_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_WFG_EK_FLAGS]) { WFG_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter SH", PoD))
	                 ->set_pikmin(Flower, Red, 37)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(8.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_wfg), ek_pod_post_wfg)
	                 ->set_cutscene_flags(NUM_WFG_DEMO_FLAGS, (DemoFlags[NUM_WFG_DEMO_FLAGS]) { WFG_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_SH_EK_FLAGS]) { SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_hob), g_pod_post_enter_hob)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_all), t_pod_all)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("SH1-2", PoD))
	                 ->set_pikmin(Flower, Red, 35)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(10.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_wfg), ek_pod_post_wfg)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_SH_EK_FLAGS]) { SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SH_CAVE_FLAGS, (CaveIndex[NUM_SH_CAVE_FLAGS]) { SH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_eb), t_pod_post_eb)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("SH3-7", PoD))
	                 ->set_pikmin(Flower, Red, 30)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(10.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_wfg), ek_pod_post_wfg)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_SH_EK_FLAGS]) { SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SH_CAVE_FLAGS, (CaveIndex[NUM_SH_CAVE_FLAGS]) { SH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_eb), t_pod_post_eb)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter BK (15/5w)", PoD))
	                 ->set_pikmin(Flower, Red, 27)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(10.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_sh), ek_pod_post_sh)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_ENTER_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_ENTER_BK_EK_FLAGS]) { ENTER_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SH_CAVE_FLAGS, (CaveIndex[NUM_SH_CAVE_FLAGS]) { SH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_eb), t_pod_post_eb)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter BK (20w)", PoD))
	                 ->set_pikmin(Flower, Red, 27)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(10.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_sh), ek_pod_post_sh)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_ENTER_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_ENTER_BK_EK_FLAGS]) { ENTER_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SH_CAVE_FLAGS, (CaveIndex[NUM_SH_CAVE_FLAGS]) { SH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_eb), t_pod_post_eb)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter BK (25w)", PoD))
	                 ->set_pikmin(Flower, Red, 22)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(10.5f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_sh), ek_pod_post_sh)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_ENTER_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_ENTER_BK_EK_FLAGS]) { ENTER_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SH_CAVE_FLAGS, (CaveIndex[NUM_SH_CAVE_FLAGS]) { SH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_post_hob), e_pod_post_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_eb), t_pod_post_eb)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("BK", PoD))
	                 ->set_pikmin(Flower, Red, 27)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Leaf, Blue, 35)
	                 ->set_onion_pikmin(Leaf, Blue, 25)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(13.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_enter_bk), ek_pod_post_enter_bk)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_BK_EK_FLAGS]) { BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_bk), t_pod_post_enter_bk)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("BK (25w)", PoD))
	                 ->set_pikmin(Flower, Red, 22)
	                 ->set_pikmin(Flower, Purple, 18)
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_pikmin(Leaf, Blue, 35)
	                 ->set_onion_pikmin(Leaf, Blue, 25)
	                 ->set_onion_pikmin(Flower, Purple, 2)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_time(13.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_enter_bk), ek_pod_post_enter_bk)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_BK_EK_FLAGS]) { BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_bk), t_pod_post_enter_bk)
	                 ->set_enter_kind(PEK_FromCave));
}

void PresetMgr::init_pod_vor2_presets()
{
	presets.push((new Preset("day 6 CR", PoD))
	                 ->set_onion_pikmin(Leaf, Blue, 60)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_day(6)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_bk), t_pod_post_enter_bk)
	                 ->set_enter_kind(PEK_FromMap));
	presets.push((new Preset("day 6 CR (25w)", PoD))
	                 ->set_onion_pikmin(Leaf, Blue, 60)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 25)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_day(6)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_bk), t_pod_post_enter_bk)
	                 ->set_enter_kind(PEK_FromMap));
	presets.push((new Preset("enter SCx", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_scx), e_pod_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_day_6), t_pod_post_day_6)
	                 ->set_enter_kind(PEK_FromMap));
	presets.push((new Preset("enter SCx (25w)", PoD))
	                 ->set_onion_pikmin(Flower, White, 25)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, Blue, 55)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_sh), g_pod_post_enter_sh)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_bk), br_pod_post_enter_bk)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_scx), e_pod_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_day_6), t_pod_post_day_6)
	                 ->set_enter_kind(PEK_FromMap));
	presets.push((new Preset("SCx1-4", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_scx), br_pod_post_enter_scx)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_scx), e_pod_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_scx), t_pod_post_enter_scx)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("SCx1-4 (25w)", PoD))
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 55)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_scx), br_pod_post_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_scx), t_pod_post_enter_scx)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("SCx5-8", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 45)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_scx), br_pod_post_enter_scx)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_scx), e_pod_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_scx), t_pod_post_enter_scx)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("SCx5-8 (40w)", PoD))
	                 ->set_pikmin(Flower, White, 40)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 40)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_scx), br_pod_post_enter_scx)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_pod_enter_scx), e_pod_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_scx), t_pod_post_enter_scx)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC1-5", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_plug_destroyed(true)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_scx), t_pod_post_enter_scx)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC1-5 (40w)", PoD))
	                 ->set_pikmin(Flower, White, 40)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 15)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_time(9.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_bk), ek_pod_post_bk)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_plug_destroyed(true)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC6-7", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_time(10.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_plug_destroyed(true)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC6-7 (40w)", PoD))
	                 ->set_pikmin(Flower, White, 40)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 15)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_time(10.5f)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_plug_destroyed(true)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FromCave));
}

void PresetMgr::init_pod_pp_presets()
{
	presets.push((new Preset("CoS", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 40)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_onion_pikmin(Flower, Blue, 5)
	                 ->set_day(8)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_COS_DEMO_FLAGS, (DemoFlags[NUM_COS_DEMO_FLAGS]) { COS_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_COS_CAVE_FLAGS, (CaveIndex[NUM_COS_CAVE_FLAGS]) { COS_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FirstEnter));
	presets.push((new Preset("CoS (40w)", PoD))
	                 ->set_pikmin(Flower, White, 40)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 35)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Flower, Blue, 5)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_day(8)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_COS_DEMO_FLAGS, (DemoFlags[NUM_COS_DEMO_FLAGS]) { COS_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_COS_CAVE_FLAGS, (CaveIndex[NUM_COS_CAVE_FLAGS]) { COS_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FirstEnter));
	presets.push((new Preset("GK", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 34)
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_onion_pikmin(Flower, Blue, 11)
	                 ->set_time(8.5f)
	                 ->set_day(8)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_GK_DEMO_FLAGS, (DemoFlags[NUM_GK_DEMO_FLAGS]) { GK_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_GK_EK_FLAGS]) { GK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_GK_CAVE_FLAGS, (CaveIndex[NUM_GK_CAVE_FLAGS]) { GK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("GK (40w)", PoD))
	                 ->set_pikmin(Flower, White, 40)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 29)
	                 ->set_onion_pikmin(Flower, Red, 23)
	                 ->set_onion_pikmin(Flower, Blue, 11)
	                 ->set_onion_pikmin(Leaf, Blue, 5)
	                 ->set_time(8.5f)
	                 ->set_day(8)
	                 ->set_upgrades(ARRAY_SIZE(ek_pod_post_fc5), ek_pod_post_fc5)
	                 ->set_cutscene_flags(NUM_GK_DEMO_FLAGS, (DemoFlags[NUM_GK_DEMO_FLAGS]) { GK_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_GK_EK_FLAGS]) { GK_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_GK_CAVE_FLAGS, (CaveIndex[NUM_GK_CAVE_FLAGS]) { GK_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_pod_post_enter_scx), g_pod_post_enter_scx)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_finished_bridges(ARRAY_SIZE(br_pod_post_enter_fc), br_pod_post_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_pod_post_enter_fc), t_pod_post_enter_fc)
	                 ->set_enter_kind(PEK_FromCave));
}
