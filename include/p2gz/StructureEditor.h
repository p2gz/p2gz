#ifndef _GZ_STRUCTURE_EDITOR_H
#define _GZ_STRUCTURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemGate.h>

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

public:
	StructureEditor()
	    : gate_menu(nullptr)
	    , gate_debug_enabled(false)
	{
	}

	void init();
	void draw();

	void add_gate(Game::ItemGate* gate);
	void set_gate_stages_left(const char* name, int stages_left);
	void clear_gates();
	void sync_gates();

	void set_enabled_gate_debug(bool set) { gate_debug_enabled = set; }
	bool is_gate_debug_enabled() { return gate_debug_enabled; }
	void draw_gate_debug(Game::ItemGate* gate, const char* name, Graphics* gfx);

private:
	const char* get_gate_name(f32 x, f32 z);

	Vec<GateWrapper*> gates;
	ListMenu* gate_menu;
	bool gate_debug_enabled;
};

}; // namespace gz

#endif
