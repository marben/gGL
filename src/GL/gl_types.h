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

	typedef Matrix<Real, 3, 1> Point3d;
	typedef Matrix<Real, 3, 1> Vector3;
	typedef Matrix<Real, 4, 1> Vector4;
	typedef Matrix<Real, 4, 1> Point4d;
	//typedef Matrix<Real, 4, 1> Matrix4;
	typedef Matrix4d Matrix4;
	typedef Matrix<Real, 3, 3> Matrix3;

	template<int begin, int end, typename T>	// TODO: put this function to some other header
	T clamp(T value)
	{
		if (value > static_cast<T>(end))
			return static_cast<T>(end);

		if (value < static_cast<T>(begin))
			return static_cast<T>(begin);

		return value;
	}


	struct GlViewport
	{
		int x, y;
		unsigned w, h;

		GlViewport() : x(0), y(0), w(640), h(480) {};	// FIXME: w & h should be initialized according to the window resolution
	};

	typedef float ZBuffer_t;

	struct GlDepthRange {
		Real zNear, zFar;

		GlDepthRange() : zNear(0), zFar(1) {};
	};

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
