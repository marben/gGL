/*
 * Rasterizer.cpp
 *
 *  Created on: May 19, 2010
 *      Author: gix
 */


	/*
	 * TODO
	 * some info about advanced triangle rasterization:
	 * 1
	 * http://www.devmaster.net/forums/showthread.php?t=1094
	 * http://www.cs.unc.edu/~olano/papers/2dh-tri/
	 *
	 */

#include "Rasterizer.h"

namespace ggl {

namespace ogl {

Rasterizer::Rasterizer() {
	// TODO Auto-generated constructor stub
}

void Rasterizer::rasterize(const OpenGL_state& state, VertexBuffer& vertexBuffer)
{
	assert(state.getActiveVertexList() != NONE);
	assert(vertexBuffer.getCoordinateType() == OBJECT);

	switch(state.getActiveVertexList())
	{
	case  GL_TRIANGLES:
		removePendingVerticesFromTrianglesList(vertexBuffer);

		if (state.getLightingEnabled() == true)
		{
			vertexBuffer.transformToEyeCoordinates();
			shadeVertices(vertexBuffer);
		}

		vertexBuffer.transformToClipCoordinates();

		vertexBuffer.transformToNormalizedDeviceCoordinates();

		vertexBuffer.transformToWindowCoordinates(state.getViewport(), state.getDepthRange());

		switch (state.getShadeModel())
		{
		case GL_SMOOTH:
			break;
		case GL_FLAT:
			break;
		}

		break;
	default:
		assert(0);	// not supported yet
		break;
	}
}

void Rasterizer::shadeVertices(VertexBuffer& buffer)
{
	assert(buffer.getCoordinateType() == EYE);
}

void Rasterizer::removePendingVerticesFromTrianglesList(VertexBuffer& buffer)
{
	// we work with GL_TRIANGLES list => vertices must be in triples
	size_t i = buffer.size() % 3;
	if (i > 0)
		buffer.popBack();

	if (i > 0)
		buffer.popBack();
}


}
}
