#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/p2gz.h>
#include <p2gz/SegmentHistory.h>
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

void SegmentHistory::update()
{
	if (entering_next_sublevel) {
		Segment* segment = cur_segment();
		GZASSERTLINE(segment);

		const u32 btn = p2gz->controller->getButtonDown();
		// Retry same sublevel, random seed
		if (btn & Controller::PRESS_X) {
			p2gz->warp->set_from_current();
			p2gz->warp->set_preset(nullptr);
			Preset::apply_squad(segment->squad, segment->onion_pikis);
			p2gz->warp->do_warp();

			entering_next_sublevel = false;
			return;
		}

		// Retry same sublevel, same seed
		if (btn & Controller::PRESS_Y) {
			p2gz->warp->set_from_current();
			p2gz->warp->set_seed(segment->seed);
			p2gz->warp->set_preset(nullptr);
			Preset::apply_squad(segment->squad, segment->onion_pikis);
			p2gz->warp->do_warp();

			entering_next_sublevel = false;
			return;
		}

		// Restart cave from the beginning
		if (btn & Controller::PRESS_L) {
			if (segments.len() == 0) {
				return;
			}

			// Find segment for the first floor of the cave, or earliest otherwise
			Segment* start_segment = nullptr;
			for (size_t i = segments.len() - 1; i > 0; i--) {
				Segment* this_segment = segments[i];
				if (this_segment->cave == segment->cave) {
					start_segment = this_segment;
					if (start_segment->sublevel == 0) {
						break;
					}
				} else {
					break;
				}
			}

			if (start_segment) {
				p2gz->warp->set_warp_area(start_segment->area);
				p2gz->warp->set_warp_cave(start_segment->cave);
				p2gz->warp->set_warp_day(start_segment->day);
				if (start_segment->sublevel == 0) {
					p2gz->warp->set_warp_sublevel(start_segment->sublevel + 1);
				} else {
					p2gz->warp->set_warp_sublevel(1);
				}
				p2gz->warp->set_preset(nullptr);
				Preset::apply_squad(segment->squad, segment->onion_pikis);
				p2gz->warp->do_warp();

				entering_next_sublevel = false;
				return;
			}
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
	Segment* seg = cur_segment();
	if (!seg) {
		return;
	}
	const u32 seed = seg->seed;

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
	// Make sure the segment list doesn't grow too large
	while (segments.len() >= 32) {
		Segment* segment = segments.removeAt(0);
		delete segment;
	}

	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	ID32 cave_id(game->getCaveID());
	Game::PikiContainer squad;
	Game::pikiMgr->saveAllPikmins(squad);

	Segment* segment = new Segment();
	segment->seed    = seed;

	segment->squad       = squad;
	segment->onion_pikis = Game::playData->mPikiContainer;
	segment->area        = game->mCurrentCourseInfo->mCourseIndex;
	segment->cave        = game->mCurrentCourseInfo->getCaveIndex_FromID(cave_id) + 1;
	segment->sublevel    = game->mCurrentFloor;
	segment->day         = Game::gameSystem->mTimeMgr->mDayCount;

	segments.push(segment);
}
