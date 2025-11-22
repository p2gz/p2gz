#include <p2gz/p2gz.h>
#include <p2gz/Preset.h>
#include <p2gz/warp.h>
#include <Game/Piki.h>
#include <System.h>
#include <Game/PikiMgr.h>
#include <types.h>
#include <Vector3.h>
#include <p2gz/PresetsPoD.h>

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

	init_pod_presets();
	init_at_presets();
}

Preset* PresetMgr::create()
{
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	Preset* preset           = new Preset(nullptr, Generated);
	preset->name             = "generated preset";
	preset->spicies_unlocked = p2gz->spray_editor->get_spicies_unlocked();
	preset->bitters_unlocked = p2gz->spray_editor->get_bitters_unlocked();
	preset->num_spicies      = p2gz->spray_editor->get_spicies();
	preset->num_bitters      = p2gz->spray_editor->get_bitters();

	preset->squad.clear();
	preset->onion_pikis.clear();

	preset->set_pokos(p2gz->poko_editor->get_pokos());
	preset->upgrades       = p2gz->ek_editor->get_upgrades_bitfield();
	preset->cutscenes      = p2gz->cutscene_mgr->get_cur_cutscenes();
	preset->ek_cutscenes   = p2gz->cutscene_mgr->get_cur_ek_cutscenes();
	preset->cave_cutscenes = p2gz->cutscene_mgr->get_cur_cave_cutscenes();

	prev_heap->becomeCurrentHeap();
	return preset;
}

void PresetMgr::fill_current_pikis(Preset* preset)
{
	GZASSERTLINE(preset);

	Iterator<Piki> iterator(pikiMgr);
	CI_LOOP(iterator)
	{
		Piki* piki = *iterator;
		if (piki->isAlive() && !piki->isZikatu() && !piki->isWildBulbmin()) {
			preset->squad(piki)++;
		}
	}
	preset->onion_pikis = Game::playData->mPikiContainer;
}

Preset* PresetMgr::suggested_preset(WarpDestination dest, PresetCategory category)
{
	gz::CaveIndex cave_e = which_cave(dest.area, dest.cave);
	if (category == General) {
		return nullptr;
	}

	if (category == PoD) {
		switch (cave_e) {
		case CAVE_EC:
			if (dest.sublevel < 1)
				return find("EC1", PoD);
			else
				return find("EC2", PoD);
		case CAVE_HoB:
			if (dest.sublevel < 2)
				return find("HoB1-2", PoD);
			else if (dest.sublevel < 4)
				return find("HoB3-4", PoD);
			else
				return find("HoB5", PoD);
		case CAVE_WFG:
			if (dest.sublevel < 3)
				return find("WFG1-WFG3", PoD);
			else
				return find("WFG4-WFG5", PoD);
		case CAVE_SH:
			if (dest.sublevel < 2)
				return find("SH1-2", PoD);
			else
				return find("SH3-7", PoD);
		case CAVE_BK:
			return find("BK", PoD);
		case CAVE_SCx:
			if (dest.sublevel < 4)
				return find("SCx1-4", PoD);
			else
				return find("SCx5-8", PoD);
		case CAVE_FC:
			if (dest.sublevel < 5)
				return find("FC1-5", PoD);
			else
				return find("FC6-7", PoD);
		case CAVE_CoS:
			return find("CoS", PoD);
		case CAVE_GK:
			return find("GK", PoD);
		case CAVE_AboveGround:
			if (dest.area == 0) {
				if (dest.day == 5)
					return find("day 6 CR", PoD);
				else
					return find("enter SCx", PoD);
			} else if (dest.area == 1) {
				if (dest.day < 5)
					return find("enter HoB", PoD);
				else
					return find("enter BK (20w)", PoD);
			} else if (dest.area == 2) {
				return find("CoS", PoD);
			}
		}
	} else if (category == AT) {
		switch (cave_e) {
		case CAVE_EC:
			if (dest.sublevel < 1)
				return find("EC1 (1st visit)", AT);
			else
				return find("EC2 (1st visit)", AT);
		case CAVE_HoB:
			if (dest.sublevel < 2)
				return find("HoB1-2", AT);
			else if (dest.sublevel < 3)
				return find("HoB3", AT);
			else if (dest.sublevel < 4)
				return find("HoB4", AT);
			else
				return find("HoB5", AT);
		case CAVE_WFG:
			if (dest.sublevel < 1)
				return find("WFG1", AT);
			else if (dest.sublevel < 3)
				return find("WFG2-3", AT);
			else
				return find("WFG4-5", AT);
		case CAVE_SH:
			if (dest.sublevel < 3)
				return find("SH1-3", AT);
			else
				return find("SH4-7", AT);
		case CAVE_BK:
			if (dest.sublevel < 4)
				return find("BK1-4", AT);
			else if (dest.sublevel < 6)
				return find("BK5-6", AT);
			else
				return find("BK7", AT);
		case CAVE_SCx:
			if (dest.sublevel < 3)
				return find("SCx1-3", AT);
			else if (dest.sublevel < 4)
				return find("SCx4", AT);
			else if (dest.sublevel < 8)
				return find("SCx5-8", AT);
			else
				return find("SCx9", AT);
		case CAVE_FC:
			if (dest.sublevel < 1)
				return find("FC1", AT);
			else if (dest.sublevel < 3)
				return find("FC2-3", AT);
			else if (dest.sublevel < 4)
				return find("FC4", AT);
			else if (dest.sublevel < 5)
				return find("FC5", AT);
			else if (dest.sublevel < 6)
				return find("FC6", AT);
			else if (dest.sublevel < 7)
				return find("FC7", AT);
			else
				return find("FC8", AT);
		case CAVE_CoS:
			if (dest.sublevel < 2)
				return find("CoS1-2", AT);
			else
				return find("CoS3-5", AT);
		case CAVE_GK:
			if (dest.sublevel < 5)
				return find("GK1-5", AT);
			else
				return find("GK6", AT);
		case CAVE_SmC:
			if (dest.sublevel < 2)
				return find("SmC1-2", AT);
			else if (dest.sublevel < 3)
				return find("SmC3", AT);
			else if (dest.sublevel < 4)
				return find("SmC4", AT);
			else
				return find("SmC5", AT);
		case CAVE_SR:
			if (dest.sublevel < 4)
				return find("SR1-4", AT);
			else if (dest.sublevel < 5)
				return find("SR5", AT);
			else if (dest.sublevel < 6)
				return find("SR6", AT);
			else
				return find("SR7", AT);
		case CAVE_CoC:
			return find("CoC1-10", AT);
		case CAVE_DD:
			if (dest.sublevel < 9)
				return find("DD1-9", AT);
			else
				return find("DD10-14", AT);
		case CAVE_HoH:
			if (dest.sublevel < 5)
				return find("HoH1-5", AT);
			else if (dest.sublevel < 8)
				return find("HoH6-8", AT);
			else
				return find("HoH9-15", AT);
		case CAVE_AboveGround:
			if (dest.area == COURSE_VoR) {
				if (dest.day == 8)
					return find("VoR cleanup", AT);
				else
					return find("enter SCx", AT);
			} else if (dest.area == COURSE_AW) {
				if (dest.day < 5)
					return find("enter HoB", AT);
				else
					return find("day 5 cleanup", AT);
			} else if (dest.area == COURSE_PP) {
				return find("enter CoS", AT);
			} else if (dest.area == COURSE_WW) {
				return find("enter CoC", AT);
			}
		}
	}

	return nullptr;
}

gz::CaveIndex PresetMgr::which_cave(u32 area, u32 cave)
{
	if (cave == 0) {
		return CAVE_AboveGround;
	}

	switch (area) {
	case 0:
		GZASSERTLINE(cave < 4);
		return static_cast<gz::CaveIndex>(CAVE_EC + cave - 1);
	case 1:
		GZASSERTLINE(cave < 5);
		return static_cast<gz::CaveIndex>(CAVE_HoB + cave - 1);
	case 2:
		GZASSERTLINE(cave < 5);
		return static_cast<gz::CaveIndex>(CAVE_CoS + cave - 1);
	case 3:
		GZASSERTLINE(cave < 4);
		return static_cast<gz::CaveIndex>(CAVE_CoC + cave - 1);
	}

	GZASSERTLINE(false);
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
