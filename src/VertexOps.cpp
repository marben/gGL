/*
 * VertexOps.cpp
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#include "VertexOps.h"

namespace ggl
{
namespace ogl
{

VertexOps::VertexOps(OpenGL_state& state, Rasterizer& rasterizer) :
	_glState(state),
	_vertexBuffer(_glState.getMatrices()),
	_rasterizer(rasterizer)
{
}

void VertexOps::addVertex(Real x, Real y, Real z, Real w)
{
	assert(_glState.getActiveVertexList() != NONE);	// TODO these should be handled correctly, not with asserts
	assert(_glState.insideBeginEnd());

	Vertex4 vertex = generateVertex(x, y, z, w);

	assert(_vertexBuffer.getCoordinateType() == OBJECT);
	_vertexBuffer.add(vertex);
}

void VertexOps::glBegin(const ActiveVertexList& list)
{
	assert(!_glState.insideBeginEnd());

	_glState.insideBeginEnd(true);

	_glState.setActiveVertexList(list);
}

void VertexOps::glEnd()
{
	assert(_glState.insideBeginEnd());

	// rasterization must take place before activeVertexList is set to NONE
	_rasterizer.rasterize(_glState, _vertexBuffer);

	_vertexBuffer.clear();
	_vertexBuffer.setCoordinateType(OBJECT);

	_glState.setActiveVertexList(NONE);

	_glState.insideBeginEnd(false);
}

Vertex4 VertexOps::generateVertex(Real x, Real y, Real z, Real w)
{
	return Vertex4(Vertex4(Matrix<Real, 4, 1>(x, y, z, w), _glState.getNormal(), _glState.getActiveColor(), _glState.getFrontMaterial(), _glState.getBackMaterial(), _glState.getLightingEnabled()));
}

}	// namespace ogl
}
