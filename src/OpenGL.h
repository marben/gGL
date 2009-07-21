/*
 * OpenGL.h
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#ifndef OPENGL_H_
#define OPENGL_H_

#include "PixelRGBA.h"
#include "Image.h"

#include <vector>
#include <Eigen/Core>

namespace ggl
{

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff.... ?

enum ActiveVertexList {NONE, GL_POINTS, GL_LINES, GL_POLYGON, GL_TRIANGLES, GL_QUADS, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUAD_STRIP};

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

template<typename T>
class Vertex4
{
public:
	Vertex4(T x, T y, T z, T w, const Color& color):_vertex(Matrix<T, 4, 1>(x, y, z, w)), _color(color){}
	Vertex4(const Matrix<T, 4, 1>& vector, const Color& color):_vertex(vector), _color(color){}

	T x()const {return _vertex[0];}
	T y()const {return _vertex[1];}
	T z()const {return _vertex[2];}
	T w()const {return _vertex[3];}
	const Color& color()const {return _color;}

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	Matrix<T, 4, 1> _vertex;	// 4d vector
	Color _color;
};

typedef Vertex4<double> Vertex4d;

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	void glClearColor(float red, float green, float blue, float alpha);
	void glVertex4d(double x, double y, double z = 0, double w = 1);
	void glBegin(ActiveVertexList what);
	void glEnd();
	void glColor(double r, double g, double b, double alpha = 1);
	const Image2dRGB& glFlush();
	void glRotated(double angle, double x, double y, double z);

private:
	void drawLines();

private:
	bool _initialized;

private:
	PixelRGBA _glClearColor;
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4d*> _linesVertexList;
	Color _activeColor;
	//Image2dRGBA _colorBuffer;	// the actual buffer we draw to
	Image2dRGB _outputBuffer;
	Matrix4d _projection;	// projection matrix
};

}

#endif /* OPENGL_H_ */
