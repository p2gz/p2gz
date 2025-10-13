#ifndef _WAYPOINT_VIEWER_H
#define _WAYPOINT_VIEWER_H
#include <Game/pathfinder.h>
#include <types.h>
#include <System.h>

namespace gz {
struct WaypointViewer {
public:
	WaypointViewer()
	{
		for (int i = 0; i < 256; i++) {
			approaching[i] = false;
		}
		for (int i = 0; i < 256; i++) {
			upcoming[i] = false;
		}
		for (int i = 0; i < 256; i++) {
			visited[i] = false;
		}

		for (int i = 0; i < 256; i++) {
			edges[0][i] = -1;
			edges[1][i] = -1;
		}
	}
	~WaypointViewer() { }

	void draw();

private:
	void categorize_nodes();
	void draw_edges(Graphics*);
	void draw_node(Game::WayPoint*, Graphics*);
	void populate_adjacency_matrix(Game::WayPoint*, int*);

	bool approaching[256];
	bool upcoming[256];
	bool visited[256];

	s16 edges[2][256];
};
} // namespace gz
#endif
