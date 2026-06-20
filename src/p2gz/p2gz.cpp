#include <p2gz/p2gz.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <p2gz/timer.h>
#include <p2gz/WaypointViewer.h>
#include <p2gz/warp.h>
#include <p2gz/DayEditor.h>
#include <p2gz/HeapBarToggle.h>
#include <p2gz/SprayEditor.h>
#include <p2gz/EnemyDebugInfo.h>
#include <p2gz/SquadEditor.h>
#include <p2gz/DismissPositions.h>
#include <p2gz/PokoEditor.h>
#include <p2gz/TreasureEditor.h>
#include <p2gz/Trainers.h>
#include <Game/Navi.h>
#include <P2JME/P2JME.h>
#include <IDelegate.h>
#include "Dolphin/rand.h"

#ifdef GZ_TEST
#include <p2gz/Test.h>
#endif

using namespace gz;

P2GZ* p2gz;

P2GZ::P2GZ()
{
	inited             = false;
	in_save_file       = true;
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	// Setup all our P2GZ menus/features here
	collision_viewer     = new CollisionViewer();
	controller           = new Controller(JUTGamePad::PORT_0);
	freecam              = new FreeCam();
	menu                 = new GZMenu();
	navi_tools           = new NaviTools();
	timer                = new Timer();
	waypoint_viewer      = new WaypointViewer();
	warp                 = new Warp();
	skippable_cutscenes  = new SkippableCutscenes();
	day_editor           = new DayEditor();
	heap_bar_toggle      = new HeapBarToggle();
	images               = new ImageMgr();
	skip_save            = new SkipSave();
	structure_editor     = new StructureEditor();
	spray_editor         = new SprayEditor();
	segment_history      = new SegmentHistory();
	enemy_debug_info     = new EnemyDebugInfo();
	treasure_debug_info  = new TreasureDebugInfo();
	cave_debug_info      = new CaveDebugInfo();
	generator_debug_info = new GeneratorDebugInfo();
	squad_editor         = new SquadEditor();
	preset_mgr           = new PresetMgr();
	cutscene_mgr         = new CutsceneMgr();
	dismiss_positions    = new DismissPositions();
	poko_editor          = new PokoEditor();
	ek_editor            = new EKEditor();
	treasure_editor      = new TreasureEditor();
	onion_editor         = new OnionEditor();
	localization_op      = new Localization();
	empress_trainer      = new EmpressTrainer();
	early_blues_trainer  = new EarlyBluesTrainer();
	drop_editor          = new DropEditor();
	navi_debug_info      = new NaviDebugInfo();
	card_data            = new P2GZCardData();
	race_mode            = new RaceMode();

#ifdef GZ_TEST
	test_runner = new test::TestRunner();
#endif

	prev_heap->becomeCurrentHeap();
}

void P2GZ::init()
{
	if (inited) {
		return;
	}

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	// Menu must come first since other inits might change menu options
	menu->init_menu();

	timer->init();
	structure_editor->init();
	warp->init();
	day_editor->init();
	images->init();
	spray_editor->init();
	squad_editor->init();
	onion_editor->init();
	cutscene_mgr->init();
	poko_editor->init();
	ek_editor->init();
	treasure_editor->init();
	localization_op->init_menu();
	preset_mgr->init();
	early_blues_trainer->init();
	race_mode->init();

#ifdef GZ_TEST
	test_runner->init();
#endif

	inited = true;
	prev_heap->becomeCurrentHeap();

	OSReport("==== Sys heap free/total after P2GZ init: %.2f KB / %.2f KB\n", sys->mSysHeap->getTotalFreeSize() / 1024.0f,
	         sys->mSysHeap->getHeapSize() / 1024.0f);
}

void P2GZ::update()
{
	if (!inited || !sys->mSysHeap) {
		return;
	}

#ifdef GZ_TEST
	test_runner->update();
#endif

	freecam->update();
	cutscene_mgr->update();
	segment_history->update();
	dismiss_positions->update();
	navi_tools->update();
	empress_trainer->update();
	early_blues_trainer->update();
	navi_debug_info->update();

	warp->update_lockout_frames();
	race_mode->update();

	// Menu must update last so button presses for menu interactions don't
	// inadvertantly do things in other systems on the same frame they're pressed.
	// NEW - we use the menu lock to prevent this issue for update calls outside of this function (such as graphical updates)
	menu->update();
}

// Anything that needs to appear on the screen in clip space should be drawn here.
void P2GZ::draw_2d()
{
	if (!inited) {
		return;
	}

#ifdef GZ_TEST
	test_runner->draw_2d();
#endif

	menu->draw();
	timer->draw();
	segment_history->draw_2d();
	empress_trainer->draw();
	early_blues_trainer->draw_status();
	navi_debug_info->draw();
	race_mode->draw_2d();
}

// Anything that needs to be drawn in 3D space should be drawn here.
void P2GZ::draw()
{
	if (!inited || !sys->mSysHeap) {
		return;
	}

	freecam->draw();
	enemy_debug_info->draw();
	treasure_debug_info->draw();
	cave_debug_info->draw();
	generator_debug_info->draw();
	structure_editor->draw();
	dismiss_positions->draw();
}

// Code to draw the version number on the title screen
void P2GZ::draw_version()
{
	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();

	j2d.mGlyphWidth  = 20.0f;
	j2d.mGlyphHeight = 20.0f;

	JUtility::TColor color = JUtility::TColor(255, 255, 255, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	// print version text in the bottom-center of screen
	// automagically determine where to print it so it's centered
	f32 width = j2d.getWidth("v.%s", P2GZ_VERSION);
	j2d.print((System::getRenderModeWidth() / 2) - (width / 2), 424.0f, "v.%s", P2GZ_VERSION);
}
