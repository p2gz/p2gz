#ifndef _SYS_TRIINDEXLIST_H
#define _SYS_TRIINDEXLIST_H

#include "Matrix3f.h"
#include "stream.h"
#include "Sys/IndexList.h"
#include "Vector3.h"

struct Graphics;

namespace Sys {
struct TriangleTable;
struct VertexTable;

struct TriIndexList : public IndexList {
	TriIndexList() { }

	virtual ~TriIndexList() { } // _08 (weak)

	void constructClone(TriangleTable&);
	void getMinMax(VertexTable&, TriangleTable&, Vector3f&, Vector3f&, f32&, f32&);
	void makeCovarianceMatrix(VertexTable&, TriangleTable&, Matrix3f&, Vector3f&);
	void draw(Graphics&, VertexTable&, TriangleTable&, bool);
    void drawTriangles(Graphics&, VertexTable&, TriangleTable&, bool); // @P2GZ, this is defined in mapMgr.cpp for now
};
} // namespace Sys

#endif
