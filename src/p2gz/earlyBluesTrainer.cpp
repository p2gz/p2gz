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

using namespace gz;

// at the base of the UP seesaw block of the pair nearest White Flower Garden (gen pair
// id {0000} in defaultgen.txt; the first block of a pair to init becomes the UP one,
// see ItemDownFloor::Item::initDependency). y matches the lower block's base so olimar
// stands below the raised block.
const Vector3f START_POSITION(920.0f, -18.0f, 1115.0f);

// waypoint 89 of forest route.txt, right next to the x2 pellet posy spawn on the west
// side of the onion plaza - the napsack recovery target for the trick
const Vector3f SUCCESS_POSITION(-53.95f, -70.88f, 2531.23f);

const f32 SUCCESS_RADIUS        = 150.0f; // how close the napsack return must land to SUCCESS_POSITION
const f32 CAMERA_DISTANCE       = 150.0f;
const f32 CAMERA_FOV            = 40.0f;
const f32 CAM_ROTATE_SPEED      = 0.1f; // radians per frame at full c-stick deflection
const f32 CAM_DEFAULT_ELEVATION = 0.45f;
const f32 CAM_MAX_ELEVATION     = 1.4f;
const f32 CAM_MIN_ELEVATION     = -1.3f; // allow looking up from below for out-of-bounds views

const f32 INSET_WIDTH  = 192.0f;
const f32 INSET_HEIGHT = 144.0f;
const f32 INSET_MARGIN = 24.0f; // keep clear of CRT overscan

const f32 INSET_COLLISION_RADIUS = 1024.0f; // triangle draw radius around olimar (matches collision viewer)

const f32 BELOW_MAP_Y = -300.0f; // checkHell's CREATURE_HELL_BELOWMAP threshold

const int RESULT_DISPLAY_FRAMES = 240;

const JUtility::TColor RESULT_GOOD = JUtility::TColor(0, 255, 0, 255);
const JUtility::TColor RESULT_BAD  = JUtility::TColor(255, 0, 0, 255);

Vector3f EarlyBluesTrainer::respawn_position()
{
	return START_POSITION;
}

Game::Navi* EarlyBluesTrainer::get_olimar()
{
	// mArray can be null/torn for a frame around section transitions (same guard as
	// EmpressTrainer::draw)
	if (!Game::naviMgr || !Game::naviMgr->mArray) {
		return nullptr;
	}
	return Game::naviMgr->getAt(NAVIID_Olimar);
}

// fill the inset rect with opaque black at far depth, like a freshly cleared frame:
// the main view already rendered here, and wherever our camera sees nothing the main
// view's pixels must not show through (same quad technique as Graphics::fillZBuffer,
// but writing color as well as depth)
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

// draw the collision triangles around the captain, plain white. initPrimDraw sets
// cull-none and z-test+write, so the triangles are visible from BOTH sides and
// occlude correctly - out of bounds you mostly look at their back faces, and judging
// the distance to them (without clipping back in bounds) is the whole point
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

	// flat-shade each triangle off its normal against a fixed diagonal light, so any
	// two differently-angled surfaces get distinct, stable shades (unlike |normal.y|,
	// which can't separate walls facing different directions). normalized (1, 2, 1)
	const Vector3f light_dir(0.408f, 0.816f, 0.408f);

	for (; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			Sys::Triangle* tri  = triTable->getTriangle(triLists->mObjects[i]);
			Vector3f& normal    = tri->mTrianglePlane.mNormal;
			f32 lit             = normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z;
			u8 shade            = (u8)(160.0f + 95.0f * lit); // [-1,1] -> 65..255, never backdrop-black

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

	enabled         = true;
	hell_warped     = false;
	was_below_map   = false;
	would_have_died = false;
	result_frames   = 0;
	cam_azimuth     = 0.0f;
	cam_elevation   = CAM_DEFAULT_ELEVATION;

	Game::SingleGameSection* sgs = gz::get_SGS();
	bool in_awakening_wood
	    = gz::in_above_ground_play() && sgs && sgs->mCurrentCourseInfo && sgs->mCurrentCourseInfo->mCourseIndex == COURSE_AW;
	if (in_awakening_wood) {
		p2gz->menu->close();
		setup_after_load();
	} else {
		pending_setup = true;
		WarpDestination dest;
		dest.area            = COURSE_AW;
		dest.cave            = CAVE_AboveGround;
		dest.enter_area_type = 0; // falling entry, skips the ship fly-in
		p2gz->warp->set_dest(dest);
		p2gz->warp->do_warp();
	}
}

void EarlyBluesTrainer::init()
{
	if (inset_viewport) {
		return;
	}

	// P2GZ::init has already made sys->mSysHeap the current heap here
	inset_camera   = new LookAtCamera();
	inset_viewport = new Viewport();
	GZASSERTLINE(inset_viewport && inset_camera);

	inset_camera->mViewAngle = CAMERA_FOV;
	inset_camera->setFixNearFar(true, 1.0f, 12800.0f);

	Vector2f screen = getScreenSize();
	Rectf rect(screen.x - INSET_WIDTH - INSET_MARGIN, screen.y - INSET_HEIGHT - INSET_MARGIN, screen.x - INSET_MARGIN,
	           screen.y - INSET_MARGIN);
	// use view slot 1 (louie's, unused in single player): J3D double-buffers draw
	// matrices per view slot and swaps them once per doViewCalc, so re-running view 0
	// would toggle slot 0 twice per frame and leave the engine drawing stale matrices
	inset_viewport->mVpId = PLAYER2_VIEWPORT;
	inset_viewport->setRect(rect);
	inset_viewport->setCamera(inset_camera); // also derives the aspect ratio from the rect
}

void EarlyBluesTrainer::setup_after_load()
{
	pending_setup    = false;
	Game::Navi* navi = get_olimar();
	if (!navi) {
		return;
	}
	teleport_to_start(navi);
	was_below_map   = false;
	hell_warped     = false;
	would_have_died = false;
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

	// an attempt is in progress once the captain or the napsack pellet carrying him
	// drops below the map
	f32 y = navi->getPosition().y;
	if (navi->getStateID() == Game::NSID_Pellet && navi->mPellet) {
		f32 pellet_y = navi->mPellet->getPosition().y;
		if (pellet_y < y) {
			y = pellet_y;
		}
	}

	if (y < BELOW_MAP_Y) {
		was_below_map = true;
	} else if (was_below_map && navi->getStateID() != Game::NSID_Pellet) {
		// the captain is back above the map and done riding: either our death-plane hook
		// or the napsack return put them here
		resolve_attempt(navi);
	}
}

void EarlyBluesTrainer::resolve_attempt(Game::Navi* navi)
{
	// where the game's own recovery placed the captain, before we snap them back
	Vector3f landed = navi->getPosition();
	Vector3f delta  = landed - SUCCESS_POSITION;

	result_success  = !hell_warped && delta.length() < SUCCESS_RADIUS;
	result_softlock = would_have_died;
	result_frames   = RESULT_DISPLAY_FRAMES;

	// the splitting glitch can leave the captain considered dead (see the Early Blues
	// Patch in creature.cpp), so force them back to life
	if (!navi->isAlive()) {
		navi->setAlive(true);
		Game::naviMgr->clearDeadCount();
		if (navi->getStateID() == Game::NSID_Dead) {
			navi->transit(Game::NSID_Walk, nullptr);
		}
	}
	navi->setLifeMax();

	teleport_to_start(navi);

	was_below_map   = false;
	hell_warped     = false;
	would_have_died = false;
}

void EarlyBluesTrainer::update_inset_camera(Game::Navi* navi)
{
	if (navi->mController1 && !p2gz->menu->is_open()) {
		cam_azimuth -= navi->mController1->getSubStickX() * CAM_ROTATE_SPEED;
		cam_elevation += navi->mController1->getSubStickY() * CAM_ROTATE_SPEED;
		if (cam_elevation > CAM_MAX_ELEVATION) {
			cam_elevation = CAM_MAX_ELEVATION;
		}
		if (cam_elevation < CAM_MIN_ELEVATION) {
			cam_elevation = CAM_MIN_ELEVATION;
		}
	}

	// orbit around the captain at a fixed distance, always looking at him; only the
	// c-stick changes the orbit angles, his movement just carries the orbit along
	f32 flat = CAMERA_DISTANCE * cosf(cam_elevation);
	Vector3f offset(flat * sinf(cam_azimuth), CAMERA_DISTANCE * sinf(cam_elevation), flat * cosf(cam_azimuth));

	Vector3f navi_pos             = navi->getPosition();
	inset_camera->mPosition       = navi_pos + offset;
	inset_camera->mLookAtPosition = navi_pos;

	// updateMatrix first so Camera::update's view-matrix snapshot isn't a frame behind
	inset_camera->updateMatrix();
	inset_camera->update();
}

// Renders the whole scene a second time into the inset viewport, on top of the HUD.
// Runs from BaseGameSection::doDraw after draw2D; newdraw_draw3D_all defers its
// frameInitAll while the inset is active so the scene draw buffers are still
// populated here (doDraw resets them right after this returns).
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

	// black out the inset rect (color + depth) so the inset behaves like its own
	// little freshly-cleared frame
	fill_inset_backdrop(vp->mBounds2);

	section->doSetView(vp->mVpId);

	// the map and other non-immediate models only compute their per-view-slot
	// matrices in an Ind-mode pass over viewable registered viewports (see
	// baseGameSection.cpp:316-322), which never covers our slot - run it ourselves
	// with our camera (same Ind-then-Imm pattern as the Kantei treasure window)
	SysShape::Model::setViewCalcModeInd();
	vp->setJ3DViewMtx(false);
	section->doViewCalc();

	// immediate-mtx models (creatures etc.) calc from the j3dSys matrix instead
	vp->setJ3DViewMtx(true);
	SysShape::Model::setViewCalcModeImm();
	section->doViewCalc();
	vp->setViewport();
	vp->setProjection();

	// same layer sequence as newdraw_drawAll, minus shadows, XFB capture and direct
	// draws - and minus the map/skybox layers (DB_FirstLayer, DB_MapLayer): the inset
	// shows collision triangles instead of map textures
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

	// hand GX state back to the viewport that was current before our pass, put the
	// current J3D view slot back on the main view, and reset GX for the 2D drawing
	// that follows in doDraw (same as the Kantei window)
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
	j2d.print(160.0f, 340.0f, result_success ? "early blues: reached warp target!" : "early blues: missed warp target");

	const JUtility::TColor& lock_color = result_softlock ? RESULT_BAD : RESULT_GOOD;
	j2d.mCharColor.set(lock_color);
	j2d.mGradientColor.set(lock_color);
	j2d.print(160.0f, 364.0f, result_softlock ? "would have softlocked in vanilla" : "no softlock");
}
