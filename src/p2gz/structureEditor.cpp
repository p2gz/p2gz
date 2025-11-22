#include <p2gz/StructureEditor.h>
#include <p2gz/gzmenu.h>
#include <p2gz/p2gz.h>
#include <Game/Entities/ItemGate.h>
#include <Game/Entities/ItemBarrel.h>
#include <Game/Entities/ItemHole.h>
#include <P2JME/P2JME.h>
#include <PSM/WorkItem.h>
#include <types.h>

using namespace gz;

#define STRUCT_SEARCH_RADIUS     (5.0f)
#define STRUCT_DEBUG_RENDER_DIST (512.0f)

/* ------------------------------------------------- */
/* --------------------- GATES --------------------- */
/* ------------------------------------------------- */

static const StructureEditor::NameCoordinateMap GATE_COORD_TO_NAME[] = {
	// VoR
	StructureEditor::NameCoordinateMap(-591.0f, 1427.0f, "EC gate"),
	StructureEditor::NameCoordinateMap(369.0f, 1565.0f, "water gate"),
	StructureEditor::NameCoordinateMap(798.0f, -520.0f, "SCx gate"),
	// AW
	StructureEditor::NameCoordinateMap(-503.0f, 1639.0f, "black gate"),
	StructureEditor::NameCoordinateMap(450.0f, 1905.0f, "plug gate"),
	StructureEditor::NameCoordinateMap(-1463.0f, 1767.0f, "globe gate"),
	StructureEditor::NameCoordinateMap(460.0f, 994.0f, "WFG gate"),
	StructureEditor::NameCoordinateMap(470.0f, 1575.0f, "BK gate"),
	StructureEditor::NameCoordinateMap(-375.0f, 375.0f, "blue onion gate"),
	StructureEditor::NameCoordinateMap(-820.0f, 4280.0f, "SH gate"),
	// PP
	StructureEditor::NameCoordinateMap(-1180.0f, 980.0f, "onion replica gate"),
	StructureEditor::NameCoordinateMap(-1510.0f, -2550.0f, "SMC area gate"),
	StructureEditor::NameCoordinateMap(-390.0f, -1290.0f, "yellow onion gate"),
	StructureEditor::NameCoordinateMap(-1162.0f, -2375.0f, "GK gate"),
	StructureEditor::NameCoordinateMap(-1612.0f, -1798.0f, "zirconium rotor gate"),
	StructureEditor::NameCoordinateMap(1390.0f, 1083.0f, "SR gate"),
	// WW
	StructureEditor::NameCoordinateMap(-350.0f, 1025.0f, "DD gate"),
	StructureEditor::NameCoordinateMap(-1220.0f, 727.0f, "bridge gate"),
	StructureEditor::NameCoordinateMap(-2819.0f, 2463.0f, "HoH gate"),
	StructureEditor::NameCoordinateMap(-3514.0f, 1153.0f, "crawmad gate"),
};

static const size_t NUM_GATE_NAMES = ARRAY_SIZE(GATE_COORD_TO_NAME); // 20

/* ------------------------------------------------- */
/* -------------------- BRIDGES -------------------- */
/* ------------------------------------------------- */

static const StructureEditor::NameCoordinateMap BRIDGE_COORD_TO_NAME[] = {
	// VoR
	StructureEditor::NameCoordinateMap(540.0f, 775.0f, "water bridge"),
	StructureEditor::NameCoordinateMap(-316.8f, -1664.5f, "FC bridge"),
	// AW
	StructureEditor::NameCoordinateMap(-1250.0f, 2260.0f, "globe bridge"),
	StructureEditor::NameCoordinateMap(-1517.3f, 3529.5f, "air brake bridge to globe"),
	StructureEditor::NameCoordinateMap(-1353.2f, 3760.6f, "air brake bridge to SH"),
	// PP
	StructureEditor::NameCoordinateMap(326.0f, -759.0f, "bitter plant bridge"),
	StructureEditor::NameCoordinateMap(1431.0f, 404.0f, "SR bridge"),
	StructureEditor::NameCoordinateMap(-2159.0f, -857.0f, "CoS bridge"),
	// WW
	StructureEditor::NameCoordinateMap(-1479.3f, 558.3f, "DD bridge"),
};

static const size_t NUM_BRIDGE_NAMES = ARRAY_SIZE(BRIDGE_COORD_TO_NAME); // 9

/* ------------------------------------------------- */
/* --------------------- PLUGS --------------------- */
/* ------------------------------------------------- */

static const StructureEditor::NameCoordinateMap PLUG_COORD_TO_NAME[] = {
	// VoR
	StructureEditor::NameCoordinateMap(800.0f, 1380.0f, "bridge plug"),
	// AW
	StructureEditor::NameCoordinateMap(380.5f, 1948.9f, "BK plug"),
	// PP
	StructureEditor::NameCoordinateMap(1408.7f, 1625.2f, "SR plug"),
	// WW
	StructureEditor::NameCoordinateMap(-2773.3f, 1328.5f, "plants plug"),
};

static const StructureEditor::WaterBoxMap PLUG_NAME_TO_WATERBOX[] = {
	// VoR
	StructureEditor::WaterBoxMap("bridge plug", -140.000000f, -60.000000f, 1250.000000f, 960.000000f, 40.000000f, 2150.000000f),
	// AW
	StructureEditor::WaterBoxMap("BK plug", 245.000000f, -172.500000f, 1774.195313f, 595.000000f, -72.500000f, 2124.195313f),
	// PP
	StructureEditor::WaterBoxMap("SR plug", 90.000000f, -140.000000f, -200.000000f, 1790.000000f, -40.000000f, 2200.000000f),
	// WW
	StructureEditor::WaterBoxMap("plants plug", -3700.000000f, -110.000000f, 850.000000f, -2500.000000f, -10.000000f, 1650.000000f),
};

static const size_t NUM_PLUG_NAMES = ARRAY_SIZE(PLUG_COORD_TO_NAME); // 4

/* ------------------------------------------------- */
/* --------------------- BAGS ---------------------- */
/* ------------------------------------------------- */

static const StructureEditor::NameCoordinateMap BAG_COORD_TO_NAME[] = {
	// VoR
	StructureEditor::NameCoordinateMap(-910.7f, 2769.2f, "landing area bag (15)"),
	StructureEditor::NameCoordinateMap(-1150.0f, 2455.0f, "hubcap bag (35)"),
	// AW
	StructureEditor::NameCoordinateMap(-395.0f, 1115.0f, "WFG bag (200)"),
};

static const size_t NUM_BAG_NAMES = ARRAY_SIZE(BAG_COORD_TO_NAME); // 3

void StructureEditor::init()
{
	gate_menu   = static_cast<ListMenu*>(p2gz->menu->get_option("level/gates")->get_sub_menu());
	bridge_menu = static_cast<ListMenu*>(p2gz->menu->get_option("level/bridges")->get_sub_menu());
	plug_menu   = static_cast<ListMenu*>(p2gz->menu->get_option("level/plugs")->get_sub_menu());
	bag_menu    = static_cast<ListMenu*>(p2gz->menu->get_option("level/bags")->get_sub_menu());
}

void StructureEditor::add_gate(Game::ItemGate* gate)
{
	bool generated               = false;
	GateWrapper* gate_wrapper    = new GateWrapper();
	gate_wrapper->gate           = gate;
	gate_wrapper->name           = get_gate_name(gate->mPosition.x, gate->mPosition.z, generated);
	gate_wrapper->generated_name = generated;
	gates.push(gate_wrapper);

	// clang-format off
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	gate_menu->push(new OpenSubMenuOption(gate_wrapper->name, (new ListMenu())
	    ->push(new RangeMenuOption("segments remaining", 0, 3, 3 - gate->mSegmentsDown, RangeMenuOption::CAP, new Delegate1<GateWrapper, s32>(gates[gates.len()-1], &GateWrapper::set_gate_segments)))
		->push(new FloatRangeMenuOption("segment health", 0.0f, gate->mMaxSegmentHealth, gate->mCurrentSegmentHealth, new Delegate1<GateWrapper, f32>(gates[gates.len()-1], &GateWrapper::set_gate_segment_health)))
    ));
	prev_heap->becomeCurrentHeap();
	// clang-format on
}

void StructureEditor::clear_gates()
{
	DELETE_ALL(gates);
	if (gate_menu) {
		gate_menu->clear();
	}
}

const char* StructureEditor::get_gate_name(f32 x, f32 z, bool& generated)
{
	for (size_t i = 0; i < NUM_GATE_NAMES; i++) {
		NameCoordinateMap map = GATE_COORD_TO_NAME[i];
		if ((absF(map.x - x) < STRUCT_SEARCH_RADIUS) && (absF(map.z - z) < STRUCT_SEARCH_RADIUS)) {
			return map.name;
		}
	}

	char* name = new char[8];
	sprintf(name, "gate %d", gates.len());
	generated = true;
	return name;
}

void StructureEditor::set_gate_stages_left(const char* name, int stages_left)
{
	for (size_t i = 0; i < gates.len(); i++) {
		GateWrapper* gate = gates[i];
		if (strcmp(name, gate->name) == 0) {
			gate->set_gate_segments(stages_left);
			return;
		}
	}
}

void StructureEditor::GateWrapper::set_gate_segments(s32 segments)
{
	if (segments == 0) {
		gate->mSegmentsDown         = 2;
		gate->mCurrentSegmentHealth = -1.0f;
		gate->mFsm->transit(gate, GATESTATE_Damaged, nullptr);
	} else {
		gate->mSegmentsDown         = 3 - segments;
		gate->mCurrentSegmentHealth = gate->mMaxSegmentHealth;
		gate->mWayPoint->setOpen(false);
		gate->setAlive(true);
		gate->mCentrePlatInstance->setCollision(true);
		gate->mFsm->transit(gate, GATESTATE_Damaged, nullptr);
	}
}

void StructureEditor::GateWrapper::set_gate_segment_health(f32 health)
{
	gate->mCurrentSegmentHealth = health;
}

void StructureEditor::sync_gates()
{
	for (size_t i = 0; i < gates.len(); i++) {
		// sanity check to only update gates we have actual info for
		if (!gates[i] || !gates[i]->name || !gates[i]->gate) {
			continue;
		}

		ListMenu* gate_submenu = static_cast<ListMenu*>(gate_menu->get_option(gates[i]->name)->get_sub_menu());
		if (gate_submenu) {
			static_cast<RangeMenuOption*>(gate_submenu->get_option("segments remaining"))->set_selection(3 - gates[i]->gate->mSegmentsDown);
			static_cast<FloatRangeMenuOption*>(gate_submenu->get_option("segment health"))
			    ->set_selection(gates[i]->gate->mCurrentSegmentHealth);
		}
	}
}

void StructureEditor::add_bridge(Game::ItemBridge::Item* bridge)
{
	BridgeWrapper* wrapper = new BridgeWrapper();
	wrapper->bridge        = bridge;
	wrapper->name          = get_bridge_name(bridge->mPosition.x, bridge->mPosition.z);
	bridges.push(wrapper);

	Game::ItemBridge::Mgr* mgr = Game::ItemBridge::mgr;

	if (!mgr) {
		return;
	}

	// clang-format off
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	bridge_menu->push(new OpenSubMenuOption(wrapper->name, (new ListMenu())
	    ->push(new RangeMenuOption("segments remaining", 0, bridge->mStageCount, bridge->mStageCount - bridge->mCurrStageIdx, RangeMenuOption::CAP, new Delegate1<BridgeWrapper, s32>(bridges[bridges.len()-1], &BridgeWrapper::set_bridge_segments)))
		->push(new FloatRangeMenuOption("segment health", 0.0f, mgr->mParms->mBridgeParms.mHealth(), bridge->getCurrentStageHealth(), new Delegate1<BridgeWrapper, f32>(bridges[bridges.len()-1], &BridgeWrapper::set_bridge_segment_health)))
		->push(new ToggleMenuOption("bridge glitch", true, new Delegate1<BridgeWrapper, bool>(bridges[bridges.len()-1], &BridgeWrapper::set_bridge_glitch)))
    ));
	prev_heap->becomeCurrentHeap();
	// clang-format on
}

void StructureEditor::clear_bridges()
{
	DELETE_ALL(bridges);
	if (bridge_menu) {
		bridge_menu->clear();
	}
}

const char* StructureEditor::get_bridge_name(f32 x, f32 z)
{
	for (size_t i = 0; i < NUM_BRIDGE_NAMES; i++) {
		NameCoordinateMap map = BRIDGE_COORD_TO_NAME[i];
		if ((absF(map.x - x) < STRUCT_SEARCH_RADIUS) && (absF(map.z - z) < STRUCT_SEARCH_RADIUS)) {
			return map.name;
		}
	}

	GZASSERTLINE(false); // all bridges should be accounted for
}

void StructureEditor::set_bridge_stages_left(const char* name, int stages_left)
{
	for (size_t i = 0; i < bridges.len(); i++) {
		BridgeWrapper* bridge = bridges[i];
		if (strcmp(name, bridge->name) == 0) {
			bridge->set_bridge_segments(stages_left);
			return;
		}
	}
}

void StructureEditor::BridgeWrapper::set_bridge_segments(s32 segments)
{
	if (segments == 0) {
		bridge->setCurrStage(bridge->mStageCount);
		bridge->setAlive(false);
		PSSystem::spSysIF->playSystemSe(PSSE_SY_WORK_FINISH, 0);

		if (bridge->mBridgeWP && bridge->mEndWP) {
			bridge->mBridgeWP->setOpen(true);
			bridge->mEndWP->setOpen(true);
			bridge->mBridgeWP->setWater(false);
			bridge->mEndWP->setWater(false);
		}

		bridge->mSoundEvent.finish();
		static_cast<PSM::WorkItem*>(bridge->mSoundObj)->eventFinish();
	} else {
		for (int i = 0; i < bridge->mStageCount - segments; i++) {
			if (bridge->mIsGlitched) {
				bridge->mStageHealths[i] = 0.0f;
			} else {
				bridge->mStageHealths[i] = Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth();
			}
		}
		bridge->mStageHealths[bridge->mStageCount - segments] = Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth();
		bridge->setCurrStage(bridge->mStageCount - segments);
		bridge->setAlive(true);

		if (bridge->mBridgeWP && bridge->mEndWP) {
			bridge->mBridgeWP->setOpen(false);
			bridge->mEndWP->setOpen(false);
			bridge->mBridgeWP->setWater(false);
			bridge->mEndWP->setWater(false);
		}
	}

	ListMenu* bridge_submenu = static_cast<ListMenu*>(p2gz->structure_editor->bridge_menu->get_option(name)->get_sub_menu());
	static_cast<FloatRangeMenuOption*>(bridge_submenu->get_option("segment health"))->set_selection(bridge->getCurrentStageHealth());
}

void StructureEditor::BridgeWrapper::set_bridge_segment_health(f32 health)
{
	bridge->mStageHealths[bridge->mCurrStageIdx] = health;
}

void StructureEditor::BridgeWrapper::set_bridge_glitch(bool glitched)
{
	bridge->mIsGlitched = glitched;
	// set all remaining stage healths to 0 (glitched) or max (not glitched)
	for (int i = bridge->mCurrStageIdx + 1; i < bridge->mStageCount; i++) {
		if (glitched) {
			bridge->mStageHealths[i] = 0.0f;
		} else {
			bridge->mStageHealths[i] = Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth();
		}
	}

	ListMenu* bridge_submenu = static_cast<ListMenu*>(p2gz->structure_editor->bridge_menu->get_option(name)->get_sub_menu());
	static_cast<FloatRangeMenuOption*>(bridge_submenu->get_option("segment health"))->set_selection(bridge->getCurrentStageHealth());
}

void StructureEditor::sync_bridges()
{
	for (size_t i = 0; i < bridges.len(); i++) {
		// sanity check to only update bridges we have actual info for
		if (!bridges[i] || !bridges[i]->name || !bridges[i]->bridge) {
			continue;
		}

		ListMenu* bridge_submenu = static_cast<ListMenu*>(bridge_menu->get_option(bridges[i]->name)->get_sub_menu());
		if (bridge_submenu) {
			static_cast<RangeMenuOption*>(bridge_submenu->get_option("segments remaining"))
			    ->set_selection(bridges[i]->bridge->mStageCount - bridges[i]->bridge->mCurrStageIdx);
			static_cast<FloatRangeMenuOption*>(bridge_submenu->get_option("segment health"))
			    ->set_selection(bridges[i]->bridge->getCurrentStageHealth());
			static_cast<ToggleMenuOption*>(bridge_submenu->get_option("bridge glitch"))->set_selection(bridges[i]->bridge->mIsGlitched);
		}
	}
}

void StructureEditor::add_plug(Game::ItemBarrel::Item* plug)
{
	bool generated        = false;
	const char* plug_name = get_plug_name(plug->mPosition.x, plug->mPosition.z, generated);
	// check if we already have a menu item for this plug
	for (int i = 0; i < plugs.len(); i++) {
		if (strcmp(plug_name, plugs[i]->name) == 0) {
			return;
		}

		// we only ever have one plug per floor, so just check for the only name it could have
		if (in_cave_play() && (strcmp("plug 0", plugs[i]->name) == 0)) {
			delete plug_name;
			return;
		}
	}

	PlugWrapper* wrapper    = new PlugWrapper();
	wrapper->plug           = plug;
	wrapper->name           = plug_name;
	wrapper->pos            = plug->mPosition;
	wrapper->generated_name = generated;
	plugs.push(wrapper);

	Game::ItemBarrel::Mgr* mgr = Game::ItemBarrel::mgr;
	if (!mgr) {
		return;
	}

	// clang-format off
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	plug_menu->push(new OpenSubMenuOption(wrapper->name, (new ListMenu())
		->push(new ToggleMenuOption("plug alive", true, new Delegate1<PlugWrapper, bool>(plugs[plugs.len()-1], &PlugWrapper::set_plug_state)))
		->push(new FloatRangeMenuOption("plug health", 0.0f, mgr->mParms->mBarrelParms.mHealth(), plug->mHealth, new Delegate1<PlugWrapper, f32>(plugs[plugs.len()-1], &PlugWrapper::set_plug_health)))
    ));
	prev_heap->becomeCurrentHeap();
	// clang-format on
}

void StructureEditor::set_plug_destroyed(bool destroyed)
{
	if (plugs.len() >= 1) {
		plugs[0]->set_plug_state(!destroyed);
	}
}

void StructureEditor::clear_plugs()
{
	DELETE_ALL(plugs);
	if (plug_menu) {
		plug_menu->clear();
	}
}

const char* StructureEditor::get_plug_name(f32 x, f32 z, bool& generated)
{
	for (size_t i = 0; i < NUM_PLUG_NAMES; i++) {
		NameCoordinateMap map = PLUG_COORD_TO_NAME[i];
		if ((absF(map.x - x) < STRUCT_SEARCH_RADIUS) && (absF(map.z - z) < STRUCT_SEARCH_RADIUS)) {
			return map.name;
		}
	}

	char* name = new char[8];
	sprintf(name, "plug %d", plugs.len());
	generated = true;
	return name;
}

void StructureEditor::PlugWrapper::set_plug_health(f32 health)
{
	if (plug) {
		plug->mHealth = health;
	}
}

void StructureEditor::PlugWrapper::set_plug_state(bool alive)
{
	if (alive) {
		plug->mFsm->transit(plug, Game::ItemBarrel::BARREL_Normal, nullptr);
		plug->setAlive(true);
		plug->createBarrel();
		plug->mAnimSpeed         = 0.0f;
		plug->mAnimator.mAnimMgr = Game::ItemBarrel::mgr->mAnimMgr;
		plug->mAnimator.startAnim(1, plug);
		plug->mStoredDamage = 0.0f;
		plug->setPosition(pos, false);

		for (size_t i = 0; i < NUM_PLUG_NAMES; i++) {
			WaterBoxMap map = PLUG_NAME_TO_WATERBOX[i];
			if (strcmp(map.name, name) != 0) {
				continue;
			}
			if (plug->mWaterbox) {
				plug->mWaterbox->mBounds.mMin   = map.min_bounds;
				plug->mWaterbox->mBounds.mMax   = map.max_bounds;
				plug->mWaterbox->mLoweredAmount = 0.0f;
				plug->mWaterbox->mState         = Game::AABBWaterBox::WaterBox_Active;
			}
		}

		if (in_cave_play()) {
			// check if there's a hole to cover with this plug
			Iterator<Game::BaseItem> iter(Game::ItemHole::mgr);
			CI_LOOP(iter)
			{
				Game::ItemHole::Item* hole = static_cast<Game::ItemHole::Item*>(*iter);
				if (hole->isAlive()) {
					Vector3f sep = plug->getPosition() - hole->getPosition();
					Vector2f sep2D(sep.x, sep.z);
					if (sep2D.length() < 40.0f) {
						hole->mBarrel = plug;
						break;
					}
				}
			}
		}
		plug->mSkipDeathEfx = false;

		ListMenu* plug_submenu = static_cast<ListMenu*>(p2gz->structure_editor->plug_menu->get_option(name)->get_sub_menu());
		static_cast<FloatRangeMenuOption*>(plug_submenu->get_option("plug health"))->set_selection(plug->mHealth);

	} else {
		// if we're alive, kill the plug.
		if (plug) {
			plug->mSkipDeathEfx    = true;
			ListMenu* plug_submenu = static_cast<ListMenu*>(p2gz->structure_editor->plug_menu->get_option(name)->get_sub_menu());
			static_cast<FloatRangeMenuOption*>(plug_submenu->get_option("plug health"))->set_selection(0.0f);
			if (plug->getCurrState()->getCurrStateID() != Game::ItemBarrel::BARREL_Dead) {
				plug->mFsm->transit(plug, Game::ItemBarrel::BARREL_Dead, nullptr);
			}
		}
	}
}

void StructureEditor::sync_plugs()
{
	for (size_t i = 0; i < plugs.len(); i++) {
		// sanity check to only update plugs we have actual info for
		if (!plugs[i] || !plugs[i]->name || !plugs[i]->plug) {
			continue;
		}

		ListMenu* plug_submenu = static_cast<ListMenu*>(plug_menu->get_option(plugs[i]->name)->get_sub_menu());
		if (plug_submenu) {
			static_cast<ToggleMenuOption*>(plug_submenu->get_option("plug alive"))->set_selection(plugs[i]->plug->isAlive());
			f32 health = 0.0f;
			if (plugs[i]->plug->isAlive() && plugs[i]->plug->mHealth > 0.0f) {
				health = plugs[i]->plug->mHealth;
			}
			static_cast<FloatRangeMenuOption*>(plug_submenu->get_option("plug health"))->set_selection(health);
		}
	}
}

void StructureEditor::add_bag(Game::ItemDownFloor::Item* bag)
{
	BagWrapper* wrapper = new BagWrapper();
	wrapper->bag        = bag;
	wrapper->name       = get_bag_name(bag->mPosition.x, bag->mPosition.z);
	bags.push(wrapper);

	Game::ItemDownFloor::Mgr* mgr = Game::ItemDownFloor::mgr;

	if (!mgr) {
		return;
	}

	// clang-format off
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	bag_menu->push(new OpenSubMenuOption(wrapper->name, (new ListMenu())
		->push(new ToggleMenuOption("bag alive", true, new Delegate1<BagWrapper, bool>(bags[bags.len()-1], &BagWrapper::set_bag_state)))
    ));
	prev_heap->becomeCurrentHeap();
	// clang-format on
}

void StructureEditor::set_bag_flattened(const char* name, bool flattened)
{
	for (size_t i = 0; i < bags.len(); i++) {
		if (strcmp(bags[i]->name, name) == 0) {
			bags[i]->set_bag_state(!flattened);
			return;
		}
	}
}

void StructureEditor::clear_bags()
{
	DELETE_ALL(bags);
	if (bag_menu) {
		bag_menu->clear();
	}
}

const char* StructureEditor::get_bag_name(f32 x, f32 z)
{
	for (size_t i = 0; i < NUM_BAG_NAMES; i++) {
		NameCoordinateMap map = BAG_COORD_TO_NAME[i];
		if ((absF(map.x - x) < STRUCT_SEARCH_RADIUS) && (absF(map.z - z) < STRUCT_SEARCH_RADIUS)) {
			return map.name;
		}
	}

	GZASSERTLINE(false); // all bags should be accounted for
}

void StructureEditor::BagWrapper::set_bag_state(bool alive)
{
	if (alive) {
		if (bag->mWayPoint) {
			bag->mWayPoint->setOpen(false);
		}
		bag->mPlatInstance->setCollision(true);
		bag->mFsm->transit(bag, Game::ItemDownFloor::DOWNFLOOR_Up, nullptr);
	} else {
		bag->mPlatInstance->setCollision(false);

		bag->mIsPressed = true;
		bag->mFsm->transit(bag, Game::ItemDownFloor::DOWNFLOOR_Down, nullptr);
		if (bag->mCarryInfoList) {
			bag->mCarryInfoList->mParam.mCarryInfo.disappear();
			bag->mCarryInfoList = nullptr;
		}

		if (bag->mWayPoint) {
			bag->mWayPoint->setOpen(true);
		}
	}
}

void StructureEditor::sync_bags()
{
	for (size_t i = 0; i < bags.len(); i++) {
		// sanity check to only update bags we have actual info for
		if (!bags[i] || !bags[i]->name || !bags[i]->bag) {
			continue;
		}

		ListMenu* bag_submenu = static_cast<ListMenu*>(bag_menu->get_option(bags[i]->name)->get_sub_menu());
		if (bag_submenu) {
			static_cast<ToggleMenuOption*>(bag_submenu->get_option("bag alive"))->set_selection(!bags[i]->bag->mIsPressed);
		}
	}
}

void StructureEditor::draw()
{
	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}
	if (gate_debug_enabled) {
		gfx->initPerspPrintf(gfx->mCurrentViewport);

		for (size_t i = 0; i < gates.len(); i++) {
			if (!gates[i]) {
				continue;
			}
			Game::ItemGate* gate = gates[i]->gate;
			const char* name     = gates[i]->name;
			if (gate && name) {
				draw_gate_debug(gate, name, gfx);
			}
		}
	}
	if (bridge_debug_enabled) {
		gfx->initPerspPrintf(gfx->mCurrentViewport);

		for (size_t i = 0; i < bridges.len(); i++) {
			if (!bridges[i]) {
				continue;
			}
			Game::ItemBridge::Item* bridge = bridges[i]->bridge;
			const char* name               = bridges[i]->name;
			if (bridge && name) {
				draw_bridge_debug(bridge, name, gfx);
			}
		}
	}

	if (plug_debug_enabled) {
		gfx->initPerspPrintf(gfx->mCurrentViewport);

		for (size_t i = 0; i < plugs.len(); i++) {
			if (!plugs[i]) {
				continue;
			}
			Game::ItemBarrel::Item* plug = plugs[i]->plug;
			const char* name             = plugs[i]->name;
			if (plug && name) {
				draw_plug_debug(plug, name, gfx);
			}
		}
	}
}

void StructureEditor::draw_gate_debug(Game::ItemGate* gate, const char* name, Graphics* gfx)
{
	if (!gate || !name) {
		return;
	}

	if (gate->getGateHealth() <= 0.0f || !gate->mLod.isFlag(AILOD_IsVisible)) {
		return;
	}

	Vector3f naviPos = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f gatePos = gate->mPosition;
	if (sqrDistanceXZ(naviPos, gatePos) > SQUARE(STRUCT_DEBUG_RENDER_DIST)) {
		return;
	}

	Color4 color(255, 255, 255, 255);

	PerspPrintfInfo info;
	info.mFont   = gP2JMEMgr->mFont;
	info.mScale  = 0.5f;
	info.mColorA = color;
	info.mColorB = color;
	Vector3f pos = gatePos + Vector3f(0, 120.0f + 40.0f, 0);

	int line_height = 22;
	// draw name
	gfx->perspPrintf(info, pos, "%s", name);
	info.mPerspectiveOffsetY += line_height;

	// draw current health
	gfx->perspPrintf(info, pos, "health remaining: %.0f", gate->getGateHealth());
	info.mPerspectiveOffsetY += line_height;

	// draw total health
	gfx->perspPrintf(info, pos, "max health: %.0f", gate->mMaxSegmentHealth * gate->mMaxSegments);
	info.mPerspectiveOffsetY += line_height;
}

void StructureEditor::draw_bridge_debug(Game::ItemBridge::Item* bridge, const char* name, Graphics* gfx)
{
	if (!bridge || !name) {
		return;
	}

	if (bridge->mCurrStageIdx >= bridge->mStageCount || !bridge->mLod.isFlag(AILOD_IsVisible)) {
		return;
	}

	Vector3f naviPos   = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f bridgePos = bridge->mPosition;
	if (sqrDistanceXZ(naviPos, bridgePos) > SQUARE(STRUCT_DEBUG_RENDER_DIST)) {
		return;
	}

	Color4 color(255, 255, 255, 255);

	PerspPrintfInfo info;
	info.mFont   = gP2JMEMgr->mFont;
	info.mScale  = 0.5f;
	info.mColorA = color;
	info.mColorB = color;
	Vector3f pos = bridgePos + Vector3f(0, 50.0f + 40.0f, 0);

	int line_height = 22;
	// draw name
	gfx->perspPrintf(info, pos, "%s", name);
	info.mPerspectiveOffsetY += line_height;

	// draw segment count
	gfx->perspPrintf(info, pos, "segments built: %d / %d", bridge->mCurrStageIdx, bridge->mStageCount);
	info.mPerspectiveOffsetY += line_height;

	// draw current health
	gfx->perspPrintf(info, pos, "health remaining: %.0f", bridge->getBridgeHealth());
	info.mPerspectiveOffsetY += line_height;

	// draw total health
	f32 maxHealth = 0.0f;
	if (bridge->mIsGlitched) {
		maxHealth = Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth();
	} else {
		maxHealth = Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth() * bridge->mStageCount;
	}

	// When burrownits or sheargrubs destroy a bridge, it effectively undoes bridge glitch,
	// so max health needs to reflect that.
	if (maxHealth < bridge->getBridgeHealth()) {
		maxHealth = bridge->getBridgeHealth();
	}

	gfx->perspPrintf(info, pos, "max health: %.0f", maxHealth);
	info.mPerspectiveOffsetY += line_height;
}

void StructureEditor::draw_plug_debug(Game::ItemBarrel::Item* plug, const char* name, Graphics* gfx)
{
	if (!plug || !name) {
		return;
	}

	if (plug->mHealth <= 0.0f || !plug->mLod.isFlag(AILOD_IsVisible)) {
		return;
	}

	Vector3f naviPos = Game::naviMgr->getActiveNavi()->getPosition();
	Vector3f plugPos = plug->mPosition;
	if (sqrDistanceXZ(naviPos, plugPos) > SQUARE(STRUCT_DEBUG_RENDER_DIST)) {
		return;
	}

	Color4 color(255, 255, 255, 255);

	PerspPrintfInfo info;
	info.mFont   = gP2JMEMgr->mFont;
	info.mScale  = 0.5f;
	info.mColorA = color;
	info.mColorB = color;
	Vector3f pos = plugPos + Vector3f(0, 50.0f + 40.0f, 0);

	int line_height = 22;
	// draw name
	gfx->perspPrintf(info, pos, "%s", name);
	info.mPerspectiveOffsetY += line_height;

	// draw current health
	gfx->perspPrintf(info, pos, "health remaining: %.0f", plug->mHealth);
	info.mPerspectiveOffsetY += line_height;

	// draw total health
	gfx->perspPrintf(info, pos, "max health: %.0f", Game::ItemBarrel::mgr->mParms->mBarrelParms.mHealth());
	info.mPerspectiveOffsetY += line_height;
}

void StructureEditor::reset_all_structures()
{
	for (size_t i = 0; i < gates.len(); i++) {
		gates[i]->set_gate_segments(3);
	}
	for (size_t i = 0; i < bridges.len(); i++) {
		bridges[i]->set_bridge_segments(bridges[i]->bridge->mStageCount);
	}
	for (size_t i = 0; i < plugs.len(); i++) {
		plugs[i]->set_plug_state(true);
	}
	for (size_t i = 0; i < bags.len(); i++) {
		bags[i]->set_bag_state(true);
	}
}
