#ifndef _WAYPOINT_VIEWER_H
#define _WAYPOINT_VIEWER_H
#include <Game/routeMgr.h>
#include <types.h>
#include <System.h>

namespace gz {
struct WaypointViewer {
public:
	WaypointViewer()
	{
		clear();
		enabled = false;
	}
	~WaypointViewer() { }

	void update();

	void toggle(bool enabled_) { enabled = enabled_; }

private:
	void categorize_nodes();
	void clear();
	void draw_edges(Graphics*);
	void draw_node(Game::WayPoint*, Graphics*);
	void populate_adjacency_matrix(Game::WayPoint*, int*);

	bool approaching[256];
	bool upcoming[256];
	bool visited[256];
	s16 edges[2][256];

	bool enabled;
};
} // namespace gz
#endif
