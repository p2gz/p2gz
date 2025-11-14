#include <p2gz/Trainers.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <Game/generalEnemyMgr.h>
#include <Game/Entities/Queen.h>

using namespace gz;

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

void EmpressTrainer::draw()
{
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

	J2DPrint j2d    = init_j2d();
	int flick_count = 0;
	int x           = 208.0f;

	bool success = empress->mFlickTimer >= 256 && empress->mFlickTimer <= 305;
	x += j2d.print(x, 440.0f, "flick count: ");

	JUtility::TColor color = JUtility::TColor(success ? 0 : 255, success ? 255 : 0, 0, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	j2d.print(x, 440.0f, "%d", (int)empress->mFlickTimer);
}
