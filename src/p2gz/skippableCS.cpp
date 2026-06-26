#include <p2gz/p2gz.h>
#include <Game/MoviePlayer.h>
#include <Game/pelletMgr.h>
#include <Game/Interaction.h>
#include <Game/Entities/ItemOnyon.h>
#include <Game/EnemyBase.h>
#include <Game/enemyInfo.h>

using namespace gz;
using namespace Game;

// find the breadbug attached to this treasure, if any
static EnemyBase* find_attached_breadbug(Creature* treasure)
{
	if (!treasure) {
		return nullptr;
	}

	for (Creature* sticker = treasure->mSticked; sticker; sticker = sticker->mCaptured) {
		if (sticker->isTeki()) {
			EnemyBase* enemy = static_cast<EnemyBase*>(sticker);
			if (enemy->getEnemyTypeID() == EnemyTypeID::EnemyID_PanModoki || enemy->getEnemyTypeID() == EnemyTypeID::EnemyID_OoPanModoki) {
				return enemy;
			}
		}
	}

	return nullptr;
}

// are we currently in one of the treasure/item collection cutscenes?
static bool is_suck_movie_playing()
{
	return moviePlayer
	    && (moviePlayer->isPlaying("s22_cv_suck_treasure") || moviePlayer->isPlaying("s22_cv_suck_equipment")
	        || moviePlayer->isPlaying("s10_suck_treasure") || moviePlayer->isPlaying("s17_suck_equipment"));
}

void SkippableCutscenes::force_collect(Game::Creature* cutscene_target)
{
	if (!enabled || !cutscene_target) {
		return;
	}

	// cave -> treasure goes to pod
	if (moviePlayer->isPlaying("s22_cv_suck_treasure") || moviePlayer->isPlaying("s22_cv_suck_equipment")) {
		if (!is_treasure_collected && start_press_reason && (strcmp(start_press_reason, "moviePl:skip") == 0)) {
			Pellet* pellet = static_cast<Pellet*>(cutscene_target);
			Onyon* pod     = ItemOnyon::mgr->mPod;
			pod->mFlags.set(CF_IsMovieExtra);
			InteractSuckDone interaction = InteractSuckDone(pellet, 0);
			pod->stimulate(interaction);
			is_treasure_collected = true;
		}
	}

	// above ground -> treasure goes to ufo/ship
	if (moviePlayer->isPlaying("s10_suck_treasure") || moviePlayer->isPlaying("s17_suck_equipment")) {
		if (!is_treasure_collected && start_press_reason && (strcmp(start_press_reason, "moviePl:skip") == 0)) {
			Pellet* pellet = static_cast<Pellet*>(cutscene_target);
			Onyon* ufo     = ItemOnyon::mgr->mUfo;
			ufo->mFlags.set(CF_IsMovieExtra);
			InteractSuckDone interaction = InteractSuckDone(pellet, 0);
			ufo->stimulate(interaction);
			is_treasure_collected = true;
		}
	}
}

void SkippableCutscenes::update_breadbug_lockout()
{
	if (!enabled || !breadbug) {
		return;
	}

	// if the cutscene is no longer running, unlock it so we don't end up with stale shit
	if (!is_suck_movie_playing()) {
		breadbug = nullptr;
		return;
	}

	// unlock skipping once the breadbug has taken damage in the cutscene
	if (breadbug->mHealth < breadbug_start_health) {
		if (moviePlayer->mCurrentConfig) {
			moviePlayer->mCurrentConfig->enableSkippableWithStart();
		}
		breadbug = nullptr;
	}
}

void SkippableCutscenes::prime_skip(Creature* cutscene_target, MovieConfig* config)
{
	if (!config) {
		return;
	}

	// toggle intro crash landing/first area enter cutscenes skippable
	if (config->is("x01_gamestart") || config->is("x01_coursein_forest") || config->is("x01_coursein_yakushima")
	    || config->is("x01_coursein_last")) {
		if (enabled) {
			config->enableSkippable();

			// set skip timer
			p2gz->timer->reset_skip_timer();
			return;
		} else {
			config->disableSkippable();
		}
	}

	// treasure cutscenes need a target
	if (!cutscene_target) {
		return;
	}
	// toggle cave and above ground treasure cutscenes skippable
	if (config->is("s22_cv_suck_treasure") || config->is("s22_cv_suck_equipment") || config->is("s10_suck_treasure")
	    || config->is("s17_suck_equipment")) {
		breadbug = nullptr;
		if (enabled) {
			is_treasure_collected = false;

			// set skip timer
			p2gz->timer->reset_skip_timer();

			// prevent a cutscene from being skippable if it has a breadbug in it, until the breadbug has been damaged properly
			EnemyBase* breadbug = find_attached_breadbug(cutscene_target);
			if (breadbug) {
				config->disableSkippable();
				breadbug              = breadbug;
				breadbug_start_health = breadbug->mHealth;
			} else {
				config->enableSkippableWithStart();
			}

			// TODO: this is where we'd also record the treasure being collected for the purposes of collection statistics
		} else {
			config->disableSkippable();
		}
	}
}
