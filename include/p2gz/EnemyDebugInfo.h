#ifndef _GZ_ENEMY_DEBUG_INFO_H
#define _GZ_ENEMY_DEBUG_INFO_H

#include <Game/EnemyBase.h>
#include <p2gz/gzCollections.h>
#include <Graphics.h>
#include <types.h>
#include <Vector3.h>
#include <Color4.h>

namespace gz {

struct EnemyDebugInfo {
	EnemyDebugInfo();

	void draw();

	void set_enabled(bool enabled_) { enabled = enabled_; }
	void push_enemy(Game::EnemyBase* enemy) { enemies.push(enemy); }
	void remove_enemy(Game::EnemyBase* enemy);

	void set_max_display_dist(f32 dist) { max_dist = dist; }
	void set_size(s32 size);

	void set_draw_enemy_name_enabled(bool enabled_) { draw_enemy_name = enabled_; }
	void set_draw_cur_state_enabled(bool enabled_) { draw_cur_state = enabled_; }
	void set_draw_flick_count_enabled(bool enabled_) { draw_flick_count = enabled_; }
	void set_draw_position_enabled(bool enabled_) { draw_position = enabled_; }

private:
	void draw_enemy_dbg(Game::EnemyBase* enemy, Graphics* gfx);

	Vec<Game::EnemyBase*> enemies;
	bool enabled;
	f32 max_dist;    // max distance from active navi to enemy to draw debug info
	int line_height; // in pixels I think?
	Vec<Color4> text_colors;
	f32 text_scale;

	bool draw_enemy_name;
	bool draw_cur_state;
	bool draw_flick_count;
	bool draw_position;

	size_t cur_color;
};

}; // namespace gz

#endif
