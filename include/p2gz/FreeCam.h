struct FreeCam {
public:
	FreeCam(bool teleport_)
	    : teleport(teleport_)
	{
	}
	~FreeCam() { }

	Game::PlayCamera* camera;
	Game::Navi* navi;
	bool teleport;
	bool enabled;
	int zoom;
	f32 animation_coefficient;
	bool is_coefficient_positive;

	void disable();
	void enable();
	void switch_captains();
	void update();
	void warp_to_current_position();

private:
	void draw_current_position();
	void update_position();
	void update_zoom();
};
