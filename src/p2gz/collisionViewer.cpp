#include "Sys/TriIndexList.h"
#include "Sys/Triangle.h"
#include "Sys/TriangleTable.h"
#include "Graphics.h"
#include "stl/math.h"

namespace Sys {
    
// TriIndexList::draw is trivial, probably bc of a debug define, so this is what that might've been
void TriIndexList::drawTriangles(Graphics& gfx, VertexTable& vertTable, TriangleTable& triTable, bool lineInfoAlreadySet)
{
    // loop through all the triangles in the list and draw each one
    for (int i = 0; i < getNum(); i++) {
        int idx = getIndex(i);
        Triangle* tri = triTable.getTriangle(idx);
        if (tri) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);

            Color4 color;
            switch (tri->mCode.getSlipCode()) {
                case MapCode::Code::SlipCode_NoSlip:
                    color = Color4(0, 255 * fabs(tri->mTrianglePlane.mNormal.y), 0, 255);
                    break;
                case MapCode::Code::SlipCode_Gradual:
                    color = Color4(0, 0, 255 * fabs(tri->mTrianglePlane.mNormal.y), 255);
                    break;
                case MapCode::Code::SlipCode_Steep:
                    color = Color4(255 * fabs(tri->mTrianglePlane.mNormal.y), 0, 0, 255);
                    break;
            }
            
            for (int i = 0; i < 3; i++) {
                Vector3f vertex = *vertTable.getVertex(tri->mVertices[i]);

                GXPosition3f32(vertex.x, vertex.y, vertex.z);
                GXColor4u8(color.r, color.g, color.b, color.a);
            }
            GXEnd();
        }
    }
}

} // namespace Sys
