#include <p2gz/DismissPositions.h>
#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <Game/PikiState.h>
#include <Game/MapMgr.h>

using namespace gz;

const f32 CIRCLE_VERTICAL_OFFSET = 5.0f;
const f32 MAX_DRAW_CIRCLE_HEIGHT = 20.0f;

const Color4 PIKMIN_COLORS[Game::PikiColorCount + 1]
    = { Color4(0, 50, 255, 255),    Color4(255, 30, 0, 255),  Color4(255, 210, 0, 255), Color4(28, 0, 52, 255),
	    Color4(255, 230, 255, 255), Color4(255, 140, 0, 255), Color4(255, 255, 255, 0) };

// TODO: Clean up copy-paste and make a reusable gz function for drawing circles.
void DismissPositions::draw_circle(Vector3f position, f32 radius, Color4 color)
{
	Graphics* gfx = sys->getGfx();
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	Vector3f vertices[3];
	vertices[0] = position;
	// Offset y position so the circle is not clipping into the ground and adjusts for slopes
	// Also if the difference in height is too great, clamp the height of the circle to prevent jank-looking circles
	f32 minY = Game::mapMgr->getMinY(vertices[0]);
	if (fabs(position.y - minY) > MAX_DRAW_CIRCLE_HEIGHT) {
		minY = position.y > minY ? position.y - MAX_DRAW_CIRCLE_HEIGHT : position.y + MAX_DRAW_CIRCLE_HEIGHT;
	}
	vertices[0].y = minY;
	vertices[0].y += CIRCLE_VERTICAL_OFFSET;

	for (int i = 0; i < 32; i++) {
		f32 theta   = -HALF_PI - (TAU * i / 32);
		vertices[1] = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + position;
		// Offset y position so the circle is not clipping into the ground and adjusts for slopes
		// Also if the difference in height is too great, clamp the height of the circle to prevent jank-looking circles
		minY = Game::mapMgr->getMinY(vertices[1]);
		if (fabs(position.y - minY) > MAX_DRAW_CIRCLE_HEIGHT) {
			minY = position.y > minY ? position.y - MAX_DRAW_CIRCLE_HEIGHT : position.y + MAX_DRAW_CIRCLE_HEIGHT;
		}
		vertices[1].y = minY;
		vertices[1].y += CIRCLE_VERTICAL_OFFSET;

		f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
		vertices[2]   = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + position;
		// Offset y position so the circle is not clipping into the ground and adjusts for slopes
		// Also if the difference in height is too great, clamp the height of the circle to prevent jank-looking circles
		minY = Game::mapMgr->getMinY(vertices[2]);
		if (fabs(position.y - minY) > MAX_DRAW_CIRCLE_HEIGHT) {
			minY = position.y > minY ? position.y - MAX_DRAW_CIRCLE_HEIGHT : position.y + MAX_DRAW_CIRCLE_HEIGHT;
		}
		vertices[2].y = minY;
		vertices[2].y += CIRCLE_VERTICAL_OFFSET;

		GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
		for (int j = 0; j < 3; j++) {
			GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
	}
}

// Draw circles on the ground indicating dismiss position and radius for each color.
void DismissPositions::draw()
{
	if (!enabled) {
		return;
	}

	if (!Game::naviMgr->getActiveNavi()) {
		return;
	}

	for (int i = 0; i < 6; i++) {
		if (positions[i] == Vector3f::zero) {
			continue;
		}

		draw_circle(positions[i], radii[i], PIKMIN_COLORS[i]);

		if (dismissed) {
			continue;
		}

		Graphics* gfx = sys->getGfx();
		GXSetLineWidth(10, GX_TO_ZERO);
		gfx->initPerspPrintf(gfx->mCurrentViewport);
		gfx->mDrawColor = PIKMIN_COLORS[i];

		Vector3f pos1 = Game::naviMgr->getActiveNavi()->getPosition();
		Vector3f pos2 = positions[i];
		// Adjust y-endpoint so the line can point to circles that aren't on the same xz plane as the player (be sure to clamp on offsets
		// that are too high!)
		f32 minY = Game::mapMgr->getMinY(pos2);
		if (fabs(pos2.y - minY) > MAX_DRAW_CIRCLE_HEIGHT) {
			minY = pos2.y > minY ? pos2.y - MAX_DRAW_CIRCLE_HEIGHT : pos2.y + MAX_DRAW_CIRCLE_HEIGHT;
		}
		pos2.y = minY;
		pos2.y += CIRCLE_VERTICAL_OFFSET;
		gfx->drawLine(pos1, pos2);
		gfx->mDrawColor = Color4(0, 0, 0, 255);
	}
}

// Adapted from Navi::releasePikis().
void DismissPositions::update()
{
	if (!Game::naviMgr || !Game::pikiMgr) {
		return;
	}

	if (!Game::naviMgr->mArray) {
		return;
	}

	if (!Game::naviMgr->getActiveNavi()) {
		return;
	}

	s32 pikis = 0;
	Game::Piki* buffer[MAX_PIKI_COUNT];
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if ((!piki->mCurrentState || piki->mCurrentState->releasable()) && piki->isAlive()
		    && piki->mNavi == Game::naviMgr->getActiveNavi()) {
			buffer[pikis++] = *iterator;
		}
	}

	if (pikis == 0) {
		dismissed = true;
		return;
	}
	dismissed = false;

	int number[8];
	for (int i = 0; i != 8; i++) {
		positions[i] = 0;
		number[i]    = 0;
	}
	for (int cColor = 0; cColor < 8; cColor++) {
		for (int i = 0; i < pikis; i++) {
			if (cColor != Game::Yellow) {
				if (cColor == buffer[i]->getKind()) {
					number[cColor]++;
					positions[cColor] += buffer[i]->getPosition();
				}
			} else if (buffer[i]->getKind() == Game::Yellow) {
				number[Game::Yellow]++;
				positions[Game::Yellow] += buffer[i]->getPosition();
			}
		}
	}

	for (int cColor = 0; cColor < 8; cColor++) {
		if (number[cColor] > 0) {
			f32 num  = number[cColor];
			f32 mean = 1.0f / number[cColor];
			positions[cColor] *= mean;
			radii[cColor] = pikmin2_sqrtf(num) * 6.25f;
		}
	}

	Game::Navi* otherNavi = Game::naviMgr->getAt(GET_OTHER_NAVI(Game::naviMgr->getActiveNavi()));
	if (!otherNavi) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		for (int cColor = 0; cColor < 8; cColor++) {
			if (number[cColor] > 0) {
				Vector3f naviPos = Game::naviMgr->getActiveNavi()->getPosition();
				Vector3f diff    = positions[cColor] - naviPos;
				f32 dist         = diff.qNormalise();
				dist             = dist - radii[cColor] - 25.0f;
				if (dist < 20.0f) {
					dist = 20.0f - dist;
					positions[cColor] += diff * dist;
				}
				if (otherNavi->isAlive()) {
					Vector3f naviPos = otherNavi->getPosition();
					Vector3f diff    = positions[cColor] - naviPos;
					f32 dist         = diff.qNormalise();
					dist             = dist - radii[cColor] - 25.0f;
					if (dist < 20.0f) {
						dist = 20.0f - dist;
						positions[cColor] += diff * dist;
					}
				}
			}

			for (int j = cColor + 1; j < 8; j++) {
				if (number[cColor] > 0 && number[j] > 0) {
					Vector3f diff = positions[cColor] - positions[j];
					f32 dist      = diff.qNormalise();
					dist          = dist - radii[cColor] - radii[j];
					if (dist < 20.0f) {
						dist = 20.0f - dist;
						positions[cColor] += diff * dist;
						positions[j] -= diff * dist;
					}
				}
			}
		}
	}
}
