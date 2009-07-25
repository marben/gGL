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

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff....

typedef PixelRGBA Color;
typedef Vertex4<double, Color> Vertex4d;

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();
	void setColorBuffer(CanvasRGB* colorBuffer) {_colorBuffer = colorBuffer;}

public:
	void glClearColor(float red, float green, float blue, float alpha);
	void glVertex4d(double x, double y, double z = 0, double w = 1);
	void glBegin(ActiveVertexList what);
	void glEnd();
	void glColor(double r, double g, double b, double alpha = 1);
	const CanvasRGB* glFlush();
	void glRotated(double angle, double x, double y, double z);
	void glTranslated(double x, double y, double z);
	void glLoadIdentity();

private:
	void drawLines();

private:
	bool _initialized;

private:
	PixelRGBA _glClearColor;
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4d> _linesVertexList;
	Color _activeColor;
	CanvasRGB* _colorBuffer;
	Matrix4d _projection;	// projection matrix

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}

#endif /* OPENGL_H_ */
