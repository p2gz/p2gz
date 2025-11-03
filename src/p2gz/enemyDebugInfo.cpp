#include <p2gz/EnemyDebugInfo.h>
#include <Game/EnemyMgrBase.h>
#include <Game/Navi.h>
#include <Graphics.h>
#include <Game/generalEnemyMgr.h>
#include <Game/EnemyParmsBase.h>
#include <Dolphin/os.h>
#include <P2JME/P2JME.h>
#include <Game/MoviePlayer.h>
#include <Game/generalEnemyMgr.h>

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
	draw_position    = false;
}

void EnemyDebugInfo::set_size(s32 size)
{
	text_scale  = 0.05f * (f32)size;
	line_height = 12 + size;
}

void EnemyDebugInfo::draw()
{
	if (!enabled) {
		return;
	}

	if (!Game::generalEnemyMgr) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	cur_color = 0;
	GeneralMgrIterator<Game::EnemyBase> iEnemyMgr(Game::generalEnemyMgr);
	CI_LOOP(iEnemyMgr)
	{
		Game::EnemyBase* enemy = iEnemyMgr.getObject();
		if (enemy) {
			draw_enemy_dbg(enemy, gfx);
		}
	}
}

void EnemyDebugInfo::draw_enemy_dbg(Game::EnemyBase* enemy, Graphics* gfx)
{
	if (!enemy || !enemy->mCurrentLifecycleState) {
		return;
	}

	if (enemy->mHealth <= 0.0f || !enemy->mLod.isFlag(AILOD_IsVisible)) {
		return;
	}

	Vector3f naviPos  = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f enemyPos = enemy->mPosition;
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

	// manually adjust offsets for certain enemies with weird lifegauge offsets
	// may need to add to these later as a QoL update, especially bosses
	switch (enemy->getEnemyTypeID()) {
	case Game::EnemyTypeID::EnemyID_Chappy:
	case Game::EnemyTypeID::EnemyID_BlueChappy:
	case Game::EnemyTypeID::EnemyID_YellowChappy:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_RedPom:
	case Game::EnemyTypeID::EnemyID_BluePom:
	case Game::EnemyTypeID::EnemyID_YellowPom:
	case Game::EnemyTypeID::EnemyID_WhitePom:
	case Game::EnemyTypeID::EnemyID_BlackPom:
	case Game::EnemyTypeID::EnemyID_RandPom:
		pos.y += 50.0f;
		break;

	case Game::EnemyTypeID::EnemyID_KumaChappy:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Armor:
		pos.y += 20.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Qurione:
		pos.y += 60.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Kabuto:
	case Game::EnemyTypeID::EnemyID_Rkabuto:
		pos.y += 45.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Fkabuto:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Tank:
	case Game::EnemyTypeID::EnemyID_Wtank:
		pos.y += 10.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Mar:
	case Game::EnemyTypeID::EnemyID_Hanachirashi:
		pos.y += 40.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Queen:
		pos.y += 160.0f;
		break;

	case Game::EnemyTypeID::EnemyID_SnakeCrow:
		pos.y += 100.0f;
		break;

	case Game::EnemyTypeID::EnemyID_SnakeWhole:
		pos.y += 160.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Bomb:
		pos.y += 15.0f;
		break;

	case Game::EnemyTypeID::EnemyID_PanModoki:
		pos.y += 20.0f;
		break;

	case Game::EnemyTypeID::EnemyID_OoPanModoki:
		pos.y += 60.0f;
		break;

	case Game::EnemyTypeID::EnemyID_KingChappy:
		pos.y += 100.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Miulin:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Damagumo:
		pos.y += 140.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Houdai:
		pos.y += 80.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Jigumo:
		pos.y += 50.0f;
		break;

	case Game::EnemyTypeID::EnemyID_BigFoot:
		pos.y += 160.0f;
		break;

	case Game::EnemyTypeID::EnemyID_BigTreasure:
		pos.y += 130.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Kurage:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_OniKurage:
		pos.y += 100.0f;
		break;

	case Game::EnemyTypeID::EnemyID_UmiMushiBlind:
		pos.y -= 20.0f;
		break;

	case Game::EnemyTypeID::EnemyID_BombSarai:
		pos.y += 70.0f;
		break;

	case Game::EnemyTypeID::EnemyID_MiniHoudai:
	case Game::EnemyTypeID::EnemyID_FminiHoudai:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Stone:
		pos.y += 90.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Rock:
		pos.y += 30.0f;
		break;

	case Game::EnemyTypeID::EnemyID_BlackMan:
		pos.y += 80.0f;
		break;

	case Game::EnemyTypeID::EnemyID_Tyre:
		pos.y += 20.0f;
		break;

	case Game::EnemyTypeID::EnemyID_DangoMushi:
		pos.y += 100.0f;
		break;
	}

	// Debug prints
	if (draw_enemy_name) {
		gfx->perspPrintf(info, pos, "%s", enemy->getCreatureName());
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_position) {
		gfx->perspPrintf(info, pos, "(%.2f, %.2f, %.2f)", enemy->mPosition.x, enemy->mPosition.y, enemy->mPosition.z);
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_cur_state && enemy->mCurrentLifecycleState->mName) {
		gfx->perspPrintf(info, pos, "state: %s", enemy->mCurrentLifecycleState->mName);
		info.mPerspectiveOffsetY += line_height;
	}
	if (draw_flick_count) {
		gfx->perspPrintf(info, pos, "flick: %d", (int)enemy->mFlickTimer);
		info.mPerspectiveOffsetY += line_height;
	}
}
