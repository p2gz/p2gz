#include <p2gz/RaceMode.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Utility.h>
#include <p2gz/InputHelpers.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <System.h>
#include <Dolphin/os.h>
#include <Game/GameSystem.h>
#include <Game/gamePlayData.h>
#include <Game/SingleGame.h>
#include <Game/SingleGameSection.h>
#include <Game/THPPlayer.h>
#include <Game/DeathMgr.h>
#include <Game/MoviePlayer.h>
#include <TParticle2dMgr.h>

using namespace gz;

// Both 0 for now; tunable later (internal, deliberately NOT a menu option).
const f32 RaceMode::RESET_PENALTY_SET_SEED = 0.0f;
const f32 RaceMode::RESET_PENALTY_RANDOM   = 0.0f;

// abort combo: hold L + R + Z + Start
const u32 RaceMode::ABORT_COMBO_MASK = Controller::PRESS_L | Controller::PRESS_R | Controller::PRESS_Z | Controller::PRESS_START;

RaceMode::RaceMode()
{
	active            = false;
	finished          = false;
	at_fresh_start    = false;
	category          = PoD;
	use_set_seed      = false;
	seed              = 0;
	reset_count       = 0;
	load_ms           = 0;
	was_loading       = false;
	load_enter_ms     = 0;
	abort_hold_frames = 0;
	pending_reset     = false;
	final_rta_ms      = 0;
	final_igt_ms      = 0;
	final_pikmin_lost = 0;
	final_is_complete = false;
	enemies_defeated  = 0;
	pikmin_thrown     = 0;
	category_opt      = nullptr;
	region_opt        = nullptr;
}

u32 RaceMode::cur_ms() { return (u32)OSTicksToMilliseconds(OSGetTime()); }

void RaceMode::ms_to_clock(u32 ms, u32& minutes, u32& seconds, u32& tenths)
{
	minutes = ms / (60 * 1000);
	seconds = (ms / 1000) % 60;
	tenths  = (ms / 100) % 10;
}

void RaceMode::init()
{
	// Populate the category + region radios. The menu structure itself is built in gzmenu.cpp.
	category_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("race/category"));
	if (category_opt) {
		category_opt->options.clear();
		category_opt->options.push("PoD");
		category_opt->options.push("AT");
	}
	region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("race/treasure region"));
	if (region_opt) {
		region_opt->options.clear();
		region_opt->options.push("US");
		region_opt->options.push("JP");
		region_opt->options.push("PAL");
	}
}

void RaceMode::set_category(size_t idx) { category = (idx == 0) ? PoD : AT; }

void RaceMode::set_seed(u32 seed_)
{
	use_set_seed = true;
	seed         = seed_;
}

void RaceMode::set_random_seed() { use_set_seed = false; }

void RaceMode::start_run()
{
	// Lock in the run configuration
	active            = true;
	finished          = false;
	at_fresh_start    = true; // the crash-landing intro is only reproducible via a new-file restart
	reset_count       = 0;
	load_ms           = 0;
	was_loading       = false;
	load_enter_ms     = 0;
	abort_hold_frames = 0;
	pending_reset     = false;
	enemies_defeated  = 0;
	pikmin_thrown     = 0;

	// The treasure region is applied via the race menu's region radio, which is bound directly to
	// Localization::set_treasure_region (see gzmenu.cpp); it takes effect on the upcoming load.

	// Close the menu FIRST. Start run! is selected with the menu open, which means the timer is
	// paused; close() unpauses it. If we reset the timer base before unpausing, unpause() would then
	// shift main_timer into the future by the menu-open duration, making get_main_elapsed_ms()
	// underflow into a garbage RTA until a cutscene skip happens to correct main_timer.
	p2gz->menu->close();

	// Start the dual timer. RTA = main_timer (keeps cutscene-skip compensation); IGT = RTA - loads.
	p2gz->timer->set_enabled(true);
	p2gz->timer->set_sub_timer_enabled(false);
	p2gz->timer->reset_main_timer();

	// Kick off a genuine new file (mirrors the file-select New Game path)
	begin_fresh_file();
}

void RaceMode::begin_fresh_file()
{
	Game::SingleGameSection* game = get_SGS();
	if (!game) {
		return;
	}

	// Tear down the live world the same way the warp system does, so starting from mid-gameplay is clean
	Game::gameSystem->resetFlag(Game::GAMESYS_IsGameWorldActive);
	if (Game::moviePlayer) {
		Game::moviePlayer->reset();
	}
	if (particle2dMgr) {
		particle2dMgr->killAll();
	}

	// Full fresh-file init, then drive the real New Game sequence:
	//   reset() -> mLoadType=NewFile -> dayCount=0 -> SGS_Movie(OPENING_GameStart)
	//   -> (auto-skipped) -> SGS_Load(MapEnter_NewGame) -> crash landing (skippable)
	Game::playData->reset();
	Game::playData->mLoadType        = Game::STORYSAVE_NewFile;
	Game::gameSystem->mTimeMgr->mDayCount = 0;

	Game::SingleGame::MovieArg arg(Game::THPPlayer::OPENING_GameStart);
	game->mFsm->transit(game, Game::SingleGame::SGS_Movie, &arg);
}

void RaceMode::request_reset()
{
	if (!active || finished) {
		return;
	}
	pending_reset = true;
}

void RaceMode::do_reset()
{
	// Only meaningful with the player actually in a floor/area
	if (!in_cave_play() && !in_above_ground_play()) {
		return;
	}

	reset_count++;

	const f32 penalty = use_set_seed ? RESET_PENALTY_SET_SEED : RESET_PENALTY_RANDOM;
	if (penalty > 0.0f) {
		p2gz->timer->offset_main_timer(penalty); // positive offset adds time; IGT inherits it
	}

	// The day-1 crash-landing segment can't be reproduced by a warp: the first bulborb, the initial
	// red unlock, and pellet posies that assume reds are all MapEnter_NewGame-only generator state, so
	// warping there sets day 256, skips the bulborb, and crashes on red-pellet spawns. Re-run the new
	// file instead (keeps the run clock + penalty + reset count).
	if (at_fresh_start) {
		// begin_fresh_file() runs PlayData::reset(), which zeroes the game's death/birth counters;
		// zero our run stats to match so the summary reflects the attempt from this restart.
		enemies_defeated = 0;
		pikmin_thrown    = 0;
		begin_fresh_file();
		return;
	}

	// Same floor/segment, fresh random seed (or the forced fixed seed via the mapCreator hook).
	// For above-ground this restores the segment's recorded entry state (day-start or post-cave-exit).
	p2gz->segment_history->retry_segment();
}

void RaceMode::abort_run()
{
	Game::SingleGameSection* game = get_SGS();

	active            = false;
	finished          = false;
	abort_hold_frames = 0;
	pending_reset     = false;

	if (game) {
		game->flow_goto_title();
	}
}

void RaceMode::on_ending(bool is_complete)
{
	if (!active || finished) {
		return;
	}

	// Category gate: PoD stops on the debt ending; AT stops only on the all-treasures ending,
	// ignoring the debt ending that plays mid-AT-run.
	const bool match = (category == AT) ? is_complete : !is_complete;
	if (!match) {
		return;
	}

	capture_stats();
	final_is_complete = is_complete;
	p2gz->timer->pause(); // freeze the RTA readout at the stop frame
	finished = true;
}

void RaceMode::capture_stats()
{
	const u32 rta      = p2gz->timer->get_main_elapsed_ms();
	const u32 eff_load = load_ms + (was_loading ? cur_ms() - load_enter_ms : 0);

	final_rta_ms      = rta;
	final_igt_ms      = (rta > eff_load) ? rta - eff_load : 0;
	final_pikmin_lost = Game::DeathMgr::get_total(Game::DeathCounter::COD_All) + Game::DeathMgr::get_today(Game::DeathCounter::COD_All);
}

void RaceMode::notify_enemy_defeated()
{
	if (active && !finished) {
		enemies_defeated++;
	}
}

void RaceMode::notify_pikmin_thrown()
{
	if (active && !finished) {
		pikmin_thrown++;
	}
}

void RaceMode::update()
{
	if (!active) {
		return;
	}

	// Consume a deferred floor reset here (safe transit context, same as warp/menu actions)
	if (pending_reset) {
		pending_reset = false;
		do_reset();
	}

	// Accumulate loading time so IGT can subtract it
	const bool loading = in_load();
	if (loading && !was_loading) {
		was_loading   = true;
		load_enter_ms = cur_ms();
	} else if (!loading && was_loading) {
		was_loading = false;
		load_ms += cur_ms() - load_enter_ms;
	}

	// The crash-landing intro is only reproducible via a new-file restart. Once the run moves past it
	// (a cave is entered, or the day advances), normal warp-based retry is correct again.
	if (at_fresh_start && Game::gameSystem && (Game::gameSystem->mTimeMgr->mDayCount >= 1 || in_cave_play())) {
		at_fresh_start = false;
	}

	// Once the run is over and we've left the ending (now on the world map / title), unlock the mode
	if (finished && (in_world_map() || in_title_screen() || in_file_select())) {
		active   = false;
		finished = false;
		return;
	}

	// Abort: hold L+R+Z+Start for ~2s
	if (!finished && p2gz->controller && (p2gz->controller->getButton() & ABORT_COMBO_MASK) == ABORT_COMBO_MASK) {
		abort_hold_frames++;
		const u32 threshold = is_30_fps() ? 60 : 120;
		if (abort_hold_frames >= threshold) {
			abort_run();
		}
	} else {
		abort_hold_frames = 0;
	}
}

static inline bool in_ending_state()
{
	Game::SingleGameSection* sgs = get_SGS();
	if (!sgs || !sgs->getCurrState()) {
		return false;
	}
	return sgs->getCurrState()->getCurrStateID() == Game::SingleGame::SGS_Ending;
}

void RaceMode::draw_2d()
{
	if (!active) {
		return;
	}

	if (finished && in_ending_state()) {
		draw_summary();
		return;
	}

	draw_timers();
	draw_controls_hint();
}

void RaceMode::draw_timers()
{
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 16.0f;
	j2d.mGlyphHeight = 16.0f;
	JUtility::TColor color(255, 255, 255, 200);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	const u32 rta      = finished ? final_rta_ms : p2gz->timer->get_main_elapsed_ms();
	const u32 eff_load = load_ms + (was_loading ? cur_ms() - load_enter_ms : 0);
	const u32 igt      = finished ? final_igt_ms : ((rta > eff_load) ? rta - eff_load : 0);

	u32 m, s, t;
	ms_to_clock(rta, m, s, t);
	j2d.print(12.0f, 12.0f, "RTA %ld:%.2ld.%.1ld", m, s, t);
	ms_to_clock(igt, m, s, t);
	j2d.print(12.0f, 32.0f, "IGT %ld:%.2ld.%.1ld", m, s, t);

	if (reset_count > 0) {
		j2d.print(12.0f, 52.0f, "resets %ld", reset_count);
	}
}

void RaceMode::draw_controls_hint()
{
	// compact, unobtrusive reminder of the in-run controls
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 12.0f;
	j2d.mGlyphHeight = 12.0f;
	JUtility::TColor color(255, 255, 255, 120);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	const f32 z = System::getRenderModeHeight() - 16.0f;
	j2d.print(12.0f, z, "B+X+Start: reset   hold L+R+Z+Start: abort");
}

void RaceMode::draw_summary()
{
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 22.0f;
	j2d.mGlyphHeight = 22.0f;
	JUtility::TColor color(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	const f32 x = 80.0f;
	f32 z       = 120.0f;
	const f32 dz = 28.0f;

	j2d.print(x, z, final_is_complete ? "ALL TREASURES - RUN COMPLETE" : "DEBT PAID - RUN COMPLETE");
	z += dz * 1.5f;

	u32 m, s, t;
	ms_to_clock(final_rta_ms, m, s, t);
	j2d.print(x, z, "RTA   %ld:%.2ld.%.1ld", m, s, t);
	z += dz;
	ms_to_clock(final_igt_ms, m, s, t);
	j2d.print(x, z, "IGT   %ld:%.2ld.%.1ld", m, s, t);
	z += dz;
	j2d.print(x, z, "resets           %ld", reset_count);
	z += dz;
	j2d.print(x, z, "pikmin lost      %ld", final_pikmin_lost);
	z += dz;
	j2d.print(x, z, "enemies defeated %ld", enemies_defeated);
	z += dz;
	j2d.print(x, z, "pikmin thrown    %ld", pikmin_thrown);
}
