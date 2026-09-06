#include <p2gz/Utility.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/p2gz.h>
#include <p2gz/SegmentHistory.h>
#include <p2gz/Preset.h>
#include <types.h>
#include <System.h>
#include <Game/GameSystem.h>
#include <Screen/Game2DMgr.h>
#include <Game/gamePlayData.h>
#include <Game/PikiMgr.h>
#include <p2gz/SegmentSnapshot.h>

using namespace gz;

void SegmentHistory::draw_2d()
{
	if (!Screen::gGame2DMgr || !Screen::gGame2DMgr->mScreenMgr) {
		return;
	}
	SceneType scene_type = Screen::gGame2DMgr->mScreenMgr->getSceneType();

	bool gz_menu_open      = p2gz->menu->is_open();
	bool is_paused         = Game::gameSystem
	                      && (scene_type == SCENE_PAUSE_MENU_DOUKUTU || scene_type == SCENE_PAUSE_MENU_ITEMS || scene_type == SCENE_PAUSE_MENU_MAP
	                          || scene_type == SCENE_PAUSE_MENU_CONTROLS);
	bool is_in_load_screen = scene_type == SCENE_FLOOR && started_creating_map;
	bool is_paused_in_cave = in_cave_play() && (is_paused || gz_menu_open);

	if (is_paused_in_cave || is_in_load_screen) {
		draw_cur_seed();
	}

	// don't show the controls when in race mode
	bool race_active = p2gz->race_mode && p2gz->race_mode->is_active();

	if (!race_active && (entering_next_segment || (cur_segment() && p2gz->menu->is_root_open()))) {
		draw_reset_controls();
	}
}

Segment* SegmentHistory::cur_segment()
{
	return current_segment;
}

void SegmentHistory::retry_segment()
{
	const Segment* current_segment = cur_segment();
	if (!current_segment || !current_segment->preset || !current_segment->preset->segment_snapshot) {
		return;
	}

	p2gz->warp->set_dest(current_segment->dest);
	p2gz->warp->set_preset(current_segment->preset, PS_Generated);
	p2gz->warp->do_warp();

	entering_next_segment = false;
}

void SegmentHistory::retry_same_seed()
{
	const Segment* current_segment = cur_segment();
	if (!current_segment || !current_segment->preset || !current_segment->preset->segment_snapshot) {
		return;
	}

	p2gz->warp->set_dest(current_segment->dest);
	p2gz->warp->set_seed(current_segment->seed);
	p2gz->warp->set_preset(current_segment->preset, PS_Generated);
	p2gz->warp->do_warp();

	entering_next_segment = false;
}

void SegmentHistory::retry_cave()
{
	const Segment* segment = cur_segment();
	if (!segment || !segment->preset || !segment->preset->segment_snapshot || !segment->dest.cave) {
		return;
	}
	WarpDestination dest    = segment->dest;
	PresetCategory category = segment->preset->category;
	dest.sublevel           = 0;
	p2gz->warp->set_dest(dest);
	if (cave_floor0_preset && cave_floor0_dest.area == dest.area && cave_floor0_dest.cave == dest.cave) {
		p2gz->warp->set_preset(cave_floor0_preset, PS_Generated);
	} else {
		p2gz->warp->set_preset(p2gz->preset_mgr->suggested_preset(dest, category), PS_Suggested);
	}
	p2gz->warp->do_warp();
	entering_next_segment = false;
}

void SegmentHistory::update()
{
	// don't allow retrying from hole-in for race mode
	if (p2gz->race_mode && p2gz->race_mode->is_active()) {
		return;
	}

	const u32 btn = p2gz->controller->getButtonDown();
	if (entering_next_segment || p2gz->menu->is_root_open()) {
		// Retry same sublevel, random seed
		if (btn & Controller::PRESS_X) {
			retry_segment();
			return;
		}

		// Retry same sublevel, same seed
		if (btn & Controller::PRESS_Y && in_cave_play()) {
			retry_same_seed();
			return;
		}

		// Restart cave from the beginning
		if (btn & Controller::PRESS_L && in_cave_play()) {
			retry_cave();
			return;
		}
	}
}

J2DPrint init_j2d(f32 glyph_size = 24.0)
{
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();

	j2d.mGlyphWidth  = glyph_size;
	j2d.mGlyphHeight = glyph_size;

	JUtility::TColor color = JUtility::TColor(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	return j2d;
}

void SegmentHistory::draw_cur_seed()
{
	const Segment* seg = cur_segment();
	if (!seg) {
		return;
	}
	const u32 seed = seg->seed;

	J2DPrint j2d = init_j2d();

	// coordinates determined experimentally
	j2d.print(208.0f, 440.0f, "0x%08X", seed);
}

void draw_ctrl(J2DPrint& j2d, const f32 glyph_size, f32& z, const char* img_name, const char* msg)
{
	f32 x = System::getRenderModeWidth() - 48.0f;
	p2gz->images->draw(img_name, x, z - p2gz->images->height() / 2.0f);
	x -= p2gz->images->spacing();

	j2d.initiate();
	x -= j2d.getWidth("%s", msg);
	j2d.print(x, z + (glyph_size / 2.0f) - 4.0f, msg);

	z += p2gz->images->height() + 4.0f;
}

void SegmentHistory::draw_reset_controls()
{
	const f32 glyph_size = 18.0f;
	J2DPrint j2d         = init_j2d(glyph_size);

	f32 z = 280.0f;

	if (in_cave_play()) {
		draw_ctrl(j2d, glyph_size, z, "x_btn", "replay sublevel");
		draw_ctrl(j2d, glyph_size, z, "y_btn", "replay seed");
		draw_ctrl(j2d, glyph_size, z, "l_btn", "restart cave");
	} else {
		draw_ctrl(j2d, glyph_size, z, "x_btn", "replay segment");
	}
}

Segment* SegmentHistory::start_segment()
{
	JKRHeap* prev_heap            = sys->mSysHeap->becomeCurrentHeap();
	Game::SingleGameSection* game = get_SGS();
	Preset* warp_preset           = p2gz->warp->warping ? p2gz->warp->get_preset_during_warp() : nullptr;
	bool new_warp                 = p2gz->warp->warping && (!warp_preset || !warp_preset->segment_snapshot);
	if (cave_floor0_preset && (!game->mInCave || new_warp)) {
		cave_floor0_preset->del();
		cave_floor0_preset = nullptr;
	}
	delete current_segment;
	current_segment = new Segment();
	prev_heap->becomeCurrentHeap();
	return current_segment;
}

void SegmentHistory::capture_segment()
{
	Segment* segment = cur_segment();
	if (!segment || !segment->preset) {
		return;
	}
	Preset* preset = segment->preset;
	if (preset->segment_snapshot) {
		return;
	}
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	if (segment->dest.cave == 0 && cave_floor0_preset) {
		cave_floor0_preset->del();
		cave_floor0_preset = nullptr;
	}
	preset->squad.clear();
	preset->onion_pikis.clear();
	preset->day  = Game::gameSystem->mTimeMgr->mDayCount + 1;
	preset->time = Game::gameSystem->mTimeMgr->mCurrentTimeOfDay;
	PresetMgr::fill_current_pikis(preset);
	preset->segment_snapshot = new SegmentSnapshot();
	preset->segment_snapshot->capture(segment->dest.cave == 0);

	if (segment->dest.cave != 0 && segment->dest.sublevel == 0) {
		if (cave_floor0_preset) {
			cave_floor0_preset->del();
		}
		cave_floor0_preset = preset;
		cave_floor0_preset->ref();
		cave_floor0_dest = segment->dest;
	}
	prev_heap->becomeCurrentHeap();
}
