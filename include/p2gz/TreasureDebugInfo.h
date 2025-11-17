#ifndef _GZ_TREASURE_DEBUG_INFO_H
#define _GZ_TREASURE_DEBUG_INFO_H

#include <p2gz/gzCollections.h>
#include <Graphics.h>
#include <Game/pelletMgr.h>

namespace gz {

struct TreasureDebugInfo {
public:
	TreasureDebugInfo();

	void draw();

	void set_enabled(bool enabled_) { enabled = enabled_; }

private:
	void draw_treasure_dbg(Game::Pellet* pellet, Graphics* gfx);

	bool enabled;
};

}; // namespace gz

#endif
