/*
 * VertexBuffer.h
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include "OpenGL_matrices.h"

namespace ggl {

namespace ogl {

class VertexBuffer {
public:
	VertexBuffer() {
	}

private:
	OpenGL_matrices* matrices;
};

}

}

#endif /* VERTEXBUFFER_H_ */
