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

using namespace gz;

void SegmentHistory::draw_2d()
{
	if (!Screen::gGame2DMgr || !Screen::gGame2DMgr->mScreenMgr) {
		return;
	}
	SceneType scene_type = Screen::gGame2DMgr->mScreenMgr->getSceneType();

	bool gz_menu_open = p2gz->menu->is_open();
	bool is_paused    = Game::gameSystem
	              && (scene_type == SCENE_PAUSE_MENU_DOUKUTU || scene_type == SCENE_PAUSE_MENU_ITEMS || scene_type == SCENE_PAUSE_MENU_MAP
	                  || scene_type == SCENE_PAUSE_MENU_CONTROLS);
	bool is_in_load_screen = scene_type == SCENE_FLOOR && started_creating_map;
	bool is_paused_in_cave = in_cave_play() && (is_paused || gz_menu_open);

	if (is_paused_in_cave || is_in_load_screen) {
		draw_cur_seed();
	}

	if (entering_next_segment || (cur_segment() && p2gz->menu->is_root_open())) {
		draw_reset_controls(entering_next_segment);
	}
}

Segment* SegmentHistory::cur_segment()
{
	if (segments.len() > 0) {
		return segments.peek();
	}
	return nullptr;
}

void SegmentHistory::retry_segment()
{
	const Segment* current_segment = cur_segment();
	if (!current_segment) {
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
	if (!current_segment) {
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
	const Segment* current_segment = cur_segment();
	if (!current_segment) {
		return;
	}
	WarpDestination current_dest = current_segment->dest;

	if (segments.len() == 0) {
		return;
	}

	// Find segment for the first floor of the cave
	const Segment* floor0_segment = nullptr;
	for (size_t i = 0; i < segments.len(); i++) {
		const Segment* this_segment = segments.peekN(i);
		if (!this_segment) {
			break;
		}

		if (this_segment->dest.cave == current_dest.cave) {
			if (this_segment->dest.area == current_dest.area && this_segment->dest.sublevel == 0) {
				floor0_segment = this_segment;
				break;
			}
		} else {
			break;
		}
	}

	if (!floor0_segment) {
		floor0_segment = current_segment;
	}

	WarpDestination floor0_dest = floor0_segment->dest;
	if (floor0_dest.sublevel != 0) {
		floor0_dest.sublevel = 0;
		// If we don't find history for floor 0 in this cave, get the recommended preset for it.
		// TODO: currently assumes the PoD preset. Adjust to reflect AT in the future
		PresetCategory cat = PoD;
		if (floor0_segment->preset && floor0_segment->preset->category != Generated) {
			cat = floor0_segment->preset->category;
		}
		p2gz->warp->set_dest(floor0_dest);
		p2gz->warp->set_preset(p2gz->preset_mgr->suggested_preset(floor0_dest, cat), PS_Suggested);
	} else {
		p2gz->warp->set_dest(floor0_dest);
		p2gz->warp->set_preset(floor0_segment->preset, PS_Generated);
	}

	p2gz->warp->do_warp();
	entering_next_segment = false;
}

void SegmentHistory::update()
{
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
	}

	if (entering_next_segment && in_cave_play()) {
		// Restart cave from the beginning
		if (btn & Controller::PRESS_L) {
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

void SegmentHistory::draw_reset_controls(bool draw_cave_retry)
{
	const f32 glyph_size = 18.0f;
	J2DPrint j2d         = init_j2d(glyph_size);

	f32 z = 280.0f;

	if (in_cave_play()) {
		draw_ctrl(j2d, glyph_size, z, "x_btn", "replay sublevel");
		draw_ctrl(j2d, glyph_size, z, "y_btn", "replay seed");
		if (draw_cave_retry) {
			draw_ctrl(j2d, glyph_size, z, "l_btn", "restart cave");
		}
	} else {
		draw_ctrl(j2d, glyph_size, z, "x_btn", "replay segment");
	}
}

Segment* SegmentHistory::start_segment()
{
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	if (segments.atCapacity()) {
		Segment* oldestSegment = segments.getLast();
		delete oldestSegment;
	}

	Segment* segment = new Segment();
	segment->preset  = nullptr;
	segments.push(segment);

	prev_heap->becomeCurrentHeap();

	return segment;
}

void SegmentHistory::record_squad()
{
	Segment* segment = cur_segment();
	if (!segment || !segment->preset || segment->preset->category != Generated) {
		return;
	}

	segment->preset->squad.clear();
	segment->preset->onion_pikis.clear();
	PresetMgr::fill_current_pikis(segment->preset);
}
