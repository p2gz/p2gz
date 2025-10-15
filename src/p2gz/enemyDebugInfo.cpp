#include <p2gz/EnemyDebugInfo.h>
#include <Game/EnemyMgrBase.h>
#include <Game/Navi.h>
#include <Graphics.h>

using namespace gz;

void init()
{
}

void update()
{
	if (!enabled) {
		return;
	}

	Game::EnemyMgrBase* mgr = generalEnemyMgr->getEnemyMgr(getEnemyTypeID());

	if (mgr != nullptr) {
		Graphics* gfx = sys->mGfx;
		gfx->initPerspPrintf(gfx->mCurrentViewport);

		for (int obj = 0; obj < mgr->mObjLimit; obj++) {
			Game::EnemyBase* enemy = mgr->getEnemy(obj);

			if (enemy->mCurrentLifecycleState == nullptr) {
				continue;
			}

			if (strcmp(enemy->mCurrentLifecycleState->mName, "dead") == 0 || strcmp(enemy->mCurrentLifecycleState->mName, "appear") == 0
			    || strcmp(enemy->mCurrentLifecycleState->mName, "stay") == 0
			    || strcmp(enemy->mCurrentLifecycleState->mName, "wait_big") == 0) {
				continue;
			}

			if (naviMgr->getActiveNavi() == nullptr) {
				continue;
			}

			Vector3f naviPos  = naviMgr->getActiveNavi()->getPosition();
			Vector3f enemyPos = enemy->getPosition();
			if (sqrDistanceXZ(naviPos, enemyPos) > SQUARE(512)) {
				continue;
			}

			PerspPrintfInfo info;
			info.mColorA  = Color4(255, 255, 255, 255);
			Vector3f pos1 = enemyPos + Vector3f(0, static_cast<EnemyParmsBase*>(enemy->mParms)->mGeneral.mLifeMeterHeight, 0);
			Vector3f pos2 = pos1 + Vector3f(0, 16, 0);
			Vector3f pos3 = pos2 + Vector3f(0, 16, 0);

			gfx->perspPrintf(info, pos1, "flick: %d", (int)enemy->mFlickTimer);
			gfx->perspPrintf(info, pos2, "%s", enemy->mCurrentLifecycleState->mName);
			gfx->perspPrintf(info, pos3, "(%d, %d)", (int)enemy->mPosition.x, (int)enemy->mPosition.z);
		}
	}

	// @P2GZ end
}
