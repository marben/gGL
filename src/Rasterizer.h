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

#include "Image.h"

namespace ggl {
namespace ogl {

class Rasterizer {
public:
	Rasterizer();	// TODO: state and vertex buffer could be passed to the constructor

	void rasterize(const OpenGL_state& state, VertexBuffer& vertexBuffer);
	void setBuffers(CanvasRGB* colorBuffer, ZBuffer_t* zBuffer) {_colorBuffer = colorBuffer; _zBuffer = zBuffer;}

private:
	//remove extra vertices, if buffer.size() % 3 != 0
	void removePendingVerticesFromTrianglesList(VertexBuffer& buffer);
	void shadeVertices(VertexBuffer& buffer);
	void putPixel(int x, int y, float z, const ggl::ColorRGB& color);
	void drawSmoothTriangles(const OpenGL_state& state, VertexBuffer& vertexBuffer);
	void drawSmoothTriangle(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);
	void drawSmoothHLine(int x0, int y, double z0, int x1, double z1, const Color& c1, const Color& c2);

	//void renderTrianglesSmooth(const OpenGL_state& state, VertexBuffer);

	CanvasRGB* _colorBuffer;
	ZBuffer_t* _zBuffer;
};

}

}

#endif /* RASTERIZER_H_ */
