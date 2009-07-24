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

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff.... ?


/*
class Color
{
public:
	Color():_r(1), _g(1), _b(1), _a(1){}
	Color(double r, double g, double b, double alpha = 1):_r(r),_g(g), _b(b), _a(alpha){}
private:
	double _r, _g, _b, _a;
};
*/

typedef PixelRGBA Color;
typedef Vertex4<double, Color> Vertex4d;

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();

public:
	void glClearColor(float red, float green, float blue, float alpha);
	void glVertex4d(double x, double y, double z = 0, double w = 1);
	void glBegin(ActiveVertexList what);
	void glEnd();
	void glColor(double r, double g, double b, double alpha = 1);
	const Image2dRGB& glFlush();
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
	//Image2dRGBA _colorBuffer;	// the actual buffer we draw to
	Image2dRGB _outputBuffer;
	Matrix4d _projection;	// projection matrix

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}

#endif /* OPENGL_H_ */
