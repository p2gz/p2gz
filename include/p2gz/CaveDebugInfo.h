#ifndef _GZ_CAVE_DEBUG_INFO_H
#define _GZ_CAVE_DEBUG_INFO_H

#include <Vector3.h>
#include <Game/Cave/Info.h>
#include <p2gz/gzCollections.h>

namespace gz {

struct SpawnPoint {
public:
	SpawnPoint(Vector3f pos_, f32 radius_, Game::Cave::BaseGen::CaveGenType kind_)
	{
		pos    = pos_;
		radius = radius_;
		kind   = kind_;
	}
	SpawnPoint()
	{
		pos    = Vector3f::zero;
		radius = 16.0f;
		kind   = Game::Cave::BaseGen::CGT_EnemyEasy;
	}

	Vector3f pos;
	f32 radius;
	Game::Cave::BaseGen::CaveGenType kind;
};

struct CaveDebugInfo {
public:
	CaveDebugInfo();
	~CaveDebugInfo() { }

	void draw();

	void register_spawn_points();
	void set_draw_spawn_points(bool enabled) { draw_spawn_points = enabled; }

	void clear() { spawn_points.clear(); }

private:
	void draw_circle(Vector3f position, f32 radius, Color4 color);

	bool draw_spawn_points;
	Vec<SpawnPoint> spawn_points;
};

} // namespace gz

#endif
