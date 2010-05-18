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

VertexOps::VertexOps(OpenGL_state &state) :
	_glState(state)
{
}

void VertexOps::addVertex(Vertex4& vertex)
{
	assert(_activeVertexList != NONE);
}


}	// end of namespace ogl
}
