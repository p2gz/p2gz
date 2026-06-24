#include <p2gz/p2gz.h>
#include <p2gz/Preset.h>
#include <p2gz/warp.h>
#include <Game/Piki.h>
#include <System.h>
#include <Game/PikiMgr.h>
#include <types.h>
#include <Vector3.h>

using namespace gz;

static const char* ALL_PRESETS_FILE_PATH = "presets/all_presets.txt";

PresetMgr::PresetMgr()
{
	void* all_presets_file = JKRDvdRipper::loadToMainRAM(ALL_PRESETS_FILE_PATH, nullptr, Switch_0, 0, nullptr,
	                                                     JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
	GZEXPECT(all_presets_file, "%s not found", ALL_PRESETS_FILE_PATH);

	RamStream filename_stream(all_presets_file, -1);
	filename_stream.setMode(STREAM_MODE_TEXT, 1);

	const int num_files = filename_stream.readInt();
	for (int i = 0; i < num_files; i++) {
		const char* filename = filename_stream.readString(nullptr, 0);
		if (strlen(filename) == 0) {
			break;
		}

		PresetPreview* preview = new PresetPreview();
		preview->read(filename);
		preset_previews.push(preview);
	}
	OSReport("Loaded %d preset previews\n", preset_previews.len());

	delete[] all_presets_file;

	// Code to serialize and print all presets. Left here for future use
	// (but hopefully shouldn't be necessary.)
	// FOREACH_VEC(presets)
	// {
	// 	char buf[10000];
	// 	RamStream stream(&buf, 10000);
	// 	stream.setMode(STREAM_MODE_TEXT, true);
	// 	presets[i]->write(stream);
	// 	stream.setMode(STREAM_MODE_BINARY, true);
	// 	stream.writeByte('\0');
	// 	OSReport("%s\n", buf);
	// }
}

void PresetMgr::init()
{
	static_cast<PresetMenuOption*>(p2gz->menu->get_option("warp/preset"))->init();
}

Preset* PresetMgr::create()
{
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	Preset* preset = new Preset();
	preset->name   = "generated preset";
	preset->origin = PO_Generated;

	preset->spicies_unlocked = p2gz->spray_editor->get_spicies_unlocked();
	preset->bitters_unlocked = p2gz->spray_editor->get_bitters_unlocked();
	preset->num_spicies      = p2gz->spray_editor->get_spicies();
	preset->num_bitters      = p2gz->spray_editor->get_bitters();

	preset->squad.clear();
	preset->onion_pikis.clear();

	preset->pokos          = p2gz->poko_editor->get_pokos();
	preset->apply_pokos    = true;
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

	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->isAlive() && !piki->isZikatu() && !piki->isWildBulbmin()) {
			preset->squad(piki)++;
		}
	}
	preset->onion_pikis = Game::playData->mPikiContainer;
}

Preset* PresetMgr::load_preset(PresetPreview* preview)
{
	if (!preview) {
		return nullptr;
	}

	for (u32 i = 0; i < presets.len(); i++) {
		if (presets[i]->name == preview->name) {
			return presets[i];
		}
	}

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	GZEXPECT(preview->filename, "PresetPreview %s has no filename", preview->name);
	Preset* preset = new Preset();
	preset->read_file(preview->filename);
	preset->preview = preview;
	presets.push(preset);

	prev_heap->becomeCurrentHeap();
	return preset;
}

// ============================================================================
// Preset grouping tables
//
// For each (category, area, cave), list the presets relevant to that
// destination, in cycle order. Entry 0 is the auto-suggested default.
//
// Cave entries are filtered by the inclusive sublevel range [min, max]
// (max == SUBLEVEL_ANY means no upper bound).
// Above-ground groups are listed per area.
// ============================================================================

static const u8 SUBLEVEL_ANY = 0xFF; // no upper sublevel bound by default

struct PresetGroupEntry {
	const char* name;
	u8 min_sublevel; // inclusive, 0-based
	u8 max_sublevel; // inclusive (SUBLEVEL_ANY = unbounded)
};

struct PresetGroup {
	const PresetGroupEntry* entries;
	size_t count;
};

#define PRESET_GROUP(arr) { arr, sizeof(arr) / sizeof((arr)[0]) }

// ---- PoD caves ----
static const PresetGroupEntry EC_POD[] = { { "EC1", 0, 0 }, { "EC2", 1, SUBLEVEL_ANY } };
static const PresetGroupEntry SCx_POD[]
    = { { "SCx1-4", 0, 3 }, { "SCx1-4 (25w)", 0, 3 }, { "SCx5-8", 4, SUBLEVEL_ANY }, { "SCx5-8 (40w)", 4, SUBLEVEL_ANY } };
static const PresetGroupEntry FC_POD[]
    = { { "FC1-5", 0, 4 }, { "FC1-5 (40w)", 0, 4 }, { "FC6-7", 5, SUBLEVEL_ANY }, { "FC6-7 (40w)", 5, SUBLEVEL_ANY } };
static const PresetGroupEntry HoB_POD[] = { { "HoB1-2", 0, 1 }, { "HoB3-4", 2, 3 }, { "HoB5", 4, SUBLEVEL_ANY } };
static const PresetGroupEntry WFG_POD[] = { { "WFG1-3", 0, 2 }, { "WFG4-5", 3, SUBLEVEL_ANY } };
static const PresetGroupEntry BK_POD[]  = { { "BK", 0, SUBLEVEL_ANY }, { "BK (25w)", 0, SUBLEVEL_ANY } };
static const PresetGroupEntry SH_POD[]  = { { "SH1-2", 0, 1 }, { "SH3-7", 2, SUBLEVEL_ANY } };
static const PresetGroupEntry CoS_POD[] = { { "CoS", 0, SUBLEVEL_ANY }, { "CoS (40w)", 0, SUBLEVEL_ANY } };
static const PresetGroupEntry GK_POD[]  = { { "GK", 0, SUBLEVEL_ANY }, { "GK (40w)", 0, SUBLEVEL_ANY } };

// Indexed by gz::CaveIndex (index 0 = above-ground, handled separately below)
static const PresetGroup POD_GROUPS[CAVE_COUNT] = {
	{ nullptr, 0 },        // CAVE_AboveGround
	PRESET_GROUP(EC_POD),  // CAVE_EC
	PRESET_GROUP(SCx_POD), // CAVE_SCx
	PRESET_GROUP(FC_POD),  // CAVE_FC
	PRESET_GROUP(HoB_POD), // CAVE_HoB
	PRESET_GROUP(WFG_POD), // CAVE_WFG
	PRESET_GROUP(BK_POD),  // CAVE_BK
	PRESET_GROUP(SH_POD),  // CAVE_SH
	PRESET_GROUP(CoS_POD), // CAVE_CoS
	PRESET_GROUP(GK_POD),  // CAVE_GK
	{ nullptr, 0 },        // CAVE_SR  (not visited in PoD)
	{ nullptr, 0 },        // CAVE_SmC (not visited in PoD)
	{ nullptr, 0 },        // CAVE_CoC (not visited in PoD)
	{ nullptr, 0 },        // CAVE_HoH (not visited in PoD)
	{ nullptr, 0 },        // CAVE_DD  (not visited in PoD)
};

// ---- PoD above-ground (indexed by area / gz::CourseIndex) ----
static const PresetGroupEntry POD_AG_VoR[] = {
	{ "Day 1", 0, SUBLEVEL_ANY },          { "Day 2", 0, SUBLEVEL_ANY },           { "Day 3", 0, SUBLEVEL_ANY },
	{ "Day 4", 0, SUBLEVEL_ANY },          { "day 6 CR", 0, SUBLEVEL_ANY },        { "day 6 CR (25w)", 0, SUBLEVEL_ANY },
	{ "enter SCx", 0, SUBLEVEL_ANY },      { "enter SCx (25w)", 0, SUBLEVEL_ANY }, { "enter FC", 0, SUBLEVEL_ANY },
	{ "enter FC (40w)", 0, SUBLEVEL_ANY },
};
static const PresetGroupEntry POD_AG_AW[] = {
	{ "enter HoB", 0, SUBLEVEL_ANY },        { "enter WFG", 0, SUBLEVEL_ANY },      { "enter SH", 0, SUBLEVEL_ANY },
	{ "enter BK (15/5w)", 0, SUBLEVEL_ANY }, { "enter BK (20w)", 0, SUBLEVEL_ANY }, { "enter BK (25w)", 0, SUBLEVEL_ANY },
};
static const PresetGroupEntry POD_AG_PP[] = {
	{ "enter CoS", 0, SUBLEVEL_ANY },
	{ "enter CoS (40w)", 0, SUBLEVEL_ANY },
	{ "enter GK", 0, SUBLEVEL_ANY },
	{ "enter GK (40w)", 0, SUBLEVEL_ANY },
};
static const PresetGroup POD_AG_GROUPS[4] = {
	PRESET_GROUP(POD_AG_VoR), // COURSE_VoR
	PRESET_GROUP(POD_AG_AW),  // COURSE_AW
	PRESET_GROUP(POD_AG_PP),  // COURSE_PP
	{ nullptr, 0 },           // COURSE_WW (PoD ends before WW)
};

// ---- AT caves ----
static const PresetGroupEntry EC_AT[]
    = { { "EC1 (1st visit)", 0, 0 }, { "EC2 (1st visit)", 1, SUBLEVEL_ANY }, { "EC1-2 (2nd visit)", 0, SUBLEVEL_ANY } };
static const PresetGroupEntry HoB_AT[] = { { "HoB1-2", 0, 1 }, { "HoB3", 2, 2 }, { "HoB4", 3, 3 }, { "HoB5", 4, SUBLEVEL_ANY } };
static const PresetGroupEntry WFG_AT[] = { { "WFG1", 0, 0 }, { "WFG2-3", 1, 2 }, { "WFG4-5", 3, SUBLEVEL_ANY } };
static const PresetGroupEntry SH_AT[]  = { { "SH1-3", 0, 2 }, { "SH4-7", 3, SUBLEVEL_ANY } };
static const PresetGroupEntry BK_AT[]  = { { "BK1-4", 0, 3 }, { "BK5-6", 4, 5 }, { "BK7", 6, SUBLEVEL_ANY } };
static const PresetGroupEntry SCx_AT[] = { { "SCx1-3", 0, 2 }, { "SCx4", 3, 3 }, { "SCx5-8", 4, 7 }, { "SCx9", 8, SUBLEVEL_ANY } };
static const PresetGroupEntry FC_AT[]  = { { "FC1", 0, 0 }, { "FC2-3", 1, 2 }, { "FC4", 3, 3 },           { "FC5", 4, 4 },
	                                       { "FC6", 5, 5 }, { "FC7", 6, 6 },   { "FC8", 7, SUBLEVEL_ANY } };
static const PresetGroupEntry CoS_AT[] = { { "CoS1-2", 0, 1 }, { "CoS3-5", 2, SUBLEVEL_ANY } };
static const PresetGroupEntry GK_AT[]  = { { "GK1-5", 0, 4 }, { "GK6", 5, SUBLEVEL_ANY } };
static const PresetGroupEntry SR_AT[]  = { { "SR1-4", 0, 3 }, { "SR5", 4, 4 }, { "SR6", 5, 5 }, { "SR7", 6, SUBLEVEL_ANY } };
static const PresetGroupEntry SmC_AT[] = { { "SmC1-2", 0, 1 }, { "SmC3", 2, 2 }, { "SmC4", 3, 3 }, { "SmC5", 4, SUBLEVEL_ANY } };
static const PresetGroupEntry CoC_AT[] = { { "CoC1-10", 0, SUBLEVEL_ANY } };
static const PresetGroupEntry HoH_AT[] = { { "HoH1-5", 0, 4 }, { "HoH6-8", 5, 7 }, { "HoH9-15", 8, SUBLEVEL_ANY } };
static const PresetGroupEntry DD_AT[]  = { { "DD1-9", 0, 8 }, { "DD10-14", 9, SUBLEVEL_ANY } };

// Indexed by gz::CaveIndex (index 0 = above-ground, handled separately below)
static const PresetGroup AT_GROUPS[CAVE_COUNT] = {
	{ nullptr, 0 },       // CAVE_AboveGround
	PRESET_GROUP(EC_AT),  // CAVE_EC
	PRESET_GROUP(SCx_AT), // CAVE_SCx
	PRESET_GROUP(FC_AT),  // CAVE_FC
	PRESET_GROUP(HoB_AT), // CAVE_HoB
	PRESET_GROUP(WFG_AT), // CAVE_WFG
	PRESET_GROUP(BK_AT),  // CAVE_BK
	PRESET_GROUP(SH_AT),  // CAVE_SH
	PRESET_GROUP(CoS_AT), // CAVE_CoS
	PRESET_GROUP(GK_AT),  // CAVE_GK
	PRESET_GROUP(SR_AT),  // CAVE_SR
	PRESET_GROUP(SmC_AT), // CAVE_SmC
	PRESET_GROUP(CoC_AT), // CAVE_CoC
	PRESET_GROUP(HoH_AT), // CAVE_HoH
	PRESET_GROUP(DD_AT),  // CAVE_DD
};

// ---- AT above-ground (indexed by area / gz::CourseIndex) ----
static const PresetGroupEntry AT_AG_VoR[] = {
	{ "Day 1", 0, SUBLEVEL_ANY },     { "Day 2", 0, SUBLEVEL_ANY },    { "Day 3", 0, SUBLEVEL_ANY },       { "Day 4", 0, SUBLEVEL_ANY },
	{ "enter SCx", 0, SUBLEVEL_ANY }, { "enter FC", 0, SUBLEVEL_ANY }, { "VoR cleanup", 0, SUBLEVEL_ANY },
};
static const PresetGroupEntry AT_AG_AW[] = {
	{ "enter WFG", 0, SUBLEVEL_ANY }, { "enter HoB", 0, SUBLEVEL_ANY }, { "day 5 cleanup", 0, SUBLEVEL_ANY },
	{ "enter BK", 0, SUBLEVEL_ANY },  { "enter SH", 0, SUBLEVEL_ANY },  { "purple farm", 0, SUBLEVEL_ANY },
};
static const PresetGroupEntry AT_AG_PP[] = {
	{ "enter CoS", 0, SUBLEVEL_ANY },
	{ "enter GK", 0, SUBLEVEL_ANY },
	{ "enter SmC", 0, SUBLEVEL_ANY },
	{ "enter SR", 0, SUBLEVEL_ANY },
};
static const PresetGroupEntry AT_AG_WW[] = {
	{ "enter CoC", 0, SUBLEVEL_ANY },
	{ "enter DD", 0, SUBLEVEL_ANY },
	{ "enter HoH", 0, SUBLEVEL_ANY },
};
static const PresetGroup AT_AG_GROUPS[4] = {
	PRESET_GROUP(AT_AG_VoR), // COURSE_VoR
	PRESET_GROUP(AT_AG_AW),  // COURSE_AW
	PRESET_GROUP(AT_AG_PP),  // COURSE_PP
	PRESET_GROUP(AT_AG_WW),  // COURSE_WW
};

#undef PRESET_GROUP

// Fills `out` with the presets relevant to `dest` for the given category, in cycle order.
void PresetMgr::relevant_presets(PresetCategory category, WarpDestination dest, gz::Vec<PresetPreview*>& out)
{
	out.clear();
	if (category == General) {
		return;
	}

	const bool is_above_ground = (dest.cave == 0);

	const PresetGroup* group;
	if (is_above_ground) {
		GZASSERTLINE(dest.area < 4);
		group = (category == PoD) ? &POD_AG_GROUPS[dest.area] : &AT_AG_GROUPS[dest.area];
	} else {
		CaveIndex cave_e = which_cave(dest.area, dest.cave);
		group            = (category == PoD) ? &POD_GROUPS[cave_e] : &AT_GROUPS[cave_e];
	}

	for (size_t i = 0; i < group->count; i++) {
		const PresetGroupEntry& entry = group->entries[i];
		// Above-ground groups aren't sublevel-filtered (duh), but cave entries are
		if (!is_above_ground && (dest.sublevel < entry.min_sublevel || dest.sublevel > entry.max_sublevel)) {
			continue;
		}
		PresetPreview* preview = find(entry.name, category);
		if (preview) {
			out.push(preview);
		}
	}
}

PresetPreview* PresetMgr::suggested_preset(WarpDestination dest, PresetCategory category)
{
	// suggested preset == first entry of the relevant list
	gz::Vec<PresetPreview*> relevant(8);
	relevant_presets(category, dest, relevant);
	if (relevant.len() == 0) {
		return nullptr;
	}
	return relevant[0];
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

PresetPreview* PresetMgr::find(const char* name, PresetCategory category)
{
	for (size_t i = 0; i < preset_previews.len(); i++) {
		PresetPreview* preset_preview = preset_previews[i];
		GZASSERTLINE(preset_preview);
		if (category == preset_preview->category && strcmp(preset_preview->name, name) == 0) {
			return preset_preview;
		}
	}

	OSReport("Attempted to find preset \"%s\" that does not exist\n", name);
	return nullptr;
}
