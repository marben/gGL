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


typedef float Real;	// this is the default floating type, we will use in opengl
// TODO: write a type defining all basic operators on integer, using fixed point precision... something like:
// template<typename Int_t, int precision>
//	class Fixed_t{
//		Fixed_t operator+(const Fixed_t a){return ( (this->_value * a->value) >> precision ) ;}
//		Int_t _value;
//}

inline int round(Real x){	// TODO: put this in some global header
	return static_cast<int>( (x>0.0) ? x + 0.5 : x - 0.5 );
}
inline int round_quick(Real x){	// TODO:here should be some extremely fast..not so precise function ..
	return round(x);
	//return static_cast<int>(x);
}

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
	void clearZBuffer();

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
	void glShadeModel(const ShadeModel& model);
	void glMatrixMode(MatrixMode mode) {_matrixMode = mode;}


private:
	void drawLines();
	void drawTriangles();
	void drawLines_smooth();
	void drawLines_flat();

	void drawTriangle_wired(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);
	void drawTriangle_flat(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3, const Color & color);
	void drawTriangle_smooth(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);

	void drawHLine_smooth(int x0, int y, double z0, int x1, double z1, const Color& c1, const Color& c2);
	void drawHLine_flat(int x0, int y, double z0, int x1, double z1, const Color& color);

	void line(int x0, int y0, int x1, int y1, const Color& color);	// 2D line on a drawing surface
	void putPixel(int x, int y, double z, const ggl::PixelRGB& color);	// puts pixel on color buffer and sets z buffer

	void drawLine_smooth(const Vertex4& vertex1, const Vertex4& vertex2);
	void drawLine_flat(const Vertex4& vertex1, const Vertex4& vertex2, const Color& color);

	bool inBetweenBeginEnd(){return _activeVertexList != NONE;}
private:
	bool _initialized;

private:
	PixelRGBA _glClearColor;
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4> _linesVertexList_smooth, _linesVertexList_flat, _trianglesVertexList_flat, _trianglesVertexList_smooth;
	Color _activeColor;
	CanvasRGB* _colorBuffer;
	Matrix4d _projection;	// projection matrix
	ShadeModel _shadeModel;
	MatrixMode _matrixMode;
	double* _zBuffer;
	int _x, _y;	// resolution we are working with

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}

#endif /* OPENGL_H_ */
