/*
 * OpenGL.h
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#ifndef OPENGL_H_
#define OPENGL_H_

#include <Eigen/Core>
#include <Eigen/StdVector>	// this is eigen's version of std::vector with custom allocator for certain types..

#include "GL/gl_types.h"
#include "PixelRGBA.h"
#include "Image.h"
#include "Vertex4.h"


namespace ggl
{

typedef double Real;	// this is the default floating type, we will use in opengl

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff....

typedef PixelRGBA Color;
typedef Vertex4_<double, Color> Vertex4;

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();
	void setColorBuffer(CanvasRGB* colorBuffer) {_colorBuffer = colorBuffer;}

	void clearColorBuffer();

public:
	void glClearColor(float red, float green, float blue, float alpha);
	void glVertex4(Real x, Real y, Real z = 0, Real w = 1);
	void glBegin(ActiveVertexList what);
	void glEnd();
	void glColor(float r, float g, float b, float alpha = 1);
	const CanvasRGB* glFlush();
	void glRotate(Real angle, Real x, Real y, Real z);
	void glTranslate(Real x, Real y, Real z);
	void glLoadIdentity();

private:
	void drawLines();

private:
	bool _initialized;

private:
	PixelRGBA _glClearColor;
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4> _linesVertexList;
	Color _activeColor;
	CanvasRGB* _colorBuffer;
	Matrix4d _projection;	// projection matrix

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}

#endif /* OPENGL_H_ */
