#include "Game/BaseGameSection.h"
#include "Game/GameSystem.h"
#include "Game/GameLight.h"

#include "Sys/DrawBuffers.h"

#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/J3D/J3DSys.h"

#include "ParticleMgr.h"
#include "SysTimers.h"
#include "IDelegate.h"
#include "System.h"
#include "Light.h"
#include "nans.h"

// @P2GZ
#include <p2gz/p2gz.h>

const char* message = "drct-post";

namespace Game {
/**
 * @note Address: 0x802398D8
 * @note Size: 0x1F4
 * Matches
 */
void BaseGameSection::newdraw_draw3D_all(Graphics& gfx)
{
	// Setup viewport callback to be newdraw_drawAll
	Delegate1<BaseGameSection, Viewport*> vpDelegate(this, &BaseGameSection::newdraw_drawAll);
	gfx.mapViewport(&vpDelegate);

	// Initialise both draw buffers for the frame
	mOpaqueDrawBuffer->frameInitAll();
	mTransparentDrawBuffer->frameInitAll();

	if (!gameSystem->isMultiplayerMode()) {
		particleMgr->setXfb(mXfbImage->mTexInfo);
	}

	// Draw particles for both viewports
	sys->mTimers->_start("part-draw", true);
	drawParticle(gfx, PLAYER1_VIEWPORT);
	drawParticle(gfx, PLAYER2_VIEWPORT);
	sys->mTimers->_stop("part-draw");

	// Draw counters for both viewports
	// (Life gauge & Carry info)
	sys->mTimers->_start("drct-post", true);
	mLightMgr->set(gfx);
	Viewport* vp = gfx.getViewport(PLAYER1_VIEWPORT);
	if (vp && vp->viewable()) {
		gfx.mCurrentViewport = vp;
		directDrawPost(gfx, vp);
	}

	mLightMgr->set(gfx);
	vp = gfx.getViewport(PLAYER2_VIEWPORT);
	if (vp && vp->viewable()) {
		gfx.mCurrentViewport = vp;
		directDrawPost(gfx, vp);
	}

	// @P2GZ fix-graphics-text-corruption: CALL ANY P2GZ UPDATES INVOLVING GRAPHICS HERE
	// Any p2gz menu that performs graphics stuff must be called here, as running it earlier in the frame before other graphics stuff gets
	// drawn causes corruption issues with JD2Print for unknown reasons

	p2gz->collision_viewer->update();
	// When we initially enter freecam menu, the input from P2GZ menu is still active since it's still the same frame (just called way
	// later). Check the lock here so we don't process that same input for freecam (otherwise we will warp frame 1 and instantly close the
	// menu).
	if (!p2gz->menu->is_lock()) {
		p2gz->freecam->update();
	}
	p2gz->waypoint_viewer->update();

	sys->mTimers->_stop("drct-post");
}

/**
 * @note Address: 0x80239ACC
 * @note Size: 0x360
 */
void BaseGameSection::newdraw_drawAll(Viewport* vp)
{
	sys->mTimers->_start("draw_calc", true);
	Graphics& gfx = *sys->mGfx;

	doSetView(vp->mVpId);
	vp->setJ3DViewMtx(true)->setViewCalcModeImm();
	doViewCalc();
	vp->setViewport();
	vp->setProjection();
	sys->mTimers->_stop("draw_calc");

	j3dSys.drawInit();

	sys->mTimers->_start("jdraw", true);
	mLightMgr->set(gfx);
	mOpaqueDrawBuffer->get(DB_FirstLayer)->draw();
	mOpaqueDrawBuffer->get(DB_MapLayer)->draw();
	mOpaqueDrawBuffer->get(DB_FarmLayer)->draw();
	mOpaqueDrawBuffer->get(DB_PikiLayer)->draw();
	mOpaqueDrawBuffer->get(DB_NormalLayer)->draw();
	doSimpleDraw(vp);
	mLightMgr->set(gfx);
	mTransparentDrawBuffer->get(DB_PikiLayer)->draw();
	mTransparentDrawBuffer->get(DB_NormalLayer)->draw();
	mLightMgr->mFogMgr->off(gfx);
	mOpaqueDrawBuffer->get(DB_NormalFogOffLayer)->draw();
	mTransparentDrawBuffer->get(DB_NormalFogOffLayer)->draw();
	mLightMgr->mFogMgr->set(gfx);
	sys->mTimers->_stop("jdraw");

	gfx.setToken("direct");

	sys->mTimers->_start("direct", true);
	j3dSys.drawInit();
	directDraw(gfx, vp);
	sys->mTimers->_stop("direct");

	Game::shadowMgr->draw(gfx, vp->mVpId);
	vp->setViewport();
	vp->setProjection();

	sys->mTimers->_start("j3d-etc", true);
	mOpaqueDrawBuffer->get(DB_PostShadowLayer)->draw();
	mTransparentDrawBuffer->get(DB_PostShadowLayer)->draw();

	// only capture normal xfb image if the 2d one doesnt exist
	if (!mXfbTexture2d && (mXfbFlags & 3) == 0) {
		mXfbImage->capture(mXfbBoundsX, mXfbBoundsY, GX_TF_RGB565, true, 0);
	}
	mLightMgr->set(gfx);
	mLightMgr->mFogMgr->off(gfx);

	mOpaqueDrawBuffer->get(DB_ObjectLastLayer)->draw();
	mTransparentDrawBuffer->get(DB_ObjectLastLayer)->draw();

	vp->setJ3DViewMtx(true);

	mLightMgr->mFogMgr->off(gfx);
	mOpaqueDrawBuffer->get(DB_PostRenderLayer)->draw();
	mTransparentDrawBuffer->get(DB_PostRenderLayer)->draw();
	mLightMgr->mFogMgr->set(gfx);
	vp->setJ3DViewMtx(true);

	mTransparentDrawBuffer->get(DB_MapLayer)->draw();
	vp->setJ3DViewMtx(false);
	sys->mTimers->_stop("j3d-etc");
}

} // namespace Game
