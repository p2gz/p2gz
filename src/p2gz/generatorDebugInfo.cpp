#include <p2gz/GeneratorDebugInfo.h>
#include <Game/Navi.h>
#include <Game/enemyInfo.h>
#include <P2JME/P2JME.h>

using namespace gz;

GeneratorDebugInfo::GeneratorDebugInfo()
{
	enabled  = false;
	max_dist = 512.0f;
}

void GeneratorDebugInfo::draw()
{
	if (!enabled || !Game::generatorCache) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	FOREACH_NODE(Game::Generator, Game::generatorCache->getFirstGenerator(), gen)
	{
		draw_gen_dbg(gen, gfx);
	}
}

void GeneratorDebugInfo::draw_gen_dbg(Game::Generator* gen, Graphics* gfx)
{
	Vector3f naviPos = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f genPos  = gen->mPosition;
	if (sqrDistanceXZ(naviPos, genPos) > SQUARE(max_dist)) {
		return;
	}

	PerspPrintfInfo info;
	info.mFont   = gP2JMEMgr->mFont;
	info.mScale  = 0.5f;
	info.mColorA = Color4(255, 255, 255, 200);
	info.mColorB = Color4(255, 255, 255, 200);
	Vector3f pos = genPos + Vector3f(0.0f, 30.0f, 0.0f);

	if (gen->mObject->mTypeID == 'teki') {
		Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
		const char* gen_name                = Game::EnemyInfoFunc::getEnemyName(gen_obj_enemy->mEnemyID, 0xFFFF);
		gfx->perspPrintf(info, pos, "%s (gen)", gen_name);
	} else if (gen->mObject->mTypeID == 'pelt') {
		Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
		int treasure_id             = gen_pellet->mGenParm->mIndex;
		int kind                    = gen_pellet->mPelType;
		const char* treasure_name   = Game::PelletList::Mgr::mInstance->getConfig(kind)->getPelletConfig(treasure_id)->mParams.mName.mData;
		gfx->perspPrintf(info, pos, "%d:%s (gen)", treasure_id, treasure_name);
	}

	info.mPerspectiveOffsetY += 22;
	gfx->perspPrintf(info, pos, "(%.2f, %.2f, %.2f)", gen->mPosition.x, gen->mPosition.y, gen->mPosition.z);
}
