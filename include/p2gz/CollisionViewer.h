#ifndef _COLLISION_VIEWER_H
#define _COLLISION_VIEWER_H

#include <Game/mapMgr.h>
#include <Game/mapParts.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <SysShape/Model.h>

namespace gz {
struct CollisionViewer {
public:
	CollisionViewer() { enabled = false; }
	~CollisionViewer() { }

	void toggle(bool);
	void draw();

	bool is_enabled() { return enabled; }

private:
	bool is_navi_on_triangle(Sys::Triangle*, Sys::VertexTable*);
	void draw_triangles(Sys::Sphere&);
	bool enabled;
};
} // namespace gz
#endif
