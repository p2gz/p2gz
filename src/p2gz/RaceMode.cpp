#include <p2gz/RaceMode.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Utility.h>
#include <p2gz/InputHelpers.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <System.h>
#include <ResetManager.h>
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

// Both 0 for now; tunable later if we want to penalise quick resetting
const f32 RaceMode::RESET_PENALTY_SET_SEED = 0.0f;
const f32 RaceMode::RESET_PENALTY_RANDOM   = 0.0f;

// abort/exit race mode combo: hold L + R + Z + Start
const u32 RaceMode::ABORT_COMBO_MASK = Controller::PRESS_L | Controller::PRESS_R | Controller::PRESS_Z | Controller::PRESS_START;

RaceMode::RaceMode()
{
	active                 = false;
	finished               = false;
	at_fresh_start         = false;
	category               = PoD;
	start_point            = SP_FreshFile;
	use_set_seed           = false;
	seed                   = 0;
	reset_count            = 0;
	load_ms                = 0;
	was_loading            = false;
	load_enter_ms          = 0;
	abort_hold_frames      = 0;
	pending_reset          = false;
	final_rta_ms           = 0;
	final_igt_ms           = 0;
	final_pikmin_lost      = 0;
	final_is_all_treasures = false;
	enemies_defeated       = 0;
	pikmin_thrown          = 0;
	category_opt           = nullptr;
	start_point_opt        = nullptr;
	region_opt             = nullptr;
}

u32 RaceMode::cur_ms()
{
	return (u32)OSTicksToMilliseconds(OSGetTime());
}

void RaceMode::ms_to_clock(u32 ms, u32& hours, u32& minutes, u32& seconds, u32& tenths)
{
	hours   = ms / (60 * 60 * 1000);
	minutes = (ms / (60 * 1000)) % 60;
	seconds = (ms / 1000) % 60;
	tenths  = (ms / 100) % 10;
}

void RaceMode::init()
{
	// populate the category + region radios
	category_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("race/category"));
	if (category_opt) {
		category_opt->options.clear();
		category_opt->options.push("PoD");
		category_opt->options.push("AT");
	}
	start_point_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("race/start point"));
	populate_start_point_options();
	region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("race/treasure region"));
	if (region_opt) {
		region_opt->options.clear();
		region_opt->options.push("US");
		region_opt->options.push("JP");
		region_opt->options.push("PAL");
	}
}

void RaceMode::set_category(size_t idx)
{
	category = (idx == 0) ? PoD : AT;
	// the preset start point is category-specific, so relabel it whenever the category changes
	populate_start_point_options();
}

void RaceMode::set_start_point(size_t idx)
{
	start_point = idx;
}

void RaceMode::populate_start_point_options()
{
	if (!start_point_opt) {
		return;
	}
	start_point_opt->options.clear();
	start_point_opt->options.push("fresh file");
	// index 1 keeps the same meaning (preset start) across categories; only the label differs
	start_point_opt->options.push(category == PoD ? "race file (SH)" : "wistful wild");
}

void RaceMode::set_seed(u32 seed_)
{
	use_set_seed = true;
	seed         = seed_;
}

void RaceMode::set_random_seed()
{
	use_set_seed = false;
}

void RaceMode::start_run()
{
	// lock in the run configuration
	active   = true;
	finished = false;
	// only a fresh-file start sits on the day-1 crash landing (where reset = redo fresh file, not retry segment)
	at_fresh_start    = (start_point == SP_FreshFile);
	reset_count       = 0;
	load_ms           = 0;
	was_loading       = false;
	load_enter_ms     = 0;
	abort_hold_frames = 0;
	pending_reset     = false;
	enemies_defeated  = 0;
	pikmin_thrown     = 0;

	// treasure region is bound straight into the region change stuff, so it'll auto-apply

	// close menu first, otherwise we mess up the timer lol
	p2gz->menu->close();

	// start RTA (main) and IGT (load-removed) timers
	p2gz->timer->set_enabled(true);
	p2gz->timer->set_sub_timer_enabled(false);
	p2gz->timer->reset_main_timer();

	// START THE RUN
	if (start_point == SP_PresetStart) {
		begin_from_preset();
	} else {
		begin_fresh_file();
	}
}

void RaceMode::begin_fresh_file()
{
	Game::SingleGameSection* game = get_SGS();
	if (!game) {
		return;
	}

	// clear game so we get a fresh restart
	Game::gameSystem->resetFlag(Game::GAMESYS_IsGameWorldActive);
	if (Game::moviePlayer) {
		Game::moviePlayer->reset();
	}
	if (particle2dMgr) {
		particle2dMgr->killAll();
	}

	// pretend we're starting a new file, so the crash landing cutscene queues up
	Game::playData->reset();
	Game::playData->mLoadType             = Game::STORYSAVE_NewFile;
	Game::gameSystem->mTimeMgr->mDayCount = 0;

	Game::SingleGame::MovieArg arg(Game::THPPlayer::OPENING_GameStart);
	game->mFsm->transit(game, Game::SingleGame::SGS_Movie, &arg);
}

void RaceMode::begin_from_preset()
{
	// warp to the common "race file" start point for this category
	// (SH1 for PoD, WW for AT)
	WarpDestination dest;
	if (category == PoD) {
		// PoD = SH1 race file
		dest.area     = COURSE_AW;
		dest.cave     = 4; // SH
		dest.sublevel = 0;
	} else {
		// AT = WW
		dest.area     = COURSE_WW;
		dest.cave     = 0; // above ground
		dest.sublevel = 0;
	}

	PresetPreview* preset = p2gz->preset_mgr->suggested_preset(dest, category);
	if (!preset) {
		// shouldn't happen (the route always has these presets), but don't warp with no preset
		OSReport("[P2GZ] race mode: no preset for start point, falling back to fresh file\n");
		at_fresh_start = true;
		begin_fresh_file();
		return;
	}

	p2gz->warp->set_dest(dest);
	p2gz->warp->set_preset(preset, PS_Suggested);
	p2gz->warp->do_warp();
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
	// can't reset if you're not in gameplay
	if (!in_cave_play() && !in_above_ground_play()) {
		return;
	}

	// track resets for metrics/display
	reset_count++;

	const f32 penalty = use_set_seed ? RESET_PENALTY_SET_SEED : RESET_PENALTY_RANDOM;
	if (penalty > 0.0f) {
		p2gz->timer->offset_main_timer(penalty); // also penalises IGT, since IGT = RTA - loads
	}

	// day 1 has to be difficult every single time. account for it if someone resets the day
	if (at_fresh_start) {
		// zero out all our metrics, so they stay consistent with the playData getting wiped as well
		enemies_defeated = 0;
		pikmin_thrown    = 0;
		begin_fresh_file();
		return;
	}

	// if not day 1, just use our retry segment functionality
	p2gz->segment_history->retry_segment();
}

void RaceMode::abort_run()
{
	// clear all race state first so nothing lingers during the fadeout frames before the reboot
	active            = false;
	finished          = false;
	at_fresh_start    = false;
	abort_hold_frames = 0;
	pending_reset     = false;

	// exit race mode by triggering a (proper) soft reset
	if (sys && sys->mResetMgr) {
		sys->mResetMgr->trigger_soft_reset();
	}
}

void RaceMode::on_ending(bool is_all_treasures)
{
	if (!active || finished) {
		return;
	}

	// make sure our ending matches the category we set when we started
	const bool match = (category == AT) ? is_all_treasures : !is_all_treasures;
	if (!match) {
		return;
	}

	capture_stats();
	final_is_all_treasures = is_all_treasures;
	p2gz->timer->pause(); // stop the timer!
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

	// if we have a reset pending, do it here so we're in control of what happens
	if (pending_reset) {
		pending_reset = false;
		do_reset();
	}

	// accumulate loading time so IGT can subtract it
	const bool loading = in_load();
	if (loading && !was_loading) {
		was_loading   = true;
		load_enter_ms = cur_ms();
	} else if (!loading && was_loading) {
		was_loading = false;
		load_ms += cur_ms() - load_enter_ms;
	}

	// reset our annoying day 1 trigger once we clear the hurdle
	if (at_fresh_start && Game::gameSystem && Game::gameSystem->mTimeMgr->mDayCount >= 1) {
		at_fresh_start = false;
	}

	// once the run is over and we've left the ending, unlock the mode
	if (finished && (in_world_map() || in_title_screen() || in_file_select())) {
		active   = false;
		finished = false;
		return;
	}

	// abort/exit race mode (and unlock gz menu) = hold L+R+Z+Start for ~2s
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
	// show button combos in the menu while it's open (before the race starts)
	if (p2gz->menu && p2gz->menu->is_active_menu("race")) {
		draw_menu_controls();
	}

	if (!active) {
		return;
	}

	if (finished && in_ending_state()) {
		// draw our nice stats overlay
		draw_summary();
		return;
	}

	draw_timers();
	draw_controls_hint();
}

void RaceMode::draw_on_top()
{
	// the normal draw_2d pass ends up underneath the end-of-day result / cutscene 2D screens
	if (!active) {
		return;
	}
	if (finished && in_ending_state()) {
		draw_summary();
		return;
	}
	draw_timers();
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

	// the font isn't monospaced, so align RTA/IGT on their decimal point
	const bool show_hours = rta >= 60u * 60u * 1000u;
	const f32 decimal_x   = 12.0f + j2d.getWidth(show_hours ? "RTA 0:00:00" : "RTA 00:00");

	draw_timer_line(j2d, decimal_x, 12.0f, "RTA", rta);
	draw_timer_line(j2d, decimal_x, 32.0f, "IGT", igt);

	if (reset_count > 0) {
		j2d.print(12.0f, 52.0f, "resets %ld", reset_count);
	}
}

void RaceMode::draw_timer_line(J2DPrint& j2d, f32 decimal_x, f32 z, const char* label, u32 ms)
{
	u32 h, m, s, t;
	ms_to_clock(ms, h, m, s, t);

	// label stays left-aligned at a fixed x - only the numeric part is right-aligned to decimal_x
	j2d.print(12.0f, z, label);
	if (h > 0) { // once past an hour, show H:MM:SS; otherwise M:SS
		j2d.print(decimal_x - j2d.getWidth("%ld:%.2ld:%.2ld", h, m, s), z, "%ld:%.2ld:%.2ld", h, m, s);
	} else {
		j2d.print(decimal_x - j2d.getWidth("%ld:%.2ld", m, s), z, "%ld:%.2ld", m, s);
	}
	j2d.print(decimal_x, z, ".%.1ld", t);
}

void RaceMode::draw_controls_hint()
{
	// we lock the menu, so we should let players know how to like. get out of that.
	// future improvement could be making this toggleable somehow, so experienced players can turn it off
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 12.0f;
	j2d.mGlyphHeight = 12.0f;
	JUtility::TColor color(255, 255, 255, 120);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	const f32 z = System::getRenderModeHeight() - 16.0f;
	j2d.print(12.0f, z, "B+X+Start: reset floor/segment   hold L+R+Z+Start: exit race mode");
}

// draw one right-aligned row: "<msg> [btn][btn]... + Start", anchored to the right screen edge
static void draw_menu_combo(J2DPrint& j2d, f32 glyph_size, f32& z, const char* msg, const char* const* btn_imgs, int num_btns)
{
	const f32 img_dim    = p2gz->images->height(); // menu images are square (width == height)
	const f32 spacing    = p2gz->images->spacing();
	const char* suffix   = " + Start";
	const f32 text_basez = z + (glyph_size / 2.0f) - 4.0f;

	// pre-compute the total row width so we can right-align it
	j2d.initiate();
	const f32 msg_w   = j2d.getWidth("%s ", msg);
	const f32 sfx_w   = j2d.getWidth("%s", suffix);
	const f32 total_w = msg_w + num_btns * (img_dim + spacing) + sfx_w;

	f32 x = System::getRenderModeWidth() - 16.0f - total_w;

	// message, then the button glyphs, then the "+ Start" suffix
	x += j2d.print(x, text_basez, "%s ", msg);
	for (int i = 0; i < num_btns; i++) {
		x += p2gz->images->draw(btn_imgs[i], x, z - img_dim / 2.0f);
		x += spacing;
	}
	j2d.initiate(); // re-initialise the text printer after image draws to prevent the GPU dying
	x += j2d.print(x, text_basez, "%s", suffix);

	z += img_dim + 4.0f;
}

void RaceMode::draw_menu_controls()
{
	const f32 glyph_size = 18.0f;
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = glyph_size;
	j2d.mGlyphHeight = glyph_size;
	JUtility::TColor color(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	f32 z = 280.0f;

	static const char* reset_btns[] = { "b_btn", "x_btn" };
	draw_menu_combo(j2d, glyph_size, z, "reset floor/segment", reset_btns, 2);

	static const char* abort_btns[] = { "l_btn", "r_btn", "z_btn" };
	draw_menu_combo(j2d, glyph_size, z, "hold to exit race mode", abort_btns, 3);
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

	const f32 x  = 80.0f;
	f32 z        = 120.0f;
	const f32 dz = 28.0f;

	j2d.print(x, z, final_is_all_treasures ? "ALL TREASURES - RUN COMPLETE" : "PAY OFF DEBT - RUN COMPLETE");
	z += dz * 1.5f;

	u32 h, m, s, t;
	ms_to_clock(final_rta_ms, h, m, s, t);
	if (h > 0) {
		j2d.print(x, z, "RTA   %ld:%.2ld:%.2ld.%.1ld", h, m, s, t);
	} else {
		j2d.print(x, z, "RTA   %ld:%.2ld.%.1ld", m, s, t);
	}
	z += dz;
	ms_to_clock(final_igt_ms, h, m, s, t);
	if (h > 0) {
		j2d.print(x, z, "IGT   %ld:%.2ld:%.2ld.%.1ld", h, m, s, t);
	} else {
		j2d.print(x, z, "IGT   %ld:%.2ld.%.1ld", m, s, t);
	}
	z += dz;
	j2d.print(x, z, "resets           %ld", reset_count);
	z += dz;
	j2d.print(x, z, "pikmin lost      %ld", final_pikmin_lost);
	z += dz;
	j2d.print(x, z, "enemies defeated %ld", enemies_defeated);
	z += dz;
	j2d.print(x, z, "pikmin thrown    %ld", pikmin_thrown);
}
