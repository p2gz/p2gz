#include <p2gz/p2gz.h>
#include <p2gz/Preset.h>
#include <p2gz/warp.h>
#include <Game/Piki.h>
#include <System.h>
#include <Game/PikiMgr.h>

using namespace gz;
using namespace Game;

PresetMgr::PresetMgr()
{
	presets.push((new Preset("everything", General))
	                 ->set_pikmin(Flower, Red, 20)
	                 ->set_pikmin(Flower, Yellow, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_sprays(false, 0, true, 16));
	presets.push((new Preset("EC", PoD))->set_pikmin(Flower, Red, 46)->set_pikmin(Leaf, Red, 6));
	presets.push((new Preset("enter HoB", PoD))
	                 ->set_onion_pikmin(Flower, Red, 36)
	                 ->set_onion_pikmin(Leaf, Red, 6)
	                 ->set_onion_pikmin(Leaf, Purple, 10));
	presets.push((new Preset("HoB1-2", PoD))->set_pikmin(Flower, Red, 62)->set_pikmin(Flower, Purple, 10));
	presets.push((new Preset("HoB3-4", PoD))->set_pikmin(Flower, Red, 52)->set_pikmin(Flower, Purple, 10)->set_pikmin(Leaf, Purple, 10));
	presets.push((new Preset("HoB5-WFG3", PoD))->set_pikmin(Flower, Red, 52)->set_pikmin(Flower, Purple, 20));
	presets.push(
	    (new Preset("WFG4-enter SH", PoD))->set_pikmin(Flower, Red, 37)->set_pikmin(Flower, Purple, 20)->set_pikmin(Flower, White, 15));
	presets.push((new Preset("SH1-2", PoD))->set_pikmin(Flower, Red, 34)->set_pikmin(Flower, Purple, 20)->set_pikmin(Flower, White, 15));
	presets.push((new Preset("SH3-7", PoD))
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5));
	presets.push((new Preset("enter BK (15/5)", PoD))
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("enter BK (20)", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("enter BK (25)", PoD))
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 24)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("BK", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 28)
	                 ->set_pikmin(Leaf, Blue, 32));
	presets.push((new Preset("day 6 CR", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Leaf, Blue, 60));
	presets.push((new Preset("enter SCx", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("SCx1-4", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("SCx5-FC", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Flower, Blue, 25));
	presets.push((new Preset("CoS", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 40)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("GK", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 34)
	                 ->set_onion_pikmin(Flower, Red, 29));
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
				return find("HoB5-WFG3", PoD);
		case WFG:
			if (dest.sublevel < 3)
				return find("HoB5-WFG3", PoD);
			else
				return find("WFG4-enter SH", PoD);
		case SH:
			if (dest.sublevel < 2)
				return find("SH1-2", PoD);
			else
				return find("SH3-7", PoD);
		case BK:
			return find("BK", PoD);
		case SCx:
			if (dest.sublevel < 4)
				return find("SCx1-3", PoD);
			else
				return find("SCx5-FC", PoD);
		case FC:
			return find("SCx5-FC", PoD);
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
				if (dest.day == 2)
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
