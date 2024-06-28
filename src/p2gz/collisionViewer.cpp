#include "Game/Navi.h"
#include "Sys/TriIndexList.h"
#include "Sys/Triangle.h"
#include "Sys/TriangleTable.h"
#include "Graphics.h"
#include "stl/math.h"

namespace Sys {
    
void TriIndexList::drawTriangles(Graphics& gfx, VertexTable& vertTable, TriangleTable& triTable, bool lineInfoAlreadySet)
{
    for (int i = 0; i < getNum(); i++) {
        int idx = getIndex(i);
        Triangle* tri = triTable.getTriangle(idx);
        if (tri) {
            bool isNaviStandingOnThisTriangle = false;
            if (Game::naviMgr->getActiveNavi() != nullptr) {
                Triangle* naviTriangle = Game::naviMgr->getActiveNavi()->mFloorTriangle;
                if (naviTriangle) {
                    isNaviStandingOnThisTriangle = true;
                    for (int i = 0; i < 3; i++) {
                        Vector3f naviVertex = *vertTable.getVertex(naviTriangle->mVertices[i]);
                        Vector3f triVertex = *vertTable.getVertex(tri->mVertices[i]);
                        if (naviVertex != triVertex) {
                            isNaviStandingOnThisTriangle = false;
                            break;
                        }
                    }
                }
            }

            Color4 color;
            if (isNaviStandingOnThisTriangle) {
                color = Color4(200, 200, 200, 128);
            } else {
                switch (tri->mCode.getSlipCode()) {
                    case MapCode::Code::SlipCode_NoSlip:
                        color = Color4(0, 50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 0, 128);
                        break;
                    case MapCode::Code::SlipCode_Gradual:
                        color = Color4(0, 0, 50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 128);
                        break;
                    case MapCode::Code::SlipCode_Steep:
                        color = Color4(50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 0, 0, 128);
                        break;
                }
            }
            
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
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
