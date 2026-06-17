#ifndef _COLLISION_VIEWER_H
#define _COLLISION_VIEWER_H

#include <Game/mapMgr.h>
#include <Game/mapParts.h>
#include <Color4.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <SysShape/Model.h>

namespace gz {

struct CollisionViewer {
public:
	CollisionViewer()
	{
		enabled          = false;
		need_to_reenable = false;
		olimar_triangle  = nullptr;
		louie_triangle   = nullptr;
		tri_visited      = nullptr;
		tri_alpha        = nullptr;
		edge_set         = nullptr;
		gathered_tris    = nullptr;
		tri_count        = 0;
		gathered_count   = 0;
		edge_count       = 0;
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
	static bool get_collision_tables(Sys::TriangleTable**, Sys::VertexTable**);

	void alloc_buffers(int tris);
	void free_buffers();

	void gather_triangles(Sys::Sphere&);
	void edge_set_insert(int a, int b);

	bool is_navi_on_triangle(Sys::Triangle*, Sys::Triangle*, Sys::VertexTable*);
	Color4 get_fill_color(Sys::Triangle*, Sys::VertexTable*, u8 alpha);
	void draw_triangles(Sys::TriangleTable*, Sys::VertexTable*, bool opaque);

	bool enabled;
	bool need_to_reenable;
	Sys::Sphere olimar_sphere;
	Sys::Sphere louie_sphere;
	Sys::Triangle* olimar_triangle;
	Sys::Triangle* louie_triangle;

	u8* tri_visited;
	u8* tri_alpha;
	u16* gathered_tris;
	u32* edge_set;
	int tri_count;
	int gathered_count;
	int edge_count;
};
} // namespace gz
#endif
