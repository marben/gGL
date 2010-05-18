/*
 * gl_types.h
 *
 *  Created on: Jul 24, 2009
 *      Author: gix
 */

#ifndef GL_TYPES_H_
#define GL_TYPES_H_

#include <Eigen/Core>
USING_PART_OF_NAMESPACE_EIGEN

#include "../ColorRGBA.h"

namespace ggl {	// just for the time being...should be reworked somehow
namespace ogl
{
	typedef double Real;	// this is the default floating type, we will use in opengl

	typedef ColorRGBA Color;

	typedef Matrix<Real, 3, 1/*, Eigen::DontAlign*/> Point3d;
	typedef Matrix<Real, 4, 1/*, Eigen::DontAlign*/> Point4d;

	enum ActiveVertexList {NONE,
		GL_POINTS, GL_LINES,
		GL_POLYGON, GL_TRIANGLES,
		GL_QUADS, GL_LINE_STRIP,
		GL_LINE_LOOP, GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN, GL_QUAD_STRIP};

	enum ShadeModel {GL_SMOOTH, GL_FLAT};

	enum MatrixMode {GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE};

	enum CullFace {GL_FRONT, GL_BACK, GL_FRONT_AND_BACK};

	enum FrontFace {GL_CW, GL_CCW};

	enum DepthFunc {GL_NEVER, GL_LESS,
		GL_EQUAL, GL_LEQUAL,
		GL_GREATER, GL_NOTEQUAL,
		GL_GEQUAL, GL_ALWAYS};
}
}

#endif /* GL_TYPES_H_ */
