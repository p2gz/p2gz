#ifndef _TRAINERS_H
#define _TRAINERS_H

#include <Game/Entities/Queen.h>
#include <Vector3.h>

struct Graphics;
struct Viewport;
struct LookAtCamera;
struct Controller;

namespace Game {
struct BaseGameSection;
struct Navi;
} // namespace Game

namespace gz {
struct EmpressTrainer {
public:
	EmpressTrainer()
	{
		enabled            = false;
		polling            = false;
		last_flick_count   = 0;
		first_damage_frame = -1;
		fade_out_frames    = 0;
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

struct EarlyBluesTrainer {
public:
	EarlyBluesTrainer()
	{
		enabled               = false;
		pending_setup         = false;
		went_to_hell          = false;
		would_have_softlocked = false;
		went_into_void        = false;
		pending_reset_frames  = 0;
		result_frames         = 0;
		result_success        = false;
		result_softlock       = false;
		result_wrong_waypoint = false;
		saved_position        = Vector3f(0.0f);
		captured_button       = 0;
		captured_button_down  = 0;
		b_hold_frames         = 0;
		b_handled             = false;
		inset_viewport        = nullptr;
		inset_camera          = nullptr;
		cam_azimuth           = 0.0f;
		cam_elevation         = 0.0f;
	}
	~EarlyBluesTrainer() { }

	void init();
	void start();
	void stop();
	bool is_enabled() { return enabled; }
	bool is_pending_setup() { return pending_setup; }
	bool is_inset_active() { return enabled && !pending_setup; }

	void update();
	void draw_status();
	void draw_inset(Game::BaseGameSection* section, Graphics& gfx);

	Vector3f respawn_position();
	void on_death_plane_warp() { went_to_hell = true; }

	void on_softlock() { would_have_softlocked = true; }
	void capture_input(Controller* pad);

private:
	Game::Navi* get_olimar();
	void setup_after_load();
	void teleport_to_start(Game::Navi* navi);
	void resolve_attempt(Game::Navi* navi, bool wrong_waypoint = false);
	void reset_to_start(Game::Navi* navi);
	void update_inset_camera(Game::Navi* navi);

	bool enabled;
	bool pending_setup;
	bool went_to_hell;
	bool would_have_softlocked;
	bool went_into_void;
	int pending_reset_frames;
	int result_frames;
	bool result_success;
	bool result_softlock;
	bool result_wrong_waypoint;

	Vector3f saved_position;
	u32 captured_button;
	u32 captured_button_down;
	int b_hold_frames;
	bool b_handled;

	Viewport* inset_viewport;
	LookAtCamera* inset_camera;
	f32 cam_azimuth;
	f32 cam_elevation;
};
} // namespace gz

#endif
