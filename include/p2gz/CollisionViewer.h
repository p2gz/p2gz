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
	CollisionViewer()
	{
		enabled   = false;
		olimarPos = Vector3f::zero;
		louiePos  = Vector3f::zero;
	}
	~CollisionViewer() { }

	void toggle(bool);
	void draw();

	bool is_enabled() { return enabled; }

private:
	bool is_navi_on_triangle(Sys::Triangle*, Sys::Triangle*, Sys::VertexTable*);
	void draw_triangles(Sys::Sphere&);
	bool enabled;
	Vector3f olimarPos;
	Vector3f louiePos;
	Sys::Triangle* olimarTriangle;
	Sys::Triangle* louieTriangle;
};
} // namespace gz
#endif
