#ifndef _GZ_STRUCTURE_EDITOR_H
#define _GZ_STRUCTURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemGate.h>

namespace gz {

struct StructureEditor {
	struct GateWrapper {
		Game::ItemGate* gate;

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
	StructureEditor() { }

	void init();

	void add_gate(Game::ItemGate* gate);
	void clear_gates();

private:
	const char* get_gate_name(f32 x, f32 z);

	Vec<GateWrapper> gates;
	ListMenu* gate_menu;
};

}; // namespace gz

#endif
