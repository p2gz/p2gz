#include <p2gz/p2gz.h>
#include <p2gz/Preset.h>
#include <p2gz/warp.h>
#include <Game/Piki.h>
#include <System.h>
#include <Game/PikiMgr.h>

using namespace gz;
using namespace Game;

#define NUM_BASE_DEMO_FLAGS 13
#define BASE_DEMO_FLAGS                                                                                                                   \
	DEMO_Pluck_First_Pikmin, DEMO_Discover_Treasure, DEMO_First_Gate_Down, DEMO_Reds_In_Onion, DEMO_First_Nectar_Use, DEMO_Day_One_Start, \
	    DEMO_Meet_Red_Pikmin, DEMO_Louie_Finds_Red_Onion, DEMO_Unlock_Captain_Switch, DEMO_First_Use_Louie, DEMO_Reunite_Captains,        \
	    DEMO_You_Appear_Lost, DEMO_First_Number_Pellet

#define NUM_AW_DEMO_FLAGS NUM_BASE_DEMO_FLAGS + 7
#define AW_DEMO_FLAGS                                                                                                  \
	BASE_DEMO_FLAGS, DEMO_Find_Cave_Deeper_Hole, DEMO_Find_Cave_Geyser, DEMO_First_Cave_Enter, DEMO_First_Cave_Return, \
	    DEMO_First_Globe_Day_End, DEMO_Purple_Candypop, DEMO_First_Corpse_In_Cave

#define NUM_HOB_DEMO_FLAGS NUM_AW_DEMO_FLAGS + 4
#define HOB_DEMO_FLAGS \
	AW_DEMO_FLAGS, DEMO_Enter_Awakening_Wood, DEMO_Purples_In_Ship, DEMO_Reds_Purples_Tutorial, DEMO_Pikmin_In_Danger_Fire

#define NUM_WFG_DEMO_FLAGS NUM_HOB_DEMO_FLAGS + 3
#define WFG_DEMO_FLAGS     HOB_DEMO_FLAGS, DEMO_White_Candypop, DEMO_Whites_Digging, DEMO_Eat_White_Pikmin

#define NUM_SH_DEMO_FLAGS NUM_WFG_DEMO_FLAGS + 3
#define SH_DEMO_FLAGS     WFG_DEMO_FLAGS, DEMO_Find_Blue_Onion, DEMO_Pikmin_In_Danger_Poison, DEMO_Pikmin_In_Danger_Water

#define NUM_VOR2_DEMO_FLAGS NUM_SH_DEMO_FLAGS + 1
#define VOR2_DEMO_FLAGS     SH_DEMO_FLAGS, DEMO_Whites_In_Ship

#define NUM_GK_DEMO_FLAGS NUM_VOR2_DEMO_FLAGS + 2
#define GK_DEMO_FLAGS     VOR2_DEMO_FLAGS, DEMO_Find_Yellow_Onion, DEMO_Enter_Perplexing_Pool

PresetMgr::PresetMgr()
{
	presets.push((new Preset("everything", General))
	                 ->set_pikmin(Flower, Red, 20)
	                 ->set_pikmin(Flower, Yellow, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_sprays(false, 0, true, 16));
	presets.push((new Preset("EC", PoD))
	                 ->set_pikmin(Flower, Red, 46)
	                 ->set_pikmin(Leaf, Red, 6)
	                 ->set_time(8.0f)
	                 ->set_cutscene_flags(NUM_BASE_DEMO_FLAGS, (DemoFlags[NUM_BASE_DEMO_FLAGS]) { BASE_DEMO_FLAGS }));
	presets.push((new Preset("enter HoB", PoD))
	                 ->set_onion_pikmin(Flower, Red, 36)
	                 ->set_onion_pikmin(Leaf, Red, 6)
	                 ->set_onion_pikmin(Leaf, Purple, 10)
	                 ->set_cutscene_flags(NUM_AW_DEMO_FLAGS, (DemoFlags[NUM_AW_DEMO_FLAGS]) { AW_DEMO_FLAGS }));
	presets.push((new Preset("HoB1-2", PoD))
	                 ->set_pikmin(Flower, Red, 62)
	                 ->set_pikmin(Flower, Purple, 10)
	                 ->set_time(8.0f)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS }));
	presets.push((new Preset("HoB3-4", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 10)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_time(8.0f)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS }));
	presets.push((new Preset("HoB5", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(8.0f)
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS }));
	presets.push((new Preset("WFG1-WFG3", PoD))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(8.0f)
	                 ->set_upgrades(1,
	                                (OlimarData::ItemIndex[1]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                })
	                 ->set_cutscene_flags(NUM_HOB_DEMO_FLAGS, (DemoFlags[NUM_HOB_DEMO_FLAGS]) { HOB_DEMO_FLAGS }));
	presets.push((new Preset("WFG4-WFG5", PoD))
	                 ->set_pikmin(Flower, Red, 37)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(8.0f)
	                 ->set_upgrades(1,
	                                (OlimarData::ItemIndex[1]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                })
	                 ->set_cutscene_flags(NUM_WFG_DEMO_FLAGS, (DemoFlags[NUM_WFG_DEMO_FLAGS]) { WFG_DEMO_FLAGS }));
	presets.push((new Preset("enter SH", PoD))
	                 ->set_pikmin(Flower, Red, 37)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(8.5f)
	                 ->set_upgrades(2,
	                                (OlimarData::ItemIndex[2]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                })
	                 ->set_cutscene_flags(NUM_WFG_DEMO_FLAGS, (DemoFlags[NUM_WFG_DEMO_FLAGS]) { WFG_DEMO_FLAGS }));
	presets.push((new Preset("SH1-2", PoD))
	                 ->set_pikmin(Flower, Red, 34)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(10.5f)
	                 ->set_upgrades(2,
	                                (OlimarData::ItemIndex[2]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("SH3-7", PoD))
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_time(10.5f)
	                 ->set_upgrades(2,
	                                (OlimarData::ItemIndex[2]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("enter BK (15/5)", PoD))
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_time(10.5f)
	                 ->set_upgrades(3,
	                                (OlimarData::ItemIndex[3]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("enter BK (20)", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_time(10.5f)
	                 ->set_upgrades(3,
	                                (OlimarData::ItemIndex[3]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("enter BK (25)", PoD))
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 24)
	                 ->set_onion_pikmin(Leaf, Blue, 16)
	                 ->set_time(10.5f)
	                 ->set_upgrades(3,
	                                (OlimarData::ItemIndex[3]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("BK", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 28)
	                 ->set_pikmin(Leaf, Blue, 32)
	                 ->set_time(13.0f)
	                 ->set_upgrades(3,
	                                (OlimarData::ItemIndex[3]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("day 6 CR", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Leaf, Blue, 60)
	                 ->set_upgrades(4,
	                                (OlimarData::ItemIndex[4]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                })
	                 ->set_cutscene_flags(NUM_SH_DEMO_FLAGS, (DemoFlags[NUM_SH_DEMO_FLAGS]) { SH_DEMO_FLAGS }));
	presets.push((new Preset("enter SCx", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_upgrades(4,
	                                (OlimarData::ItemIndex[4]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                })
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS }));
	presets.push((new Preset("SCx1-4", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_time(9.5f)
	                 ->set_upgrades(4,
	                                (OlimarData::ItemIndex[4]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                })
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS }));
	presets.push((new Preset("SCx5-FC5", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_time(9.5f)
	                 ->set_upgrades(4,
	                                (OlimarData::ItemIndex[4]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                })
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS }));
	presets.push((new Preset("FC6-7", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_time(10.5f)
	                 ->set_upgrades(5,
	                                (OlimarData::ItemIndex[5]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                    OlimarData::ODII_BruteKnuckles,
	                                })
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS }));
	presets.push((new Preset("CoS", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 40)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_upgrades(5,
	                                (OlimarData::ItemIndex[5]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                    OlimarData::ODII_BruteKnuckles,
	                                })
	                 ->set_cutscene_flags(NUM_VOR2_DEMO_FLAGS, (DemoFlags[NUM_VOR2_DEMO_FLAGS]) { VOR2_DEMO_FLAGS }));
	presets.push((new Preset("GK", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 34)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_time(8.5f)
	                 ->set_upgrades(5,
	                                (OlimarData::ItemIndex[5]) {
	                                    OlimarData::ODII_PrototypeDetector,
	                                    OlimarData::ODII_FiveManNapsack,
	                                    OlimarData::ODII_JusticeAlloy,
	                                    OlimarData::ODII_ForgedCourage,
	                                    OlimarData::ODII_BruteKnuckles,
	                                })
	                 ->set_cutscene_flags(NUM_GK_DEMO_FLAGS, (DemoFlags[NUM_GK_DEMO_FLAGS]) { GK_DEMO_FLAGS }));
}

Preset* PresetMgr::create()
{
	Preset* preset           = new Preset(nullptr, Generated);
	preset->spicies_unlocked = p2gz->spray_editor->get_spicies_unlocked();
	preset->bitters_unlocked = p2gz->spray_editor->get_bitters_unlocked();
	preset->num_spicies      = p2gz->spray_editor->get_spicies();
	preset->num_bitters      = p2gz->spray_editor->get_bitters();

	preset->squad.clear();
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Piki* piki = *iterator;
		if (piki->isAlive() && !piki->isZikatu()) {
			preset->squad(piki)++;
		}
	}

	preset->onion_pikis.clear();
	preset->onion_pikis = Game::playData->mPikiContainer;

	return preset;
}

Preset* PresetMgr::find(const char* name, PresetCategory category)
{
	for (size_t i = 0; i < presets.len(); i++) {
		Preset* preset = presets[i];
		GZASSERTLINE(preset);
		if (category == preset->category && strcmp(preset->name, name) == 0) {
			return preset;
		}
	}

	OSReport("Attempted to find preset \"%s\" that does not exist\n", name);
	return nullptr;
}

namespace gz {
typedef enum Cave {
	AG  = 0,
	EC  = 1,
	SCx = 2,
	FC  = 3,
	HoB = 4,
	WFG = 5,
	BK  = 6,
	SH  = 7,
	CoS = 8,
	GK  = 9,
	SR  = 10,
	SmC = 11,
	CoC = 12,
	HoH = 13,
	DD  = 14
} Cave;
}; // namespace gz

gz::Cave which_cave(u32 area, u32 cave)
{
	if (cave == 0) {
		return AG;
	}

	switch (area) {
	case 0:
		GZASSERTLINE(cave < 4);
		return static_cast<gz::Cave>(EC + cave - 1);
	case 1:
		GZASSERTLINE(cave < 5);
		return static_cast<gz::Cave>(HoB + cave - 1);
	case 2:
		GZASSERTLINE(cave < 5);
		return static_cast<gz::Cave>(CoS + cave - 1);
	case 3:
		GZASSERTLINE(cave < 4);
		return static_cast<gz::Cave>(CoC + cave - 1);
	}

	GZASSERTLINE(false);
}

Preset* PresetMgr::suggested_preset(WarpDestination dest, PresetCategory category)
{
	gz::Cave cave_e = which_cave(dest.area, dest.cave);
	if (category == General) {
		return nullptr;
	}

	if (category == PoD) {
		switch (cave_e) {
		case EC:
			return find("EC", PoD);
		case HoB:
			if (dest.sublevel < 2)
				return find("HoB1-2", PoD);
			else if (dest.sublevel < 4)
				return find("HoB3-4", PoD);
			else
				return find("HoB5", PoD);
		case WFG:
			if (dest.sublevel < 3)
				return find("WFG1-WFG3", PoD);
			else
				return find("WFG4-WFG5", PoD);
		case SH:
			if (dest.sublevel < 2)
				return find("SH1-2", PoD);
			else
				return find("SH3-7", PoD);
		case BK:
			return find("BK", PoD);
		case SCx:
			if (dest.sublevel < 4)
				return find("SCx1-4", PoD);
			else
				return find("SCx5-FC5", PoD);
		case FC:
			if (dest.sublevel < 5)
				return find("SCx5-FC5", PoD);
			else
				return find("FC6-7", PoD);
		case CoS:
			return find("CoS", PoD);
		case GK:
			return find("GK", PoD);
		case AG:
			if (dest.area == 0) {
				if (dest.day == 5)
					return find("day 6 CR", PoD);
				else
					return find("enter SCx", PoD);
			} else if (dest.area == 1) {
				if (dest.day < 5)
					return find("enter HoB", PoD);
				else
					return find("enter BK (20)", PoD);
			} else if (dest.area == 2) {
				return find("CoS", PoD);
			}
		}
	} else if (category == AT) {
		// TODO: AT presets
	}

	return nullptr;
}
