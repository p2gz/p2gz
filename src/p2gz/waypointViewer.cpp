#include <p2gz/WaypointViewer.h>
#include <Game/pathfinder.h>
#include <Game/PikiMgr.h>
#include <PikiAI.h>
#include <System.h>
#include <Game/Navi.h>
#include <Game/MapMgr.h>

using namespace gz;

const Color4 VISITED_COLOR      = Color4(0, 255, 0, 255);
const Color4 APPROACHING_COLOR  = Color4(255, 255, 0, 255);
const Color4 UPCOMING_COLOR     = Color4(255, 255, 255, 255);
const Color4 NOT_IN_ROUTE_COLOR = Color4(0, 0, 0, 255);

const int RENDER_DISTANCE = SQUARE(512);

// Classify each node as visited, approaching, or upcoming.
void WaypointViewer::categorize_nodes()
{
	Iterator<Game::Piki> pikiIter(Game::pikiMgr);
	CI_LOOP(pikiIter)
	{
		Game::Piki* piki = *pikiIter;
		if (piki->getCurrActionID() == PikiAI::ACT_Transport) {
			PikiAI::ActTransport* action = static_cast<PikiAI::ActTransport*>(piki->getCurrAction());
			if (action->mIsMoving) {
				int i = 0;
				for (Game::PathNode* node = action->mPathMove->mRootNode; node; node = node->mNext) {
					if (i <= action->mPathMove->mCurrGraphIdx) {
						visited[node->mWpIndex] = true;
					} else if (i == action->mPathMove->mCurrGraphIdx + 1) {
						approaching[node->mWpIndex] = true;
					} else {
						upcoming[node->mWpIndex] = true;
					}
					i++;
				}
			}
		}
	}
}

// Draw a node based on its classification.
void WaypointViewer::draw_node(Game::WayPoint* wp, Graphics* gfx)
{
	Vector3f apex = wp->mPosition + Vector3f(0, 16, 0);
	if (visited[wp->mIndex]) {
		gfx->mDrawColor = VISITED_COLOR;
	} else if (approaching[wp->mIndex]) {
		gfx->mDrawColor = APPROACHING_COLOR;
	} else if (upcoming[wp->mIndex]) {
		gfx->mDrawColor = UPCOMING_COLOR;
	}

	gfx->drawCone(wp->mPosition, apex, 16, 8);
	gfx->mDrawColor = Color4(0, 0, 0, 255);
}

// Determine which nodes are adjacent. Nodes that are one-way do not consistently
// use to-links or from-links, and some nodes are bidirectional, so we just check
// both directions and draw duplicate lines in the event that they are bidirectional.
void WaypointViewer::populate_adjacency_matrix(Game::WayPoint* wp, int* nextEdge)
{
	for (int i = 0; i < wp->mNumToLinks; i++) {
		if (wp->mToLinks[i] != -1) {
			edges[0][*nextEdge] = wp->mIndex;
			edges[1][*nextEdge] = wp->mToLinks[i];
			(*nextEdge)++;
		}
	}
	for (int i = 0; i < wp->mNumFromLinks; i++) {
		if (wp->mFromLinks[i] != -1) {
			edges[0][*nextEdge] = wp->mIndex;
			edges[1][*nextEdge] = wp->mFromLinks[i];
			(*nextEdge)++;
		}
	}
}

// Draw edges between nodes based on their classifications.
void WaypointViewer::draw_edges(Graphics* gfx)
{
	for (int i = 0; i < 256; i++) {
		s16 wp1 = edges[0][i];
		s16 wp2 = edges[1][i];
		if (wp1 == -1 && wp2 == -1) {
			break;
		}

		Game::WayPoint* vertex1 = Game::mapMgr->mRouteMgr->getWayPoint(wp1);
		Vector3f apex1          = vertex1->mPosition + Vector3f(0, 16, 0);

		Game::WayPoint* vertex2 = Game::mapMgr->mRouteMgr->getWayPoint(wp2);
		Vector3f apex2          = vertex2->mPosition + Vector3f(0, 16, 0);

		GXSetLineWidth(10, GX_TO_ZERO);
		gfx->initPerspPrintf(gfx->mCurrentViewport);

		if (visited[wp1] && visited[wp2]) {
			gfx->mDrawColor = VISITED_COLOR;
		} else if ((visited[wp1] && approaching[wp2]) || (visited[wp2] && approaching[wp1])) {
			gfx->mDrawColor = APPROACHING_COLOR;
		} else if ((approaching[wp1] && upcoming[wp2]) || (approaching[wp2] && upcoming[wp1])) {
			gfx->mDrawColor = UPCOMING_COLOR;
		} else if (upcoming[wp1] && upcoming[wp2]) {
			gfx->mDrawColor = UPCOMING_COLOR;
		} else {
			gfx->mDrawColor = NOT_IN_ROUTE_COLOR;
		}

		gfx->drawLine(apex1, apex2);
		gfx->mDrawColor = NOT_IN_ROUTE_COLOR;
	}
}

// Draw the waypoint graph.
void WaypointViewer::draw()
{
	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);
	GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);

	categorize_nodes();

	int nextEdge = 0;
	Iterator<Game::WayPoint> waypointIter(Game::mapMgr->mRouteMgr);
	CI_LOOP(waypointIter)
	{
		Game::WayPoint* wp = *waypointIter;
		if (Game::naviMgr->getActiveNavi() != nullptr) {
			Vector3f naviPos = Game::naviMgr->getActiveNavi()->getPosition();
			Vector3f wpPos   = wp->getPosition();
			if (sqrDistanceXZ(naviPos, wpPos) <= RENDER_DISTANCE) {
				draw_node(wp, gfx);
				populate_adjacency_matrix(wp, &nextEdge);
			}
		}
	}

	draw_edges(gfx);
}
