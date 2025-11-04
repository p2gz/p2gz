#include <p2gz/CaveDebugInfo.h>
#include <Graphics.h>
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
	Graphics* gfx = sys->getGfx();
	GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
	gfx->mDrawColor = Color4(0, 0, 0, 255);

	if (draw_spawn_points) {
		for (size_t i = 0; i < spawn_points.len(); i++) {
			Vector3f position = spawn_points[i].pos;
			const f32 radius  = spawn_points[i].radius;
			Color4 color(255, 255, 255, 255);
			f32 vertical_offset = 0.0f;

			// Same colors as Caveripper
			switch (spawn_points[i].kind) {
			case Game::Cave::BaseGen::CGT_EnemyEasy:
				color = Color4(250, 87, 207, 255);
				break;
			case Game::Cave::BaseGen::CGT_EnemyHard:
				color           = Color4(201, 2, 52, 255);
				vertical_offset = 1.0f;
				break;
			case Game::Cave::BaseGen::CGT_TreasureItem:
				color           = Color4(230, 115, 0, 255);
				vertical_offset = 2.0f;
				break;
			case Game::Cave::BaseGen::CGT_HoleOrGeyser:
			case Game::Cave::BaseGen::CGT_DoorSeam:
				color           = Color4(130, 130, 130, 255);
				vertical_offset = -1.0f;
				break;
			case Game::Cave::BaseGen::CGT_Plant:
				color           = Color4(59, 148, 90, 255);
				vertical_offset = 0.5f;
				break;
			case Game::Cave::BaseGen::CGT_Start:
				color           = Color4(230, 50, 86, 255);
				vertical_offset = -1.0f;
				break;
			case Game::Cave::BaseGen::CGT_EnemySpecial:
				color           = Color4(89, 6, 138, 255);
				vertical_offset = 3.0f;
				break;
			default:
				continue;
			}

			draw_circle(position + Vector3f(0.0, vertical_offset, 0.0f), radius, color);
		}
	}
}

static const f32 CIRCLE_VERTICAL_OFFSET = 3.0f;
void CaveDebugInfo::draw_circle(Vector3f position, f32 radius, Color4 color)
{
	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);

	// Offset y position so the circle is not clipping into the ground.
	position.y += CIRCLE_VERTICAL_OFFSET;

	Vector3f vertices[3];
	vertices[0] = position;

	for (int i = 0; i < 32; i++) {
		f32 theta   = -HALF_PI - (TAU * i / 32);
		vertices[1] = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + position;

		f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
		vertices[2]   = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + position;

		GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
		for (int j = 0; j < 3; j++) {
			GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
	}
}
