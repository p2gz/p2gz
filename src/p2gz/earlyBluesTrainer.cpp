#include <p2gz/Trainers.h>
#include <p2gz/p2gz.h>
#include <p2gz/Utility.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzmenu.h>
#include <Game/BaseGameSection.h>
#include <Game/CameraMgr.h>
#include <Game/GameLight.h>
#include <Game/MapMgr.h>
#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <Game/pelletMgr.h>
#include <Game/GameSystem.h>
#include <Game/TimeMgr.h>
#include <Game/gamePlayData.h>
#include <Controller.h>
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
const Vector3f SUCCESS_POSITION(61.19f, 10.00f, 229.37f);

const f32 SUCCESS_RADIUS        = 10.0f;
const f32 CAMERA_DISTANCE       = 150.0f;
const f32 CAMERA_FOV            = 40.0f;
const f32 CAM_ROTATE_SPEED      = 0.05f;
const f32 CAM_DEFAULT_ELEVATION = 0.45f;
const f32 CAM_MAX_ELEVATION     = 1.4f;
const f32 CAM_MIN_ELEVATION     = -1.3f;

const f32 INSET_WIDTH  = 192.0f;
const f32 INSET_HEIGHT = 144.0f;
const f32 INSET_MARGIN = 24.0f;

const f32 INSET_COLLISION_RADIUS = 512.0f;

const int B_HOLD_FRAMES = 20;

const f32 DEATH_PLANE_Y = -300.0f;

const f32 START_TIME_OF_DAY = 8.5f;

const int RESULT_DISPLAY_FRAMES = 60;
const int RESULT_FADE_FRAMES    = 30;
const int RESET_DELAY_FRAMES    = 30;

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

	int total = 0;
	for (Sys::TriIndexList* list = triLists; list; list = static_cast<Sys::TriIndexList*>(list->mNext)) {
		total += list->getNum();
	}
	if (total == 0) {
		return;
	}

	gfx.initPrimDraw(vp->getMatrix(true));

	const Vector3f light_dir(0.408f, 0.816f, 0.408f);

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, (u16)(total * 3));
	for (; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			Sys::Triangle* tri = triTable->getTriangle(triLists->mObjects[i]);
			Vector3f& normal   = tri->mTrianglePlane.mNormal;
			f32 lit            = normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z;
			u8 shade           = (u8)(160.0f + 95.0f * lit);

			for (int j = 0; j < 3; j++) {
				Vector3f* vertex = vertTable->getVertex(tri->mVertices[j]);
				GXPosition3f32(vertex->x, vertex->y, vertex->z);
				GXColor4u8(shade, shade, shade, 255);
			}
		}
	}
	GXEnd();
}

void EarlyBluesTrainer::stop()
{
	if (enabled && Game::gameSystem && Game::gameSystem->mTimeMgr) {
		Game::gameSystem->mTimeMgr->resetFlag(Game::TIMEFLAG_Stopped);
	}

	enabled              = false;
	pending_setup        = false;
	result_frames        = 0;
	pending_reset_frames = 0;
}

void EarlyBluesTrainer::start()
{
	GZASSERTLINE(inset_viewport && inset_camera);

	enabled               = true;
	went_to_hell          = false;
	would_have_softlocked = false;
	went_into_void        = false;
	result_frames         = 0;
	pending_reset_frames  = 0;
	saved_position        = START_POSITION;
	b_hold_frames         = 0;
	b_handled             = false;
	cam_azimuth           = 0.0f;
	cam_elevation         = CAM_DEFAULT_ELEVATION;
	inset_camera_visible  = true;

	pending_setup         = true;
	WarpDestination saved = p2gz->warp->get_dest();
	WarpDestination dest;
	dest.area            = COURSE_AW;
	dest.cave            = CAVE_AboveGround;
	dest.enter_area_type = 0;
	p2gz->warp->set_dest(dest);
	p2gz->warp->set_preset(p2gz->preset_mgr->find("enter SH", PoD), PS_Chosen);
	p2gz->warp->do_warp();
	p2gz->warp->set_dest(saved);
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

	if (Game::gameSystem->mTimeMgr) {
		Game::gameSystem->mTimeMgr->setTime(START_TIME_OF_DAY);
		Game::gameSystem->mTimeMgr->setFlag(Game::TIMEFLAG_Stopped);
	}

	Game::playData->mDemoFlags.resetFlag(Game::DEMO_Find_Blue_Onion);

	went_to_hell          = false;
	would_have_softlocked = false;
	went_into_void        = false;
	pending_reset_frames  = 0;
}

void EarlyBluesTrainer::teleport_to_start(Game::Navi* navi)
{
	Vector3f pos = saved_position;
	navi->setPosition(pos, false);
}

void EarlyBluesTrainer::update()
{
	if (!enabled) {
		return;
	}

	if (pending_setup) {
		Game::SingleGameSection* sgs = gz::get_SGS();
		if (gz::in_above_ground_gameplay() && !p2gz->warp->warping && sgs && sgs->mCurrentCourseInfo && sgs->mCurrentCourseInfo->mCourseIndex == COURSE_AW) {
			setup_after_load();
		}
		return;
	}

	if (result_frames > 0) {
		result_frames--;
	}

	Game::SingleGameSection* sgs = gz::get_SGS();
	bool in_awakening_wood = gz::in_above_ground_play() && sgs && sgs->mCurrentCourseInfo && sgs->mCurrentCourseInfo->mCourseIndex == COURSE_AW;
	if (!in_awakening_wood) {
		stop();
		return;
	}

	Game::Navi* navi = get_olimar();
	if (!navi) {
		return;
	}

	if (pending_reset_frames > 0) {
		if (--pending_reset_frames == 0) {
			reset_to_start(navi);
		}
		return;
	}

	Vector3f pos = navi->getPosition();

	if (!p2gz->menu->is_open()) {
		if (captured_button_down & Controller::PRESS_Y) {
			saved_position = pos;
		}

		if (captured_button_down & Controller::PRESS_A) {
			inset_camera_visible = !inset_camera_visible;
		}

		if (captured_button & Controller::PRESS_B) {
			b_hold_frames++;
			if (b_hold_frames >= B_HOLD_FRAMES && !b_handled) {
				b_handled      = true;
				saved_position = START_POSITION;
				reset_to_start(navi);
				return;
			}
		} else {
			bool tapped   = b_hold_frames > 0 && b_hold_frames < B_HOLD_FRAMES && !b_handled;
			b_hold_frames = 0;
			b_handled     = false;
			if (tapped) {
				reset_to_start(navi);
				return;
			}
		}
	}

	Vector3f to_target  = pos - SUCCESS_POSITION;
	bool reached_target = to_target.length() < SUCCESS_RADIUS;
	bool napsacking     = navi->getStateID() == Game::NSID_Pellet;

	if (reached_target) {
		resolve_attempt(navi);
		return;
	}

	if (went_to_hell && !napsacking) {
		resolve_attempt(navi);
		return;
	}

	if (napsacking && navi->mPellet) {
		Vector3f pellet_pos = navi->mPellet->getPosition();
		if (pellet_pos.y < DEATH_PLANE_Y) {
			went_into_void = true;
		} else if (went_into_void) {
			f32 dx = pellet_pos.x - SUCCESS_POSITION.x;
			f32 dz = pellet_pos.z - SUCCESS_POSITION.z;
			if (dx * dx + dz * dz >= SUCCESS_RADIUS * SUCCESS_RADIUS) {
				resolve_attempt(navi, true);
				return;
			}
		}
	}
}

// Disable whistling, switching captains, and throwing (A toggles the inset camera instead).
void EarlyBluesTrainer::capture_input(Controller* pad)
{
	captured_button      = pad->getButton();
	captured_button_down = pad->getButtonDown();

	if (pending_setup) {
		return;
	}

	u32 mask = Controller::PRESS_A | Controller::PRESS_B | Controller::PRESS_Y;
	pad->mButton.mButton &= ~mask;
	pad->mButton.mButtonDown &= ~mask;
	pad->mButton.mButtonUp &= ~mask;
}

void EarlyBluesTrainer::resolve_attempt(Game::Navi* navi, bool wrong_waypoint)
{
	Vector3f landed = navi->getPosition();
	Vector3f delta  = landed - SUCCESS_POSITION;

	result_success        = !went_to_hell && !wrong_waypoint && delta.length() < SUCCESS_RADIUS;
	result_wrong_waypoint = wrong_waypoint;
	result_softlock       = would_have_softlocked;
	result_frames         = RESULT_DISPLAY_FRAMES;

	if (went_to_hell) {
		reset_to_start(navi);
	} else {
		pending_reset_frames = RESET_DELAY_FRAMES;
	}
}

void EarlyBluesTrainer::reset_to_start(Game::Navi* navi)
{
	if (!navi->isAlive()) {
		navi->setAlive(true);
		Game::naviMgr->clearDeadCount();
	}
	navi->setLifeMax();

	int state = navi->getStateID();
	if (state == Game::NSID_Dead || state == Game::NSID_Pellet) {
		navi->transit(Game::NSID_Walk, nullptr);
	}

	teleport_to_start(navi);

	Game::playData->mDemoFlags.resetFlag(Game::DEMO_Find_Blue_Onion);

	went_to_hell          = false;
	would_have_softlocked = false;
	went_into_void        = false;
	pending_reset_frames  = 0;
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

		if (cam_elevation > prev_elevation) {
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
	if (!enabled || pending_setup || !inset_viewport || !inset_camera_visible || !gz::in_above_ground_play()) {
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

static void draw_control_line(J2DPrint& j2d, const char* icon, const char* label, f32 z)
{
	j2d.mCharColor.set(p2gz->menu->color_std);
	j2d.mGradientColor.set(p2gz->menu->color_std);

	f32 x = 16.0f;
	x += p2gz->images->draw(icon, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0f));
	x += p2gz->images->spacing() / 2.0f;
	j2d.initiate();
	j2d.print(x, z, "%s", label);
}

void EarlyBluesTrainer::draw_status()
{
	if (!enabled) {
		return;
	}

	if (!pending_setup && !p2gz->menu->is_open() && gz::in_above_ground_play()) {
		J2DPrint tips(gP2JMEMgr->mFont, 0.0f);
		tips.initiate();
		tips.mGlyphWidth  = p2gz->menu->glyph_width;
		tips.mGlyphHeight = p2gz->menu->glyph_height;

		const f32 line_h   = p2gz->images->height() + 4.0f;
		const f32 bottom_z = 430.0f;
		draw_control_line(tips, "a_btn", inset_camera_visible ? "hide inset camera" : "show inset camera", bottom_z - 3.0f * line_h);
		draw_control_line(tips, "b_btn", "reset (hold for default)", bottom_z - 2.0f * line_h);
		draw_control_line(tips, "y_btn", "move reset position", bottom_z - line_h);
		draw_control_line(tips, "c_stick", "pan inset camera", bottom_z);
	}

	if (result_frames <= 0) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 24.0f;
	j2d.mGlyphHeight = 24.0f;

	u8 alpha = result_frames >= RESULT_FADE_FRAMES ? 255 : (u8)(255 * result_frames / RESULT_FADE_FRAMES);

	const f32 result_glyph    = 24.0f;
	const f32 ctrl_icon_top   = (430.0f - 3.0f * (p2gz->images->height() + 4.0f)) - p2gz->images->height() + (p2gz->menu->line_height / 2.0f);
	const f32 result_bottom_z = ctrl_icon_top - 8.0f - result_glyph;
	const f32 warp_text_z     = result_success ? result_bottom_z - result_glyph : result_bottom_z;

	JUtility::TColor warp_color = result_success ? RESULT_GOOD : RESULT_BAD;
	warp_color.a                = alpha;
	j2d.mCharColor.set(warp_color);
	j2d.mGradientColor.set(warp_color);
	j2d.print(160.0f, warp_text_z, result_success ? "reached waypoint" : result_wrong_waypoint ? "wrong waypoint" : "did not nap");

	if (result_success) {
		JUtility::TColor lock_color = result_softlock ? RESULT_BAD : RESULT_GOOD;
		lock_color.a                = alpha;
		j2d.mCharColor.set(lock_color);
		j2d.mGradientColor.set(lock_color);
		j2d.print(160.0f, result_bottom_z, result_softlock ? "would have softlocked" : "no softlock");
	}
}
