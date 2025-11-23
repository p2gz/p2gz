#include <p2gz/CaveDebugInfo.h>
#include <p2gz/DrawHelpers.h>
#include <Game/Cave/RandMapMgr.h>

using namespace gz;

CaveDebugInfo::CaveDebugInfo()
{
	draw_spawn_points = false;
}

void CaveDebugInfo::draw()
{
	if (draw_spawn_points) {
		do_draw_spawn_points();
	}
}

void CaveDebugInfo::do_draw_spawn_points()
{
	GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);

	// "placed" nodes are ones that we haven't been to yet
	FOREACH_NODE(Game::Cave::MapNode, Game::Cave::randMapMgr->mGenerator->mPlacedMapNodes->mChild, node)
	{
		Game::Cave::BaseGen* spawn_points = node->mUnitInfo->getBaseGen();
		if (!spawn_points) {
			continue;
		}

		FOREACH_NODE(Game::Cave::BaseGen, spawn_points->mChild, spawn)
		{
			draw_spawn_point(node, spawn);
		}
	}

	// when we've visited a map node, it gets moved to the visited list
	FOREACH_NODE(Game::Cave::MapNode, Game::Cave::randMapMgr->mGenerator->mVisitedMapNodes->mChild, node)
	{
		Game::Cave::BaseGen* spawn_points = node->mUnitInfo->getBaseGen();
		if (!spawn_points) {
			continue;
		}

		FOREACH_NODE(Game::Cave::BaseGen, spawn_points->mChild, spawn)
		{
			draw_spawn_point(node, spawn);
		}
	}
}

void CaveDebugInfo::draw_spawn_point(Game::Cave::MapNode* node, Game::Cave::BaseGen* spawn)
{
	Vector3f pos                          = node->getBaseGenGlobalPosition(spawn);
	Game::Cave::BaseGen::CaveGenType kind = spawn->mSpawnType;
	f32 radius                            = 16.0f;
	Color4 color(255, 255, 255, 255);
	f32 vertical_offset = 1.0f;

	switch (kind) {
	case Game::Cave::BaseGen::CGT_EnemyEasy:
		radius = spawn->mRadius;
		color  = Color4(250, 87, 207, 255);
		break;
	case Game::Cave::BaseGen::CGT_EnemyHard:
		color           = Color4(201, 2, 52, 255);
		vertical_offset = 2.5f;
		break;
	case Game::Cave::BaseGen::CGT_TreasureItem:
		radius          = 24.0f;
		color           = Color4(230, 115, 0, 255);
		vertical_offset = 3.5f;
		break;
	case Game::Cave::BaseGen::CGT_HoleOrGeyser:
	case Game::Cave::BaseGen::CGT_DoorSeam:
		radius = 12.0f;
		color  = Color4(130, 130, 130, 255);
		break;
	case Game::Cave::BaseGen::CGT_Plant:
		radius          = 12.0f;
		color           = Color4(59, 148, 90, 255);
		vertical_offset = 2.0f;
		break;
	case Game::Cave::BaseGen::CGT_Start:
		radius          = 12.0f;
		color           = Color4(230, 50, 86, 255);
		vertical_offset = 1.5f;
		break;
	case Game::Cave::BaseGen::CGT_EnemySpecial:
		radius          = 18.0f;
		color           = Color4(89, 6, 138, 255);
		vertical_offset = 3.0f;
		break;
	}

	if (kind == Game::Cave::BaseGen::CGT_EnemyEasy) {
		draw_ring(pos, radius, color, vertical_offset);
		draw_ring(pos, radius - 5.0f, color, vertical_offset);
		draw_ring(pos, radius - 10.0f, color, vertical_offset);
	} else {
		draw_circle(pos, radius, color, vertical_offset);
	}
}
