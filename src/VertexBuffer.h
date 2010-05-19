/*
 * VertexBuffer.h
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <vector>

#include "OpenGL_matrices.h"
#include "Vertex4.h"

namespace ggl {
namespace ogl {

class VertexBuffer {
public:
	VertexBuffer(const OpenGL_matrices& matrices) :
		_matrices(matrices)
	{
	}

	void add(Vertex4& vertex);

private:
	const OpenGL_matrices& _matrices;
	std::vector<Vertex4> _vertices;
};

}

}

#endif /* VERTEXBUFFER_H_ */
