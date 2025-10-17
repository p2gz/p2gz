#include <p2gz/EnemyDebugInfo.h>
#include <Game/EnemyMgrBase.h>
#include <Game/Navi.h>
#include <Graphics.h>
#include <Game/generalEnemyMgr.h>
#include <Game/EnemyParmsBase.h>
#include <Dolphin/os.h>
#include <P2JME/P2JME.h>

using namespace gz;

EnemyDebugInfo::EnemyDebugInfo()
{
	enabled     = false;
	max_dist    = 512.0f;
	line_height = 22;
	text_scale  = 0.5f;

	cur_color = 0;
	text_colors.push(Color4(130, 130, 255, 200));
	text_colors.push(Color4(255, 60, 255, 200));
	text_colors.push(Color4(90, 255, 130, 200));
	text_colors.push(Color4(90, 230, 230, 200));

	draw_enemy_name  = true;
	draw_cur_state   = true;
	draw_flick_count = true;
	draw_position    = true;
}

void EnemyDebugInfo::set_size(s32 size)
{
	text_scale  = 0.05f * (f32)size;
	line_height = 12 + size;
}

void EnemyDebugInfo::remove_enemy(Game::EnemyBase* enemy)
{
	int idx = enemies.find(enemy);
	if (idx != -1) {
		enemies.removeAt(idx);
	}
}

void EnemyDebugInfo::draw()
{
	if (!enabled) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	cur_color = 0;
	for (size_t i = 0; i < enemies.len(); i++) {
		Game::EnemyBase* enemy = enemies[i];
		draw_enemy_dbg(enemy, gfx);
	}
}

void EnemyDebugInfo::draw_enemy_dbg(Game::EnemyBase* enemy, Graphics* gfx)
{
	if (!enemy || !enemy->mCurrentLifecycleState) {
		return;
	}

	const char* lifecycle = enemy->mCurrentLifecycleState->mName;
	if (strcmp(lifecycle, "dead") == 0 || strcmp(lifecycle, "appear") == 0 || strcmp(lifecycle, "stay") == 0
	    || strcmp(lifecycle, "wait_big") == 0) {
		return;
	}

	Vector3f naviPos  = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f enemyPos = enemy->getPosition();
	if (sqrDistanceXZ(naviPos, enemyPos) > SQUARE(max_dist)) {
		return;
	}

	Color4 color = text_colors[cur_color];
	cur_color += 1;
	if (cur_color >= text_colors.len()) {
		cur_color = 0;
	}

	PerspPrintfInfo info;
	info.mFont   = gP2JMEMgr->mFont;
	info.mScale  = text_scale;
	info.mColorA = color;
	info.mColorB = color;
	Vector3f pos = enemyPos + Vector3f(0, static_cast<Game::EnemyParmsBase*>(enemy->mParms)->mGeneral.mLifeMeterHeight, 0);

	// Debug prints
	if (draw_enemy_name) {
		gfx->perspPrintf(info, pos, "%s", enemy->getCreatureName());
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_position) {
		gfx->perspPrintf(info, pos, "(%.2f, %.2f, %.2f)", enemy->mPosition.x, enemy->mPosition.y, enemy->mPosition.z);
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_cur_state) {
		gfx->perspPrintf(info, pos, "state: %s", enemy->mCurrentLifecycleState->mName);
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_flick_count) {
		gfx->perspPrintf(info, pos, "flick: %d", (int)enemy->mFlickTimer);
		info.mPerspectiveOffsetY += line_height;
	}
}
