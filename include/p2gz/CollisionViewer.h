#ifndef _COLLISION_VIEWER_H
#define _COLLISION_VIEWER_H

#include <Game/mapMgr.h>
#include <Game/mapParts.h>
#include <Color4.h>
#include <Sys/Edge.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <SysShape/Model.h>

namespace gz {

// 2- and 4-bit packed array accessors (used for the collision viewer's per-triangle
// ring marks and persistent alpha levels)
inline u32 get2(const u8* arr, int i)
{
	return (arr[i >> 2] >> ((i & 3) * 2)) & 3;
}

inline void min2(u8* arr, int i, u32 v)
{
	int shift = (i & 3) * 2;
	if (v < ((arr[i >> 2] >> shift) & 3)) {
		arr[i >> 2] = (u8)((arr[i >> 2] & ~(3 << shift)) | (v << shift));
	}
}

inline u32 get4(const u8* arr, int i)
{
	return (arr[i >> 1] >> ((i & 1) * 4)) & 15;
}

inline void set4(u8* arr, int i, u32 v)
{
	int shift   = (i & 1) * 4;
	arr[i >> 1] = (u8)((arr[i >> 1] & ~(15 << shift)) | (v << shift));
}

struct CollisionViewer {
public:
	CollisionViewer()
	{
		enabled          = false;
		need_to_reenable = false;
		los_valid        = false;
	}
	~CollisionViewer() { }

	void toggle(bool);
	void draw();

	void handle_warp()
	{
		toggle(false);
		need_to_reenable = true;
	}
	bool is_enabled() { return enabled; }

private:
	bool is_navi_on_triangle(Sys::Triangle*, Sys::Triangle*, Sys::VertexTable*);
	Color4 fill_color(Sys::Triangle* tri, Sys::VertexTable* vertTable, u32 q);
	void emit_fills(Sys::TriangleTable* triTable, Sys::VertexTable* vertTable, bool opaque);
	bool enabled;
	bool need_to_reenable;
	Sys::Sphere olimarSphere;
	Sys::Sphere louieSphere;
	Sys::Triangle* olimarTriangle;
	Sys::Triangle* louieTriangle;
	Sys::Edge losEdge; // camera -> active captain sight line
	bool los_valid;
};
} // namespace gz
#endif
