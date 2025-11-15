#include <p2gz/Trainers.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <Game/generalEnemyMgr.h>
#include <Game/Entities/Queen.h>
#include <p2gz/SegmentHistory.h>
#include <p2gz/p2gz.h>

using namespace gz;

void EmpressTrainer::draw()
{
	if (!Game::naviMgr) {
		return;
	}

	if (!Game::naviMgr->mArray) {
		return;
	}

	if (!Game::naviMgr->getActiveNavi()) {
		return;
	}

	if (!Game::generalEnemyMgr) {
		return;
	}

	Game::Queen::Mgr* queenMgr = static_cast<Game::Queen::Mgr*>(Game::generalEnemyMgr->getEnemyMgr(Game::EnemyTypeID::EnemyID_Queen));
	if (!queenMgr) {
		return;
	}

	Game::Queen::Obj* empress = static_cast<Game::Queen::Obj*>(queenMgr->getEnemy(0));
	if (!empress) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();

	j2d.mGlyphWidth  = 24.0f;
	j2d.mGlyphHeight = 24.0f;

	JUtility::TColor color = JUtility::TColor(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	int flick_count = 0;

	bool success = last_flick_count >= 256 && last_flick_count <= 305;

	int x1 = 208.0f;
	x1 += j2d.print(x1, 440.0f, "flick count: ");

	color = JUtility::TColor(success ? 0 : 255, success ? 255 : 0, 0, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	if (empress->getStateID() != Game::Queen::QUEEN_Sleep && Game::naviMgr->getActiveNavi()) {
		x1 += j2d.print(x1, 440.0f, "%d", last_flick_count);

		if (!success) {
			color = JUtility::TColor(255, 255, 255, 255);
			j2d.mCharColor.set(color);
			j2d.mGradientColor.set(color);
			x1 += j2d.print(x1, 440.0f, " (");
			if (last_flick_count < 256) {
				x1 += j2d.print(x1, 440.0f, "-");
				x1 += j2d.print(x1, 440.0f, "%d", 256 - last_flick_count);
			} else {
				x1 += j2d.print(x1, 440.0f, "+");
				x1 += j2d.print(x1, 440.0f, "%d", last_flick_count - 305);
			}
			x1 += j2d.print(x1, 440.0f, ")");
		}
		if (fade_out_frames == 30) {
			Game::gameSystem->startFadeout(1.0f);
		}
		if (fade_out_frames == 60) {
			fade_out_frames   = 0;
			first_flick_frame = -1;

			const Segment* current_segment = p2gz->segment_history->cur_segment();
			GZASSERTLINE(current_segment);

			WarpDestination current_dest = current_segment->dest;
			current_dest.use_set_seed    = false;
			p2gz->warp->set_dest(current_dest);
			p2gz->warp->set_preset(current_segment->preset, PS_Generated);
			p2gz->warp->do_warp();
			return;
		}

		fade_out_frames++;
		return;
	}

	j2d.print(x1, 440.0f, "%d", (int)empress->mFlickTimer);
	last_flick_count = (int)empress->mFlickTimer;

	if (last_flick_count > 0 && first_flick_frame == -1) {
		first_flick_frame = empress->mAnimator->getAnimator(0).mTimer - 59;
	}

	color = JUtility::TColor(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	int x2 = 160.0f;
	x2 += j2d.print(160.0f, 416.0f, "first damage frame: ");
	if (first_flick_frame != -1) {
		j2d.print(x2, 416.0f, "%d", first_flick_frame);
	}
}
