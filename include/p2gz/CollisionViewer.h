#ifndef _COLLISION_VIEWER_H
#define _COLLISION_VIEWER_H

#include <Game/mapMgr.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <SysShape/Model.h>

namespace gz {
struct CollisionViewer {
public:
	CollisionViewer() { enabled = false; }
	~CollisionViewer() { }

	void toggle(bool enabled_)
	{
		SysShape::Model* mapModel = static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapModel;
		if (enabled_) {
			mapModel->hide();
		} else {
			mapModel->show();
		}
		enabled = enabled_;
	}

	void draw();

private:
	bool is_navi_on_triangle(Sys::Triangle*, Sys::VertexTable&);
	void draw_triangles(Sys::Sphere&);
	bool enabled;
};
} // namespace gz
#endif
