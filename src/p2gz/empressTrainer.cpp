#include <p2gz/Trainers.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <Game/generalEnemyMgr.h>
#include <Game/Entities/Queen.h>
#include <p2gz/SegmentHistory.h>
#include <p2gz/p2gz.h>

using namespace gz;

const JUtility::TColor WHITE = JUtility::TColor(255, 255, 255, 255);
const JUtility::TColor RED   = JUtility::TColor(255, 0, 0, 255);
const JUtility::TColor GREEN = JUtility::TColor(0, 255, 0, 255);

Game::Queen::Obj* EmpressTrainer::get_empress()
{
	if (!Game::generalEnemyMgr) {
		return nullptr;
	}

	Game::Queen::Mgr* queenMgr = static_cast<Game::Queen::Mgr*>(Game::generalEnemyMgr->getEnemyMgr(Game::EnemyTypeID::EnemyID_Queen));
	if (!queenMgr) {
		return nullptr;
	}

	return static_cast<Game::Queen::Obj*>(queenMgr->getEnemy(0));
}

void EmpressTrainer::start()
{
	enabled = true;

	WarpDestination dest;
	dest.area         = 1;
	dest.cave         = 1;
	dest.sublevel     = 4;
	dest.use_set_seed = false;
	Preset* preset    = p2gz->preset_mgr->find("HoB5", PoD);

	p2gz->warp->set_dest(dest);
	p2gz->warp->set_preset(preset, PS_Chosen);
	p2gz->warp->do_warp();
}

void EmpressTrainer::draw()
{
	if (!enabled || first_damage_frame == -1) {
		return;
	}

	if (!Game::naviMgr || !Game::naviMgr->mArray || !Game::naviMgr->getActiveNavi()) {
		return;
	}

	Game::Queen::Obj* empress = get_empress();
	if (!empress) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();

	j2d.mGlyphWidth  = 24.0f;
	j2d.mGlyphHeight = 24.0f;

	j2d.mCharColor.set(WHITE);
	j2d.mGradientColor.set(WHITE);

	int x = 160.0f;
	j2d.print(x, 416.0f, "first damage frame: %d", first_damage_frame);

	x += j2d.print(x, 440.0f, "flick count: ");
	bool success = last_flick_count >= 256 && last_flick_count <= 305;
	j2d.mCharColor.set(success ? GREEN : RED);
	j2d.mGradientColor.set(success ? GREEN : RED);
	x += j2d.print(x, 440.0f, "%d", last_flick_count);

	if (empress->getStateID() != Game::Queen::QUEEN_Sleep && Game::naviMgr->getActiveNavi()) {
		if (!success) {
			x += j2d.print(x, 440.0f, " (");
			if (last_flick_count < 256) {
				x += j2d.print(x, 440.0f, "-");
				x += j2d.print(x, 440.0f, "%d", 256 - last_flick_count);
			} else {
				x += j2d.print(x, 440.0f, "+");
				x += j2d.print(x, 440.0f, "%d", last_flick_count - 305);
			}
			x += j2d.print(x, 440.0f, ")");
		}
	}
}

void EmpressTrainer::update()
{
	if (!enabled) {
		return;
	}

	Game::Queen::Obj* empress = get_empress();
	if (!empress) {
		return;
	}

	if (empress->getStateID() == Game::Queen::QUEEN_Sleep) {
		last_flick_count = (int)empress->mFlickTimer;
		if (last_flick_count > 0 && first_damage_frame == -1) {
			first_damage_frame = empress->mAnimator->getAnimator(0).mTimer - 59;
		}
	}

	if (empress->getStateID() != Game::Queen::QUEEN_Sleep && Game::naviMgr->getActiveNavi()) {
		fade_out_frames++;
	}

	if (fade_out_frames == 30) {
		Game::gameSystem->startFadeout(1.0f);
	}

	if (fade_out_frames == 60) {
		fade_out_frames    = 0;
		first_damage_frame = -1;

		const Segment* current_segment = p2gz->segment_history->cur_segment();
		GZASSERTLINE(current_segment);

		WarpDestination current_dest = current_segment->dest;
		current_dest.use_set_seed    = false;
		p2gz->warp->set_dest(current_dest);
		p2gz->warp->set_preset(current_segment->preset, PS_Generated);
		p2gz->warp->do_warp();
	}
}
