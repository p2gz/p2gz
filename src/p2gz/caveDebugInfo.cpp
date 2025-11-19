#include <p2gz/CaveDebugInfo.h>
#include <p2gz/DrawHelpers.h>
#include <Game/Cave/RandMapMgr.h>

using namespace gz;

CaveDebugInfo::CaveDebugInfo()
    : spawn_points(128)
{
	draw_spawn_points = false;
}

void CaveDebugInfo::register_spawn_points()
{
	clear();

	Game::Cave::MapNode* placed_nodes = Game::Cave::randMapMgr->mGenerator->getPlacedNodes();
	FOREACH_NODE(Game::Cave::MapNode, placed_nodes->mChild, node)
	{
		Game::Cave::BaseGen* spawn_point = node->mUnitInfo->getBaseGen();
		if (!spawn_point) {
			continue;
		}

		FOREACH_NODE(Game::Cave::BaseGen, spawn_point->mChild, spawn)
		{
			Vector3f pos                          = node->getBaseGenGlobalPosition(spawn);
			Game::Cave::BaseGen::CaveGenType kind = spawn->mSpawnType;
			f32 radius;

			switch (kind) {
			case Game::Cave::BaseGen::CGT_EnemyEasy:
				radius = spawn->mRadius;
				break;

			case Game::Cave::BaseGen::CGT_TreasureItem:
				radius = 24.0f;
				break;

			case Game::Cave::BaseGen::CGT_HoleOrGeyser:
			case Game::Cave::BaseGen::CGT_DoorSeam:
			case Game::Cave::BaseGen::CGT_Plant:
			case Game::Cave::BaseGen::CGT_Start:
				radius = 12.0f;
				break;

			case Game::Cave::BaseGen::CGT_EnemySpecial:
				radius = 18.0f;
				break;
			default:
				radius = 16.0f;
			}

			spawn_points.push(SpawnPoint(pos, radius, kind));
		}
	}
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

	for (size_t i = 0; i < spawn_points.len(); i++) {
		Vector3f position = spawn_points[i].pos;
		const f32 radius  = spawn_points[i].radius;
		Color4 color(255, 255, 255, 255);
		f32 vertical_offset = 1.0f;

		// Same colors as Caveripper
		switch (spawn_points[i].kind) {
		case Game::Cave::BaseGen::CGT_EnemyEasy:
			color = Color4(250, 87, 207, 255);
			break;
		case Game::Cave::BaseGen::CGT_EnemyHard:
			color           = Color4(201, 2, 52, 255);
			vertical_offset = 2.5f;
			break;
		case Game::Cave::BaseGen::CGT_TreasureItem:
			color           = Color4(230, 115, 0, 255);
			vertical_offset = 3.5f;
			break;
		case Game::Cave::BaseGen::CGT_HoleOrGeyser:
		case Game::Cave::BaseGen::CGT_DoorSeam:
			color = Color4(130, 130, 130, 255);
			break;
		case Game::Cave::BaseGen::CGT_Plant:
			color           = Color4(59, 148, 90, 255);
			vertical_offset = 2.0f;
			break;
		case Game::Cave::BaseGen::CGT_Start:
			color           = Color4(230, 50, 86, 255);
			vertical_offset = 1.5f;
			break;
		case Game::Cave::BaseGen::CGT_EnemySpecial:
			color           = Color4(89, 6, 138, 255);
			vertical_offset = 3.0f;
			break;
		default:
			continue;
		}

		if (spawn_points[i].kind == Game::Cave::BaseGen::CGT_EnemyEasy) {
			draw_ring(position, radius, color, vertical_offset);
			draw_ring(position, radius - 5.0f, color, vertical_offset);
			draw_ring(position, radius - 10.0f, color, vertical_offset);
		} else {
			draw_circle(position, radius, color, vertical_offset);
		}
	}
}
