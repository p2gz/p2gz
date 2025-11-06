#include <p2gz/DrawHelpers.h>
#include <Graphics.h>
#include <Game/MapMgr.h>
#include <System.h>

using namespace gz;

f32 gz::get_min_Y_clamped(Vector3f& pos, Vector3f& ref_pos, f32 max_height_diff, f32 offset_above_floor)
{
	// clamps pos's height toward the floor, but vertically no further than max_height_diff away from ref_pos
	f32 minY = Game::mapMgr->getMinY(pos);
	if (fabs(ref_pos.y - minY) > max_height_diff) {
		minY = (ref_pos.y > minY) ? ref_pos.y - max_height_diff : ref_pos.y + max_height_diff;
	}

	// offset up a bit so we're not clipped into the floor
	return minY + offset_above_floor;
}

void gz::draw_circle(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor)
{
	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);

	Vector3f vertices[3];
	vertices[0]   = position;
	f32 center_y  = Game::mapMgr->getMinY(position);
	vertices[0].y = offset_above_floor + center_y;

	for (int i = 0; i < 32; i++) {
		f32 theta     = -HALF_PI - (TAU * i / 32);
		vertices[1]   = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + position;
		vertices[1].y = offset_above_floor + center_y;

		f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
		vertices[2]   = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + position;
		vertices[2].y = offset_above_floor + center_y;

		GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
		for (int j = 0; j < 3; j++) {
			GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
	}
}

void gz::draw_wrapped_circle(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor, f32 max_height)
{
	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);

	Vector3f vertices[3];
	vertices[0] = position;
	// clamp toward floor, but no further than MAX_DRAW_CIRCLE_HEIGHT away from circle center
	// (also add an offset so we're not *in* the floor)
	vertices[0].y = get_min_Y_clamped(vertices[0], position, max_height, offset_above_floor);

	for (int i = 0; i < 32; i++) {
		f32 theta     = -HALF_PI - (TAU * i / 32);
		vertices[1]   = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + position;
		vertices[1].y = get_min_Y_clamped(vertices[1], position, max_height, offset_above_floor);

		f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
		vertices[2]   = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + position;
		vertices[2].y = get_min_Y_clamped(vertices[2], position, max_height, offset_above_floor);

		GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
		for (int j = 0; j < 3; j++) {
			GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
	}
}

void gz::draw_ring(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor)
{
	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);
	GXSetLineWidth(10, GX_TO_ZERO);
	gfx->mDrawColor = color;

	for (int i = 0; i < 32; i++) {
		f32 theta          = -HALF_PI - (TAU * i / 32);
		Vector3f start_pos = Vector3f(radius * sinf(theta), position.y + offset_above_floor, radius * cosf(theta)) + position;

		f32 nextTheta    = -HALF_PI - (TAU * (i + 1) / 32);
		Vector3f end_pos = Vector3f(radius * sinf(nextTheta), position.y + offset_above_floor, radius * cosf(nextTheta)) + position;

		gfx->drawLine(start_pos, end_pos);
	}
}
