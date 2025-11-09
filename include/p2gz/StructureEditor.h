#ifndef _GZ_STRUCTURE_EDITOR_H
#define _GZ_STRUCTURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemGate.h>
#include <Game/Entities/ItemBridge.h>
#include <Game/Entities/ItemDownFloor.h>

namespace gz {

struct StructureEditor {
	struct GateWrapper {
		GateWrapper()
		{
			gate = nullptr;
			name = nullptr;
		}

		Game::ItemGate* gate;
		const char* name;

		void set_gate_segments(s32 segments);
		void set_gate_segment_health(f32 health);
	};

	struct BridgeWrapper {
		BridgeWrapper()
		{
			bridge = nullptr;
			name   = nullptr;
		}

		Game::ItemBridge::Item* bridge;
		const char* name;

		void set_bridge_segments(s32 segments);
		void set_bridge_segment_health(f32 health);
		void set_bridge_glitch(bool glitched);
	};

	struct PlugWrapper {
		PlugWrapper()
		{
			plug = nullptr;
			name = nullptr;
		}

		Game::ItemBarrel::Item* plug;
		const char* name;
		Vector3f pos;

		void set_plug_state(bool alive);
		void set_plug_health(f32 health);
	};

	struct BagWrapper {
		BagWrapper()
		{
			bag  = nullptr;
			name = nullptr;
		}

		Game::ItemDownFloor::Item* bag;
		const char* name;

		void set_bag_state(bool alive);
	};

	struct NameCoordinateMap {
		NameCoordinateMap(f32 x_, f32 z_, const char* name_)
		    : x(x_)
		    , z(z_)
		    , name(name_)
		{
		}

		const f32 x;
		const f32 z;
		const char* name;
	};

	struct WaterBoxMap {
		WaterBoxMap(const char* name_, f32 minX, f32 minY, f32 minZ, f32 maxX, f32 maxY, f32 maxZ)
		    : min_bounds(minX, minY, minZ)
		    , max_bounds(maxX, maxY, maxZ)
		    , name(name_)
		{
		}

		const Vector3f min_bounds;
		const Vector3f max_bounds;
		const char* name;
	};

public:
	StructureEditor()
	    : gate_menu(nullptr)
	    , gate_debug_enabled(false)
	    , bridge_menu(nullptr)
	    , bridge_debug_enabled(false)
	    , plug_menu(nullptr)
	    , plug_debug_enabled(false)
	    , bag_menu(nullptr)
	{
	}

	void init();
	void draw();

	void add_gate(Game::ItemGate* gate);
	void set_gate_stages_left(const char* name, int stages_left);
	void clear_gates();
	void sync_gates();

	void add_bridge(Game::ItemBridge::Item* bridge);
	void set_bridge_stages_left(const char* name, int stages_left);
	void clear_bridges();
	void sync_bridges();

	void add_plug(Game::ItemBarrel::Item* plug);
	void clear_plugs();
	void sync_plugs();

	void add_bag(Game::ItemDownFloor::Item* bag);
	void clear_bags();
	void sync_bags();

	void set_enabled_gate_debug(bool set) { gate_debug_enabled = set; }
	bool is_gate_debug_enabled() { return gate_debug_enabled; }
	void draw_gate_debug(Game::ItemGate* gate, const char* name, Graphics* gfx);

	void set_enabled_bridge_debug(bool set) { bridge_debug_enabled = set; }
	bool is_bridge_debug_enabled() { return bridge_debug_enabled; }
	void draw_bridge_debug(Game::ItemBridge::Item* bridge, const char* name, Graphics* gfx);

	void set_enabled_plug_debug(bool set) { plug_debug_enabled = set; }
	bool is_plug_debug_enabled() { return plug_debug_enabled; }
	void draw_plug_debug(Game::ItemBarrel::Item* plug, const char* name, Graphics* gfx);

	ListMenu* gate_menu;
	ListMenu* bridge_menu;
	ListMenu* plug_menu;
	ListMenu* bag_menu;

private:
	const char* get_gate_name(f32 x, f32 z);
	const char* get_bridge_name(f32 x, f32 z);
	const char* get_plug_name(f32 x, f32 z);
	const char* get_bag_name(f32 x, f32 z);

	Vec<GateWrapper*> gates;
	Vec<BridgeWrapper*> bridges;
	Vec<PlugWrapper*> plugs;
	Vec<BagWrapper*> bags;

	bool gate_debug_enabled;
	bool bridge_debug_enabled;
	bool plug_debug_enabled;
};

}; // namespace gz

#endif
