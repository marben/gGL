/*
 * VertexOps.h
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#ifndef VERTEXOPS_H_
#define VERTEXOPS_H_

#include <Eigen/Core>
#include "OpenGL_state.h"
#include "Vertex4.h"
#include "VertexBuffer.h"
#include "Rasterizer.h"

namespace ggl {
namespace ogl {

/*
 * This class takes care of the vertex coordinates transformations + vertex lightning (basic shading)
 */


class VertexOps {
public:
	VertexOps(OpenGL_state& state, Rasterizer& rasterizer);
	void glBegin(const ActiveVertexList& list);
	void glEnd();
	void addVertex(Real x, Real y, Real z, Real w);

private:
	Vertex4 generateVertex(Real x, Real y, Real z, Real w);

private:
	OpenGL_state& _glState;

	VertexBuffer _vertexBuffer;
	Rasterizer _rasterizer;
};

}

}

#endif /* VERTEXOPS_H_ */
