#include <p2gz/Trainers.h>
#include <p2gz/p2gz.h>
#include <p2gz/Utility.h>
#include <p2gz/gzMacros.h>
#include <Game/BaseGameSection.h>
#include <Game/CameraMgr.h>
#include <Game/GameLight.h>
#include <Game/MapMgr.h>
#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <Game/pelletMgr.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <Sys/TriIndexList.h>
#include <Sys/DrawBuffers.h>
#include <JSystem/J3D/J3DSys.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <Viewport.h>
#include <Camera.h>
#include <Splitter.h>
#include <System.h>
#include <og/Sound.h>

using namespace gz;

const Vector3f START_POSITION(920.0f, -18.0f, 1115.0f);
const Vector3f SUCCESS_POSITION(-53.95f, -70.88f, 2531.23f);

const f32 SUCCESS_RADIUS        = 150.0f;
const f32 CAMERA_DISTANCE       = 150.0f;
const f32 CAMERA_FOV            = 40.0f;
const f32 CAM_ROTATE_SPEED      = 0.05f;
const f32 CAM_DEFAULT_ELEVATION = 0.45f;
const f32 CAM_MAX_ELEVATION     = 1.4f;
const f32 CAM_MIN_ELEVATION     = -1.3f;

const f32 INSET_WIDTH  = 192.0f;
const f32 INSET_HEIGHT = 144.0f;
const f32 INSET_MARGIN = 24.0f;

const f32 INSET_COLLISION_RADIUS = 1024.0f;

// How far Olimar must move along the hit wall's (horizontal) normal in a frame to count as
// crossing it. Tangential seam-scraping barely moves him that way; an eject or a walk-through
// moves him along it. The bar is HIGH to arm (the big initial clip onto the seam, so approach
// bumps don't falsely arm) and LOW to fail (a slow walk-through off the seam barely registers).
// Sign is mesh-dependent, so magnitude is used. Tune from the "push=" telemetry.
const f32 WALL_ARM_PUSH  = 5.0f;
const f32 WALL_FAIL_PUSH = 1.0f;

const int RESULT_DISPLAY_FRAMES = 240;

const JUtility::TColor RESULT_GOOD = JUtility::TColor(0, 255, 0, 255);
const JUtility::TColor RESULT_BAD  = JUtility::TColor(255, 0, 0, 255);

Vector3f EarlyBluesTrainer::respawn_position()
{
	return START_POSITION;
}

Game::Navi* EarlyBluesTrainer::get_olimar()
{
	if (!Game::naviMgr || !Game::naviMgr->mArray) {
		return nullptr;
	}
	return Game::naviMgr->getAt(NAVIID_Olimar);
}

// Incomprehensible magic to make the inset camera render a black background when there are no textures to draw.
static void fill_inset_backdrop(Rectf& bounds)
{
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetCullMode(GX_CULL_NONE);
	GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
	GXSetViewport(bounds.p1.x, bounds.p1.y, bounds.getWidth(), bounds.getHeight(), 0.0f, 1.0f);
	GXSetScissor(bounds.p1.x, bounds.p1.y, bounds.getWidth(), bounds.getHeight());
	GXSetColorUpdate(GX_TRUE);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);

	Mtx44 proj;
	C_MTXOrtho(proj, bounds.p1.y, bounds.p2.y, bounds.p1.x, bounds.p2.x, -1.0f, 1.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);

	Mtx pos;
	PSMTXIdentity(pos);
	GXLoadPosMtxImm(pos, 0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(bounds.p1.x, bounds.p1.y, -0.999f);
	GXColor4u8(0, 0, 0, 255);
	GXPosition3f32(bounds.p1.x, bounds.p2.y, -0.999f);
	GXColor4u8(0, 0, 0, 255);
	GXPosition3f32(bounds.p2.x, bounds.p2.y, -0.999f);
	GXColor4u8(0, 0, 0, 255);
	GXPosition3f32(bounds.p2.x, bounds.p1.y, -0.999f);
	GXColor4u8(0, 0, 0, 255);
	GXEnd();

	GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
}

// Draw white triangles indicating the map collision in the inset camera.
static void draw_inset_collision(Graphics& gfx, Viewport* vp, Game::Navi* navi)
{
	if (!Game::mapMgr || Game::gameSystem->mIsInCave) {
		return;
	}

	Game::ShapeMapMgr* shapeMapMgr = static_cast<Game::ShapeMapMgr*>(Game::mapMgr);
	Vector3f navi_pos              = navi->getPosition();
	Sys::Sphere sphere(navi_pos, INSET_COLLISION_RADIUS);
	Sys::TriIndexList* triLists  = shapeMapMgr->mMapCollision.mDivider->findTriLists(sphere);
	Sys::VertexTable* vertTable  = shapeMapMgr->mMapCollision.mDivider->mVertexTable;
	Sys::TriangleTable* triTable = shapeMapMgr->mMapCollision.mDivider->mTriangleTable;
	if (!triLists) {
		return;
	}

	gfx.initPrimDraw(vp->getMatrix(true));

	const Vector3f light_dir(0.408f, 0.816f, 0.408f);

	for (; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			Sys::Triangle* tri  = triTable->getTriangle(triLists->mObjects[i]);
			Vector3f& normal    = tri->mTrianglePlane.mNormal;
			f32 lit             = normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z;
			u8 shade            = (u8)(160.0f + 95.0f * lit);

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
			for (int j = 0; j < 3; j++) {
				Vector3f* vertex = vertTable->getVertex(tri->mVertices[j]);
				GXPosition3f32(vertex->x, vertex->y, vertex->z);
				GXColor4u8(shade, shade, shade, 255);
			}
			GXEnd();
		}
	}
}

void EarlyBluesTrainer::stop()
{
	enabled       = false;
	pending_setup = false;
	result_frames = 0;
}

void EarlyBluesTrainer::start()
{
	if (!gz::in_play()) {
		return;
	}

	GZASSERTLINE(inset_viewport && inset_camera);

	enabled          = true;
	hell_warped      = false;
	on_seam          = false;
	has_prev         = false;
	wall_hit_pending = false;
	would_have_died  = false;
	result_frames    = 0;
	cam_azimuth      = 0.0f;
	cam_elevation    = CAM_DEFAULT_ELEVATION;

	Game::SingleGameSection* sgs = gz::get_SGS();
	bool in_awakening_wood = gz::in_above_ground_play() && sgs && sgs->mCurrentCourseInfo && sgs->mCurrentCourseInfo->mCourseIndex == COURSE_AW;
	if (in_awakening_wood) {
		p2gz->menu->close();
		setup_after_load();
	} else {
		pending_setup = true;
		WarpDestination dest;
		dest.area            = COURSE_AW;
		dest.cave            = CAVE_AboveGround;
		dest.enter_area_type = 0;
		p2gz->warp->set_dest(dest);
		p2gz->warp->do_warp();
	}
}

void EarlyBluesTrainer::init()
{
	if (inset_viewport) {
		return;
	}

	inset_camera   = new LookAtCamera();
	inset_viewport = new Viewport();
	GZASSERTLINE(inset_viewport && inset_camera);

	inset_camera->mViewAngle = CAMERA_FOV;
	inset_camera->setFixNearFar(true, 1.0f, 12800.0f);

	Vector2f screen = getScreenSize();
	Rectf rect(screen.x - INSET_WIDTH - INSET_MARGIN, screen.y - INSET_HEIGHT - INSET_MARGIN, screen.x - INSET_MARGIN, screen.y - INSET_MARGIN);
	inset_viewport->mVpId = PLAYER2_VIEWPORT;
	inset_viewport->setRect(rect);
	inset_viewport->setCamera(inset_camera);
}

void EarlyBluesTrainer::setup_after_load()
{
	pending_setup    = false;
	Game::Navi* navi = get_olimar();
	if (!navi) {
		return;
	}
	teleport_to_start(navi);
	on_seam          = false;
	has_prev         = false;
	wall_hit_pending = false;
	hell_warped      = false;
	would_have_died  = false;
}

void EarlyBluesTrainer::teleport_to_start(Game::Navi* navi)
{
	Vector3f pos = START_POSITION;
	navi->setPosition(pos, false);
}

void EarlyBluesTrainer::update()
{
	if (!enabled) {
		return;
	}

	if (pending_setup) {
		Game::SingleGameSection* sgs = gz::get_SGS();
		if (gz::in_above_ground_gameplay() && !p2gz->warp->warping && sgs && sgs->mCurrentCourseInfo
		    && sgs->mCurrentCourseInfo->mCourseIndex == COURSE_AW) {
			setup_after_load();
		}
		return;
	}

	if (result_frames > 0) {
		result_frames--;
	}

	if (!gz::in_above_ground_play()) {
		return;
	}

	Game::Navi* navi = get_olimar();
	if (!navi) {
		return;
	}

	Vector3f pos    = navi->getPosition();
	bool napsacking = navi->getStateID() == Game::NSID_Pellet;

	// before he reaches the seam, log where the warp settled him, to tune START_POSITION
	if (!on_seam) {
		OSReport("[early blues] olimar at (%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);
	}

	if (!napsacking && wall_hit_pending && has_prev && last_wall_tri) {
		// How far he moved this frame along the hit wall's horizontal normal. Tangential
		// scraping along the seam barely moves him that way (~0); an eject or a walk-through
		// across the wall moves him along it. The normal's sign is mesh-dependent, so use
		// the magnitude.
		Vector3f n     = last_wall_tri->mTrianglePlane.mNormal;
		Vector3f moved = pos - prev_pos;
		f32 nlen2      = n.x * n.x + n.z * n.z;
		f32 apush      = 0.0f;
		if (nlen2 > 0.0001f) {
			f32 nlen = sqrtf(nlen2);
			f32 push = (moved.x * n.x + moved.z * n.z) / nlen;
			apush    = push < 0.0f ? -push : push;
		}
		if (apush > 0.5f) {
			OSReport("[early blues] wall hit: push=%.2f on_seam=%d\n", apush, on_seam);
		}

		if (!on_seam) {
			// ARM: only the big initial clip onto the seam arms (approach bumps don't).
			if (apush > WALL_ARM_PUSH) {
				on_seam = true;
				OSReport("[early blues] clipped onto the seam - attempt armed\n");
			}
		} else if (apush > WALL_FAIL_PUSH) {
			// FAIL: once on the seam, a much smaller move across a wall means he walked or
			// clipped off it back toward the playfield.
			OSReport("[early blues] crossed a wall off the seam - attempt failed\n");
			resolve_attempt(navi, true);
			return;
		}
	}

	if (!napsacking) {
		// success: he made it across the seam to the blue onion target. The void fall is
		// caught independently by hell_warped (the death-plane hook).
		Vector3f to_target  = pos - SUCCESS_POSITION;
		bool reached_target = on_seam && to_target.length() < SUCCESS_RADIUS;
		if (hell_warped || reached_target) {
			resolve_attempt(navi);
			return;
		}
	}

	wall_hit_pending = false;
	prev_pos         = pos;
	has_prev         = true;
}

void EarlyBluesTrainer::resolve_attempt(Game::Navi* navi, bool clipped_back)
{
	Vector3f landed = navi->getPosition();
	Vector3f delta  = landed - SUCCESS_POSITION;
	OSReport("[early blues] attempt ended at (%.2f, %.2f, %.2f)\n", landed.x, landed.y, landed.z);

	result_success      = !hell_warped && !clipped_back && delta.length() < SUCCESS_RADIUS;
	result_clipped_back = clipped_back;
	result_softlock     = would_have_died;
	result_frames       = RESULT_DISPLAY_FRAMES;

	if (!navi->isAlive()) {
		navi->setAlive(true);
		Game::naviMgr->clearDeadCount();
		if (navi->getStateID() == Game::NSID_Dead) {
			navi->transit(Game::NSID_Walk, nullptr);
		}
	}
	navi->setLifeMax();

	teleport_to_start(navi);

	on_seam          = false;
	has_prev         = false;
	wall_hit_pending = false;
	hell_warped      = false;
	would_have_died  = false;
}

void EarlyBluesTrainer::update_inset_camera(Game::Navi* navi)
{
	if (navi->mController1 && !p2gz->menu->is_open()) {
		cam_azimuth -= navi->mController1->getSubStickX() * CAM_ROTATE_SPEED;
		f32 prev_elevation = cam_elevation;
		cam_elevation += navi->mController1->getSubStickY() * CAM_ROTATE_SPEED;
		if (cam_elevation > CAM_MAX_ELEVATION) {
			cam_elevation = CAM_MAX_ELEVATION;
		}
		if (cam_elevation < CAM_MIN_ELEVATION) {
			cam_elevation = CAM_MIN_ELEVATION;
		}

		// same audio feedback as the freecam zoom while the camera actually moves
		if (cam_elevation >= prev_elevation) {
			og::ogSound->setZoomIn();
		} else if (cam_elevation < prev_elevation) {
			og::ogSound->setZoomOut();
		}
	}

	f32 flat = CAMERA_DISTANCE * cosf(cam_elevation);
	Vector3f offset(flat * sinf(cam_azimuth), CAMERA_DISTANCE * sinf(cam_elevation), flat * cosf(cam_azimuth));

	Vector3f navi_pos             = navi->getPosition();
	inset_camera->mPosition       = navi_pos + offset;
	inset_camera->mLookAtPosition = navi_pos;

	inset_camera->updateMatrix();
	inset_camera->update();
}

// Slightly less incomprehensible magic to actually draw the inset camera.
void EarlyBluesTrainer::draw_inset(Game::BaseGameSection* section, Graphics& gfx)
{
	if (!enabled || pending_setup || !inset_viewport || !gz::in_above_ground_play()) {
		return;
	}

	Game::Navi* navi = get_olimar();
	if (!navi) {
		return;
	}

	update_inset_camera(navi);

	Viewport* vp         = inset_viewport;
	Viewport* prev_vp    = gfx.mCurrentViewport;
	gfx.mCurrentViewport = vp;

	fill_inset_backdrop(vp->mBounds2);

	section->doSetView(vp->mVpId);

	SysShape::Model::setViewCalcModeInd();
	vp->setJ3DViewMtx(false);
	section->doViewCalc();

	vp->setJ3DViewMtx(true);
	SysShape::Model::setViewCalcModeImm();
	section->doViewCalc();
	vp->setViewport();
	vp->setProjection();

	j3dSys.drawInit();
	section->mLightMgr->set(gfx);
	section->mOpaqueDrawBuffer->get(Game::DB_FarmLayer)->draw();
	section->mOpaqueDrawBuffer->get(Game::DB_PikiLayer)->draw();
	section->mOpaqueDrawBuffer->get(Game::DB_NormalLayer)->draw();
	section->doSimpleDraw(vp);
	section->mLightMgr->set(gfx);
	section->mTransparentDrawBuffer->get(Game::DB_PikiLayer)->draw();
	section->mTransparentDrawBuffer->get(Game::DB_NormalLayer)->draw();
	section->mLightMgr->mFogMgr->off(gfx);
	section->mOpaqueDrawBuffer->get(Game::DB_NormalFogOffLayer)->draw();
	section->mTransparentDrawBuffer->get(Game::DB_NormalFogOffLayer)->draw();
	section->mLightMgr->mFogMgr->set(gfx);

	draw_inset_collision(gfx, vp, navi);

	if (Game::BaseHIOParms::sDrawParticle && particleMgr) {
		vp->setProjection();
		vp->setViewport();
		if (!Game::gameSystem->isMultiplayerMode() && section->mPrevNaviIdx != NAVIID_Multiplayer) {
			section->mLightMgr->mFogMgr->off(gfx);
			particleMgr->draw(vp, 0);
			section->mLightMgr->mFogMgr->set(gfx);
		}
		particleMgr->draw(vp, 1);
		section->mLightMgr->mFogMgr->off(gfx);
		particleMgr->draw(vp, 2);
		section->mLightMgr->mFogMgr->set(gfx);
	}

	gfx.mCurrentViewport = prev_vp;
	section->doSetView(PLAYER1_VIEWPORT);
	if (prev_vp) {
		prev_vp->setViewport();
		prev_vp->setProjection();
		prev_vp->setJ3DViewMtx(false);
	}
	j3dSys.reinitGX();
}

void EarlyBluesTrainer::draw_status()
{
	if (!enabled || result_frames <= 0) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 24.0f;
	j2d.mGlyphHeight = 24.0f;

	const JUtility::TColor& warp_color = result_success ? RESULT_GOOD : RESULT_BAD;
	j2d.mCharColor.set(warp_color);
	j2d.mGradientColor.set(warp_color);
	const char* warp_text = result_success      ? "early blues: reached warp target!"
	                        : result_clipped_back ? "early blues: clipped back in bounds"
	                                              : "early blues: missed warp target";
	j2d.print(160.0f, 340.0f, warp_text);

	const JUtility::TColor& lock_color = result_softlock ? RESULT_BAD : RESULT_GOOD;
	j2d.mCharColor.set(lock_color);
	j2d.mGradientColor.set(lock_color);
	j2d.print(160.0f, 364.0f, result_softlock ? "would have softlocked in vanilla" : "no softlock");
}
