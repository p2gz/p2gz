#ifndef _GZ_SKIPPABLECS_H
#define _GZ_SKIPPABLECS_H

#include <types.h>

// predeclarations
namespace Game {
struct Creature;
struct EnemyBase;
struct MovieConfig;
} // namespace Game

namespace gz {

struct SkippableCutscenes {
public:
	SkippableCutscenes()
	    : enabled(true)
	    , is_treasure_collected(false)
	    , start_press_reason(nullptr)
	    , breadbug(nullptr)
	    , breadbug_start_health(0.0f)
	{
	}

	void toggle_skippable(bool enabled_) { enabled = enabled_; }
	bool is_skippable() { return enabled; }

	// if we're in the right cutscene and we're skipping, make the treasure force collect
	void force_collect(Game::Creature* cutscene_target);

	// if we skip after treasure hits ship, flag that we've already collected the treasure
	// so we don't do it twice
	void set_collected() { is_treasure_collected = true; }

	// if we're in the right cutscene, make cutscene skippable
	void prime_skip(Game::Creature* cutscene_target, Game::MovieConfig* config);

	// check when we can skip a cutscene that involves a breadbug
	void update_breadbug_lockout();

	// keep track of why we hit start/"paused" the game, so we know when we're skipping a cutscene
	void record_start_press(char* start_press_reason_) { start_press_reason = start_press_reason_; }

private:
	bool enabled;
	bool is_treasure_collected;
	char* start_press_reason; // to know when we're Actually Skipping

	Game::EnemyBase* breadbug; // breadbug attached to treasure in this cutscene
	f32 breadbug_start_health; // breadbug reference health (from cutscene start)
};

}; // namespace gz

#endif
