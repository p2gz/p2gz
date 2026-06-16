#ifndef _TRAINERS_H
#define _TRAINERS_H

#include <Game/Entities/Queen.h>
#include <Vector3.h>

struct Graphics;
struct Viewport;
struct LookAtCamera;

namespace Game {
struct BaseGameSection;
struct Navi;
} // namespace Game

namespace Sys {
struct Triangle;
} // namespace Sys

namespace gz {
struct EmpressTrainer {
public:
	EmpressTrainer()
	{
		enabled            = false;
		polling            = false;
		first_damage_frame = -1;
	}
	~EmpressTrainer() { }

	void stop() { enabled = false; }

	void draw();
	void start();
	void update();

private:
	Game::Queen::Obj* get_empress();

	bool enabled;
	bool polling;
	int last_flick_count;
	int first_damage_frame;
	int fade_out_frames;
};

// Practice trainer for the Awakening Wood "early blues" out-of-bounds trick. Warps to AW,
// places Olimar at a fixed start, shows an inset camera orbiting him (c-stick rotates it).
// The trick: clip out of bounds onto a floor seam past the edge wall and walk it to the
// blue onion. The attempt ends (reset + report) when he reaches the target (success), falls
// into the void death plane, or a wall shoves him off the seam back into the playfield.
struct EarlyBluesTrainer {
public:
	EarlyBluesTrainer()
	{
		enabled          = false;
		pending_setup    = false;
		hell_warped      = false;
		on_seam          = false;
		has_prev         = false;
		wall_hit_pending = false;
		last_wall_tri    = nullptr;
		would_have_died  = false;
		result_frames       = 0;
		result_success      = false;
		result_softlock     = false;
		result_clipped_back = false;
		inset_viewport  = nullptr;
		inset_camera    = nullptr;
		cam_azimuth     = 0.0f;
		cam_elevation   = 0.0f;
	}
	~EarlyBluesTrainer() { }

	void init(); // allocates the inset viewport + camera on the sys heap, called from P2GZ::init
	void start();
	void stop();
	bool is_enabled() { return enabled; }

	// the inset render pass will run this frame (gates the deferred frameInitAll in
	// newdraw_draw3D_all / BaseGameSection::doDraw)
	bool is_inset_active() { return enabled && !pending_setup; }

	// while the inset is live, every creature must enter the draw buffers so the
	// out-of-bounds camera has something to render (see Creature::updateLOD)
	bool force_visible_lod() { return is_inset_active(); }

	void update();
	void draw_status();
	void draw_inset(Game::BaseGameSection* section, Graphics& gfx);

	// where the death-plane hook in FakePiki::doSimulation respawns Olimar
	Vector3f respawn_position();
	void on_death_plane_warp() { hell_warped = true; }

	// called from the Early Blues Patch branch in Creature::checkHell, at the exact
	// spot where vanilla would have killed the captain (the softlock)
	void on_vanilla_death_averted() { would_have_died = true; }

	// called from the wall-collision site in FakePiki::move when the EB captain hits a wall;
	// update() then checks whether he crossed that wall's plane (left the seam)
	void note_wall_hit(Sys::Triangle* tri)
	{
		last_wall_tri    = tri;
		wall_hit_pending = true;
	}

private:
	Game::Navi* get_olimar();
	void setup_after_load();
	void teleport_to_start(Game::Navi* navi);
	void resolve_attempt(Game::Navi* navi, bool clipped_back = false);
	void update_inset_camera(Game::Navi* navi);

	bool enabled;
	bool pending_setup;   // warp to AW issued, waiting for gameplay to begin
	bool hell_warped;       // this attempt ended via the death plane (fell into the void)
	bool on_seam;           // sticky: Olimar has clipped out onto the OOB seam (attempt armed)
	bool would_have_died;   // vanilla checkHell would have killed the captain this attempt
	int result_frames;      // remaining frames to show the attempt result text
	bool result_success;
	bool result_softlock;
	bool result_clipped_back; // this attempt ended by being shoved off the seam back in bounds

	Vector3f prev_pos;        // Olimar's position last frame (segment clip + displacement tests)
	bool has_prev;            // prev_pos holds a valid sample
	bool wall_hit_pending;        // the FakePiki hook flagged a wall collision this frame
	Sys::Triangle* last_wall_tri; // the wall triangle hit, for the plane-straddle cross test

	Viewport* inset_viewport;
	LookAtCamera* inset_camera;
	f32 cam_azimuth;
	f32 cam_elevation;
};
} // namespace gz

#endif
