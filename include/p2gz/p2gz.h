#ifndef _P2GZ_H
#define _P2GZ_H

#include <p2gz/gzmenu.h>
#include <p2gz/CollisionViewer.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <p2gz/timer.h>
#include <p2gz/WaypointViewer.h>
#include <p2gz/SprayEditor.h>
#include <p2gz/warp.h>
#include <p2gz/SkippableCS.h>
#include <p2gz/EnemyDebugInfo.h>
#include <p2gz/TreasureDebugInfo.h>
#include <p2gz/DayEditor.h>
#include <p2gz/HeapBarToggle.h>
#include <p2gz/SkipSave.h>
#include <p2gz/Localization.h>
#include <p2gz/SquadEditor.h>
#include <p2gz/StructureEditor.h>
#include <p2gz/SegmentHistory.h>
#include <p2gz/EnemyDebugInfo.h>
#include <p2gz/Preset.h>
#include <p2gz/DismissPositions.h>
#include <p2gz/PokoEditor.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/EKEditor.h>
#include <p2gz/CaveDebugInfo.h>
#include <p2gz/TreasureEditor.h>
#include <p2gz/GeneratorDebugInfo.h>
#include <p2gz/Test.h>
#include <p2gz/Trainers.h>
#include <p2gz/OnionEditor.h>
#include <p2gz/DropEditor.h>

/*!!! VERSION NUMBER - TO BE UPDATED EACH RELEASE !!!*/
#define P2GZ_VERSION "alpha5"

struct P2GZ {
public:
	P2GZ();
	~P2GZ();

	void init();
	void update();
	void draw_2d();
	void draw();

	static void draw_version();

	// our own persistent controller so we don't crash the game on new file starting (don't ask)
	Controller* controller;

	// gz specifics
	gz::GZMenu* menu;
	gz::CollisionViewer* collision_viewer;
	gz::FreeCam* freecam;
	gz::NaviTools* navi_tools;
	gz::Timer* timer;
	gz::WaypointViewer* waypoint_viewer;
	gz::Warp* warp;
	gz::SkippableCutscenes* skippable_cutscenes;
	gz::EnemyDebugInfo* enemy_debug_info;
	gz::TreasureDebugInfo* treasure_debug_info;
	gz::GeneratorDebugInfo* generator_debug_info;
	gz::DayEditor* day_editor;
	gz::HeapBarToggle* heap_bar_toggle;
	gz::ImageMgr* images;
	gz::SkipSave* skip_save;
	gz::SquadEditor* squad_editor;
	gz::StructureEditor* structure_editor;
	gz::SprayEditor* spray_editor;
	gz::SegmentHistory* segment_history;
	gz::PresetMgr* preset_mgr;
	gz::CutsceneMgr* cutscene_mgr;
	gz::DismissPositions* dismiss_positions;
	gz::PokoEditor* poko_editor;
	gz::EKEditor* ek_editor;
	gz::CaveDebugInfo* cave_debug_info;
	gz::TreasureEditor* treasure_editor;
	gz::Localization* localization_op;
	gz::EmpressTrainer* empress_trainer;
	gz::OnionEditor* onion_editor;
	gz::DropEditor* drop_editor;

	gz::test::TestRunner* test_runner;

	bool in_save_file;

private:
	bool inited;
};

// global instance
extern P2GZ* p2gz;

#endif
