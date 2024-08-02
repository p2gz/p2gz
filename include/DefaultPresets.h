#include "GlobalData.h"
#include "Game/Piki.h"
#include "Game/gamePlayData.h"

using namespace Game;

gzCollections::Vec<Preset> getDefaultPresets() {
    using namespace gzCollections;

    IndexBitflag<u64> ec_cflags = IndexBitflag<u64>()
        .set(DEMO_Pluck_First_Pikmin)
        .set(DEMO_Discover_Treasure)
        .set(DEMO_First_Gate_Down)
        .set(DEMO_First_Nectar_Use)
        .set(DEMO_Day_One_Start)
        .set(DEMO_Meet_Red_Pikmin)
        .set(DEMO_Louie_Finds_Red_Onion)
        .set(DEMO_Unlock_Captain_Switch)
        .set(DEMO_First_Use_Louie)
        .set(DEMO_Reunite_Captains)
        .set(DEMO_First_Number_Pellet);

    IndexBitflag<u64> hob_cflags = IndexBitflag<u64>(ec_cflags)
        .set(DEMO_Find_Cave_Deeper_Hole)
        .set(DEMO_Find_Cave_Geyser)
        .set(DEMO_First_Cave_Enter)
        .set(DEMO_First_Cave_Return)
        .set(DEMO_First_Globe_Day_End)
        .set(DEMO_Purple_Candypop)
        .set(DEMO_Enter_Awakening_Wood)
        .set(DEMO_First_Corpse_In_Cave)
        .set(DEMO_Purples_In_Ship);
    IndexBitflag<u32> hob_upgrades = IndexBitflag<u32>().set(OlimarData::ODII_SphericalAtlas);

    IndexBitflag<u64> wfg_cflags = IndexBitflag<u64>(hob_cflags)
        .set(DEMO_Whites_Digging)
        .set(DEMO_White_Candypop);
    IndexBitflag<u32> wfg_upgrades = IndexBitflag<u32>(hob_upgrades).set(OlimarData::ODII_PrototypeDetector);

    IndexBitflag<u64> sh_cflags = IndexBitflag<u64>(wfg_cflags)
        .set(DEMO_Pikmin_In_Danger_Poison)
        .set(DEMO_Find_Blue_Onion);
    IndexBitflag<u32> sh_upgrades = IndexBitflag<u32>(wfg_upgrades).set(OlimarData::ODII_FiveManNapsack);

    IndexBitflag<u64> bk_cflags = IndexBitflag<u64>(sh_cflags).set(DEMO_Max_Pikmin_On_Field);
    IndexBitflag<u32> bk_upgrades = IndexBitflag<u32>(sh_upgrades).set(OlimarData::ODII_JusticeAlloy);

    IndexBitflag<u64> vor2_cflags = IndexBitflag<u64>(bk_cflags).set(DEMO_Whites_In_Ship);
    IndexBitflag<u32> vor2_upgrades = IndexBitflag<u32>(bk_upgrades).set(OlimarData::ODII_ForgedCourage);

    IndexBitflag<u64> pp_cflags = IndexBitflag<u64>(vor2_cflags).set(DEMO_Enter_Perplexing_Pool);
    IndexBitflag<u32> pp_upgrades = IndexBitflag<u32>(vor2_upgrades).set(OlimarData::ODII_BruteKnuckles);

    Preset defaultPresets[] = {
        // PoD EC
        Preset().setMsgId('3200_00')
            .setPikmin(Flower, Red,    46)
            .setPikmin(Leaf,   Red,     6)
            .addCutsceneFlags(ec_cflags),
        // PoD Enter HoB
        Preset().setMsgId('3201_00')
            .setOnionPikmin(Flower, Red,    46)
            .setOnionPikmin(Leaf,   Red,     6)
            .setOnionPikmin(Leaf,   Purple, 10)
            .addCutsceneFlags(hob_cflags)
            .addUpgrades(hob_upgrades),
        // PoD HoB1
        Preset().setMsgId('3202_00')
            .setPikmin(Flower, Red,    62)
            .setPikmin(Flower, Purple, 10)
            .addCutsceneFlags(hob_cflags)
            .addUpgrades(hob_upgrades),
        // PoD HoB2-4
        Preset().setMsgId('3203_00')
            .setPikmin(Flower, Red,    52)
            .setPikmin(Flower, Purple, 10)
            .setPikmin(Leaf,   Purple, 10)
            .addCutsceneFlags(hob_cflags)
            .addUpgrades(hob_upgrades),
        // PoD HoB5
        Preset().setMsgId('3204_00')
            .setPikmin(Flower, Red,    52)
            .setPikmin(Flower, Purple, 20)
            .addCutsceneFlags(hob_cflags)
            .addUpgrades(hob_upgrades),
        // PoD Enter WFG-WFG3
        Preset().setMsgId('3226_00')
            .setPikmin(Flower, Red,    52)
            .setPikmin(Flower, Purple, 20)
            .addCutsceneFlags(hob_cflags)
            .addUpgrades(wfg_upgrades),
        // PoD WFG4-WFG5
        Preset().setMsgId('3205_00')
            .setPikmin(Flower, Red,    37)
            .setPikmin(Flower, Purple, 20)
            .setPikmin(Flower, White,  15)
            .addCutsceneFlags(wfg_cflags)
            .addUpgrades(wfg_upgrades),
        // PoD Enter SH
        Preset().setMsgId('3227_00')
            .setPikmin(Flower, Red,    37)
            .setPikmin(Flower, Purple, 20)
            .setPikmin(Flower, White,  15)
            .addCutsceneFlags(wfg_cflags)
            .addUpgrades(sh_upgrades),
        // PoD SH1-2
        Preset().setMsgId('3206_00')
            .setPikmin(     Flower, Red,    34)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setOnionPikmin(Leaf,   Blue,   16)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(sh_upgrades),
        // PoD SH3-7
        Preset().setMsgId('3207_00')
            .setPikmin(     Flower, Red,    29)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,   5)
            .setOnionPikmin(Leaf,   Blue,   16)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(sh_upgrades),
        // PoD SH7 (+5W)
        Preset().setMsgId('3208_00')
            .setPikmin(     Flower, Red,    24)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  10)
            .setOnionPikmin(Leaf,   Blue,   16)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(sh_upgrades),
        // PoD Enter BK
        Preset().setMsgId('3209_00')
            .setPikmin(     Flower, Red,    26)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,   5)
            .setOnionPikmin(Leaf,   Blue,   16)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(bk_upgrades),
        // PoD Enter BK (+2B)
        Preset().setMsgId('3210_00')
            .setPikmin(     Flower, Red,    26)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,   5)
            .setOnionPikmin(Leaf,   Blue,   18)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(bk_upgrades),
        // PoD Enter BK (+5W)
        Preset().setMsgId('3211_00')
            .setPikmin(     Flower, Red,    21)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  10)
            .setOnionPikmin(Leaf,   Blue,   16)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(bk_upgrades),
        // PoD Enter BK (+5W +2B)
        Preset().setMsgId('3212_00')
            .setPikmin(     Flower, Red,    21)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  10)
            .setOnionPikmin(Leaf,   Blue,   18)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(sh_cflags)
            .addUpgrades(bk_upgrades),
        // PoD BK1-BK7
        Preset().setMsgId('3213_00')
            .setPikmin(     Flower, Red,    26)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,   5)
            .setPikmin(     Leaf,   Blue,   36)
            .setOnionPikmin(Leaf,   Blue,   24)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(bk_cflags)
            .addUpgrades(bk_upgrades),
        // PoD BK1-BK7 (+5W)
        Preset().setMsgId('3214_00')
            .setPikmin(     Flower, Red,    21)
            .setPikmin(     Flower, Purple, 18)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  10)
            .setPikmin(     Leaf,   Blue,   36)
            .setOnionPikmin(Leaf,   Blue,   24)
            .setOnionPikmin(Flower, Red,     3)
            .setOnionPikmin(Flower, Purple,  2)
            .addCutsceneFlags(bk_cflags)
            .addUpgrades(bk_upgrades),
        // PoD Enter SCx (Day 6)
        Preset().setMsgId('3215_00')
            .setOnionPikmin(Leaf,   Blue,   65)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Purple, 20)
            .setOnionPikmin(Flower, White,  15)
            .setOnionPikmin(Leaf,   White,  10)
            .addCutsceneFlags(bk_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD Enter SCx (Day 7)
        Preset().setMsgId('3216_00')
            .setOnionPikmin(Flower, Blue,   65)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Purple, 20)
            .setOnionPikmin(Flower, White,  15)
            .setOnionPikmin(Leaf,   White,  10)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD SCx1-4
        Preset().setMsgId('3217_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  5)
            .setPikmin(     Flower, Blue,   60)
            .setOnionPikmin(Flower, Red,    29)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD SCx1-4 (+5W)
        Preset().setMsgId('3218_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  15)
            .setPikmin(     Leaf,   White,  10)
            .setPikmin(     Flower, Blue,   55)
            .setOnionPikmin(Flower, Red,    24)
            .setOnionPikmin(Leaf,   Blue,    5)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD SCx5-Enter FC
        Preset().setMsgId('3219_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  35)
            .setPikmin(     Flower, Blue,   45)
            .setOnionPikmin(Flower, Red,    29)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD SCx5-Enter FC (40W)
        Preset().setMsgId('3220_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  40)
            .setPikmin(     Flower, Blue,   40)
            .setOnionPikmin(Flower, Red,    24)
            .setOnionPikmin(Leaf,   Blue,    5)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD FC1-FC5
        Preset().setMsgId('3221_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  35)
            .setPikmin(     Flower, Blue,   20)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Blue,   25)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD FC1-FC5 (40W)
        Preset().setMsgId('3222_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  40)
            .setPikmin(     Flower, Blue,   15)
            .setOnionPikmin(Flower, Red,    24)
            .setOnionPikmin(Flower, Blue,   25)
            .setOnionPikmin(Leaf,   Blue,    5)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(vor2_upgrades),
        // PoD FC6-FC7
        Preset().setMsgId('3228_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  35)
            .setPikmin(     Flower, Blue,   20)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Blue,   25)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(pp_upgrades),
        // PoD FC6-FC7 (40W)
        Preset().setMsgId('3229_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  40)
            .setPikmin(     Flower, Blue,   15)
            .setOnionPikmin(Flower, Red,    24)
            .setOnionPikmin(Flower, Blue,   25)
            .setOnionPikmin(Leaf,   Blue,    5)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(pp_upgrades),
        // PoD Enter CoS
        Preset().setMsgId('3223_00')
            .setOnionPikmin(Flower, Blue,   50)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Purple, 20)
            .setOnionPikmin(Flower, White,  40)
            .addCutsceneFlags(vor2_cflags)
            .addUpgrades(pp_upgrades),
        // PoD CoS1-GK6
        Preset().setMsgId('3224_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  35)
            .setPikmin(     Flower, Blue,   45)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Blue,    5)
            .addCutsceneFlags(pp_cflags)
            .addUpgrades(pp_upgrades),
        // PoD CoS1-GK6 (+5W)
        Preset().setMsgId('3225_00')
            .setPikmin(     Flower, Purple, 20)
            .setPikmin(     Flower, White,  40)
            .setPikmin(     Flower, Blue,   40)
            .setOnionPikmin(Flower, Red,    29)
            .setOnionPikmin(Flower, Blue,   10)
            .addCutsceneFlags(pp_cflags)
            .addUpgrades(pp_upgrades),
        // Testing
        Preset().setMsgId('3299_00')
            .setPikmin(     Flower, Red,     20)
            .setPikmin(     Flower, Yellow,  20)
            .setPikmin(     Flower, Blue,    20)
            .setPikmin(     Flower, Purple,  20)
            .setPikmin(     Flower, White,   20)
            .setOnionPikmin(Flower, Red,    999)
            .setOnionPikmin(Flower, Yellow, 999)
            .setOnionPikmin(Flower, Blue,   999)
            .setOnionPikmin(Flower, Purple, 999)
            .setOnionPikmin(Flower, White,  999)
            .setSprays(99, 99)
            .addUpgrades(pp_upgrades),
    };

    size_t numPresets = sizeof(defaultPresets) / sizeof(Preset);
    gzCollections::Vec<Preset> presets(numPresets);
    for (size_t i = 0; i < numPresets; i++) {
        presets.push(defaultPresets[i]);
    }
    return presets;
}
