#include "Sys/TriIndexList.h"
#include "Sys/Triangle.h"
#include "Sys/TriangleTable.h"
#include "Graphics.h"

namespace Sys {
    
// TriIndexList::draw is trivial, probably bc of a debug define, so this is what that might've been
void TriIndexList::drawTriangles(Graphics& gfx, VertexTable& vertTable, TriangleTable& triTable, bool lineInfoAlreadySet)
{
    // loop through all the triangles in the list and draw each one
    for (int i = 0; i < getNum(); i++) {
        int idx = getIndex(i);
        Triangle* tri = triTable.getTriangle(idx);
        if (tri) {
            // grab the vertices and draw lines connecting each one
            Vector3f vertA = *vertTable.getVertex(tri->mVertices[0]);
            Vector3f vertB = *vertTable.getVertex(tri->mVertices[1]);
            Vector3f vertC = *vertTable.getVertex(tri->mVertices[2]);

            // triangles are currently clipping into the ground - maybe we raise them slightly?
            vertA.y += 0.02f;
            vertB.y += 0.02f;
            vertC.y += 0.02f;

            if (!lineInfoAlreadySet) {
                GXSetLineWidth(32, GX_TO_ZERO);
                gfx.initPerspPrintf(gfx.mCurrentViewport);
                gfx.mDrawColor = Color4(50, 255, 10, 255);
            }

            // draw lines
            gfx.drawLine(vertA, vertB);
            gfx.drawLine(vertB, vertC);
            gfx.drawLine(vertC, vertA);
        }
    }
}

} // namespace Sys
