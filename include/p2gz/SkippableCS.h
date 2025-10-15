#ifndef _GZ_SKIPPABLECS_H
#define _GZ_SKIPPABLECS_H

// predeclarations
namespace Game {
struct Creature;
struct MovieConfig;
} // namespace Game

namespace gz {

struct SkippableTreasureCS {
public:
	SkippableTreasureCS()
	    : enabled(true)
	    , is_treasure_collected(false)
	    , start_press_reason(nullptr)
	{
	}

	void toggle_skippable(bool enabled_) { enabled = enabled_; }

	// if we're in the right cutscene and we're skipping, make the treasure force collect
	void force_collect(Game::Creature* cutscene_target);

	// if we skip after treasure hits ship, flag that we've already collected the treasure
	// so we don't do it twice
	void set_collected() { is_treasure_collected = true; }

	// if we're in the right cutscene, make cutscene skippable
	void prime_skip(Game::Creature* cutscene_target, Game::MovieConfig* config);

	// keep track of why we hit start/"paused" the game, so we know when we're skipping a cutscene
	void record_start_press(char* start_press_reason_) { start_press_reason = start_press_reason_; }

private:
	bool enabled;
	bool is_treasure_collected;
	char* start_press_reason; // to know when we're Actually Skipping
};

}; // namespace gz

#endif
