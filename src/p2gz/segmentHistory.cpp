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

	bool is_paused = Game::gameSystem
	              && (scene_type == SCENE_PAUSE_MENU_DOUKUTU || scene_type == SCENE_PAUSE_MENU_ITEMS || scene_type == SCENE_PAUSE_MENU_MAP
	                  || scene_type == SCENE_PAUSE_MENU_CONTROLS);
	bool is_in_load_screen = scene_type == SCENE_FLOOR && started_creating_map;
	bool gz_menu_open      = p2gz->menu->is_open();

	if (is_paused || is_in_load_screen || gz_menu_open) {
		draw_cur_seed();
	}

	if (entering_next_sublevel) {
		draw_reset_controls();
	}
}

const Segment* SegmentHistory::cur_segment()
{
	if (segments.len() > 0) {
		return segments.peek();
	}
	return nullptr;
}

void SegmentHistory::update()
{
	if (entering_next_sublevel) {
		const Segment* current_segment = cur_segment();
		GZASSERTLINE(current_segment);
		WarpDestination current_dest = current_segment->dest;

		const u32 btn = p2gz->controller->getButtonDown();

		// Retry same sublevel, random seed
		if (btn & Controller::PRESS_X) {
			current_dest.use_set_seed = false;

			p2gz->warp->set_dest(current_dest);
			p2gz->warp->set_preset(current_segment->preset, PS_Generated);
			p2gz->warp->do_warp();

			entering_next_sublevel = false;
			return;
		}

		// Retry same sublevel, same seed
		if (btn & Controller::PRESS_Y) {
			current_dest.use_set_seed = true;

			p2gz->warp->set_dest(current_dest);
			p2gz->warp->set_preset(current_segment->preset, PS_Generated);
			p2gz->warp->do_warp();

			entering_next_sublevel = false;
			return;
		}

		// Restart cave from the beginning
		if (btn & Controller::PRESS_L) {
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
			floor0_dest.use_set_seed    = false;
			if (floor0_dest.sublevel != 0) {
				floor0_dest.sublevel = 0;
				// If we don't find history for floor 0 in this cave, get the recommended preset for it.
				// TODO: currently assumes the PoD preset. Adjust to reflect AT in the future
				PresetCategory cat = PoD;
				if (floor0_segment->preset && floor0_segment->preset->category != Generated) {
					cat = floor0_segment->preset->category;
				}
				p2gz->warp->set_preset(p2gz->preset_mgr->suggested_preset(floor0_dest, cat), PS_Suggested);
			} else {
				p2gz->warp->set_preset(floor0_segment->preset, PS_Generated);
			}
			p2gz->warp->set_dest(floor0_dest);
			p2gz->warp->do_warp();

			entering_next_sublevel = false;
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
	const u32 seed = seg->dest.seed;

	J2DPrint j2d = init_j2d();

	// coordinates determined experimentally
	j2d.print(208.0f, 440.0f, "0x%08X", seed);
}

void draw_ctrl(J2DPrint& j2d, const f32 glyph_size, f32 x, f32& z, const char* img_name, const char* msg)
{
	x += p2gz->images->draw(img_name, 16.0f, z - p2gz->images->height() / 2.0f);
	x += p2gz->images->spacing();

	j2d.initiate();
	j2d.print(x, z + (glyph_size / 2.0f) - 4.0f, msg);

	z += p2gz->images->height() + 4.0f;
}

void SegmentHistory::draw_reset_controls()
{
	const f32 glyph_size = 18.0f;
	J2DPrint j2d         = init_j2d(glyph_size);

	f32 x = 16.0f;
	f32 z = 180.0f;

	draw_ctrl(j2d, glyph_size, x, z, "x_btn", "replay sublevel");
	draw_ctrl(j2d, glyph_size, x, z, "y_btn", "replay seed");
	draw_ctrl(j2d, glyph_size, x, z, "l_btn", "restart cave");
}

void SegmentHistory::start_segment(u32 seed)
{
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	Segment* segment     = new Segment();
	WarpDestination dest = Warp::current_dest();
	dest.seed            = seed;
	dest.use_set_seed    = true;
	segment->dest        = dest;
	segment->preset      = nullptr; // pikis are not alive when this is run. it will be set later

	segments.push(const_cast<const Segment*>(segment));

	prev_heap->becomeCurrentHeap();
}
