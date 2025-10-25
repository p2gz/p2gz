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
		const u32 btn = p2gz->controller->getButtonDown();
		if (btn & Controller::PRESS_X) {
			p2gz->warp->set_from_current();
			p2gz->warp->do_warp();
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
	Segment* segment = new Segment();
	segment->seed    = seed;

	Game::PikiContainer squad;
	Game::pikiMgr->saveAllPikmins(squad);

	segment->squad       = squad;
	segment->onion_pikis = Game::playData->mPikiContainer;

	segments.push(segment);
}
