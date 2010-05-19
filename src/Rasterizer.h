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
	Rasterizer();

	void rasterize(const OpenGL_state& state, VertexBuffer& vertexBuffer);
};

}

}

#endif /* RASTERIZER_H_ */
