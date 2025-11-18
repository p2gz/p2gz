#ifndef _GZ_GENERATOR_DEBUG_INFO_H
#define _GZ_GENERATOR_DEBUG_INFO_H

#include <types.h>
#include <Graphics.h>
#include <Game/gameGenerator.h>

namespace gz {

struct GeneratorDebugInfo {
	GeneratorDebugInfo();

	void draw();

	void set_enabled(bool enabled_) { enabled = enabled_; }

private:
	void draw_gen_dbg(Game::Generator* gen, Graphics* gfx);

	bool enabled;
	f32 max_dist;
};

}; // namespace gz

#endif
