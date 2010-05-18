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

namespace ggl {
namespace ogl {

/*
 * This class takes care of the vertex coordinates transformations + vertex lightning (basic shading)
 */


class VertexOps {
public:
	VertexOps(OpenGL_state &state);
	void addVertex(Vertex4& vertex);


private:
	OpenGL_state _glState;

	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
};

}

}

#endif /* VERTEXOPS_H_ */
