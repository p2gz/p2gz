#ifndef _TRAINERS_H
#define _TRAINERS_H

namespace gz {
struct EmpressTrainer {
	EmpressTrainer() { first_flick_frame = -1; }
	~EmpressTrainer() { }

	void draw();

	int last_flick_count;
	int first_flick_frame;
	int fade_out_frames;
};
} // namespace gz

#endif
