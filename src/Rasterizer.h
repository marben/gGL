/*
 * Rasterizer.h
 *
 *  Created on: May 19, 2010
 *      Author: gix
 */

#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include "OpenGL_state.h"
#include "VertexBuffer.h"

namespace ggl {
namespace ogl {

class Rasterizer {
public:
	Rasterizer();	// TODO: state and vertex buffer could be passed to the constructor

	void rasterize(const OpenGL_state& state, VertexBuffer& vertexBuffer);

private:
	//remove extra vertices, if buffer.size() % 3 != 0
	void removePendingVerticesFromTrianglesList(VertexBuffer& buffer);
	void shadeVertices(VertexBuffer& buffer);

	//void renderTrianglesSmooth(const OpenGL_state& state, VertexBuffer);
};

}

}

#endif /* RASTERIZER_H_ */
