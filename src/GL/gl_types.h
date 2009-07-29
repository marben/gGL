/*
 * gl_types.h
 *
 *  Created on: Jul 24, 2009
 *      Author: gix
 */

#ifndef GL_TYPES_H_
#define GL_TYPES_H_

namespace ggl{	// just for the time being...should be reworked somehow
	enum ActiveVertexList {NONE, GL_POINTS, GL_LINES, GL_POLYGON, GL_TRIANGLES, GL_QUADS, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUAD_STRIP};
	enum ShadeModel {GL_SMOOTH, GL_FLAT};
}

#endif /* GL_TYPES_H_ */
