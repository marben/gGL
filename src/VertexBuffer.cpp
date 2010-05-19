/*
 * VertexBuffer.cpp
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#include "VertexBuffer.h"

namespace ggl
{
namespace ogl
{

void VertexBuffer::add(Vertex4& vertex)
{
	_vertices.push_back(vertex);
}


}	// namespace ogl
}
