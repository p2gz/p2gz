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
	void set_draw_name_id_enabled(bool enabled_) { draw_name_id = enabled_; }
	void set_draw_position_enabled(bool enabled_) { draw_position = enabled_; }
	void set_draw_carry_power_enabled(bool enabled_) { draw_carry_power = enabled_; }
	void set_draw_velocity_enabled(bool enabled_) { draw_velocity = enabled_; }

private:
	void draw_treasure_dbg(Game::Pellet* pellet, Graphics* gfx);

	bool enabled;
	bool draw_name_id;
	bool draw_position; 
	bool draw_carry_power;  
	bool draw_velocity; 

};

}; // namespace gz

#endif
