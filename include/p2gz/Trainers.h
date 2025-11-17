#ifndef _TRAINERS_H
#define _TRAINERS_H

#include <Game/Entities/Queen.h>

namespace gz {
struct EmpressTrainer {
public:
	EmpressTrainer() { first_damage_frame = -1; }
	~EmpressTrainer() { }

	void stop() { enabled = false; }

	void draw();
	void start();
	void update();

private:
	Game::Queen::Obj* get_empress();

	bool enabled;
	int last_flick_count;
	int first_damage_frame;
	int fade_out_frames;
};
} // namespace gz

#endif
