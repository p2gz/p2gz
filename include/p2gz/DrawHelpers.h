#ifndef _GZ_DRAW_HELPERS_H
#define _GZ_DRAW_HELPERS_H

#include <types.h>
#include <Vector3.h>
#include <Color4.h>

namespace gz {

f32 get_min_Y_clamped(Vector3f& pos, Vector3f& ref_pos, f32 max_height_diff, f32 offset_above_floor);
void draw_circle(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor);
void draw_wrapped_circle(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor, f32 max_height);
void draw_ring(Vector3f position, f32 radius, Color4 color, f32 offset_above_floor);

}; // namespace gz

#endif
