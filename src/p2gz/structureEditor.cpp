#include <p2gz/StructureEditor.h>
#include <p2gz/gzmenu.h>
#include <p2gz/p2gz.h>
#include <Game/Entities/ItemGate.h>
#include <P2JME/P2JME.h>
#include <types.h>

using namespace gz;

#define GATE_SEARCH_RADIUS     (5.0f)
#define GATE_DEBUG_RENDER_DIST (512.0f)

static const size_t NUM_GATE_NAMES                                            = 20;
static const StructureEditor::NameCoordinateMap COORD_TO_NAME[NUM_GATE_NAMES] = {
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
	StructureEditor::NameCoordinateMap(-3514.0f, 1153.0f, "water gate"),
};

void StructureEditor::init()
{
	gate_menu = static_cast<ListMenu*>(p2gz->menu->get_option("map/structures/gates")->get_sub_menu());
}

void StructureEditor::add_gate(Game::ItemGate* gate)
{
	GateWrapper* gate_wrapper = new GateWrapper();
	gate_wrapper->gate        = gate;
	gate_wrapper->name        = get_gate_name(gate->mPosition.x, gate->mPosition.z);
	gates.push(gate_wrapper);

	// clang-format off
	gate_menu->push(new OpenSubMenuOption(gate_wrapper->name, (new ListMenu())
	    ->push(new RangeMenuOption("segments remaining", 0, 3, 3 - gate->mSegmentsDown, RangeMenuOption::CAP, new Delegate1<GateWrapper, s32>(gates[gates.len()-1], &GateWrapper::set_gate_segments)))
		->push(new FloatRangeMenuOption("segment health", 0.0f, gate->mMaxSegmentHealth, gate->mCurrentSegmentHealth, new Delegate1<GateWrapper, f32>(gates[gates.len()-1], &GateWrapper::set_gate_segment_health)))
    ));
	// clang-format on
}

void StructureEditor::clear_gates()
{
	gates.clear();
	if (gate_menu) {
		gate_menu->clear();
	}
}

const char* StructureEditor::get_gate_name(f32 x, f32 z)
{
	for (size_t i = 0; i < NUM_GATE_NAMES; i++) {
		NameCoordinateMap map = COORD_TO_NAME[i];
		if ((absF(map.x - x) < GATE_SEARCH_RADIUS) && (absF(map.z - z) < GATE_SEARCH_RADIUS)) {
			return map.name;
		}
	}

	char* name = new char[8];
	sprintf(name, "gate %d", gates.len());
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

void StructureEditor::draw()
{
	if (!gate_debug_enabled) {
		return;
	}

	if (!Game::itemGateMgr) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}
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
	if (sqrDistanceXZ(naviPos, gatePos) > SQUARE(GATE_DEBUG_RENDER_DIST)) {
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
