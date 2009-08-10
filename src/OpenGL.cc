/*
 * OpenGL.cc
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#include "OpenGL.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#define PI 3.1415926535897932384626433832795	// tell me of a better way...
inline double radian(const double degree)
{
	return degree*(static_cast<double>(PI)/180.0);
}

namespace ggl
{

void OpenGL::clearColorBuffer()
{
	_colorBuffer->clear(_glClearColor);
}

void OpenGL::glLoadIdentity()
{
	// TODO: need to distinguish active matrix!!
	_projection.setIdentity();
}

void OpenGL::glTranslate(Real x, Real y, Real z)
{
	// TODO: check, whether we are not between glBegin glEnd, otherwise set some error

	Matrix4d trMatrix;
	trMatrix <<	1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;

	_projection *= trMatrix;
}

void OpenGL::glRotate(Real angle, Real x, Real y, Real z)
{
	/*
	 * form man page:
	 *
	  glRotate produces a rotation of angle	degrees	around the
	  vector (x,y,z).  The current matrix (see glMatrixMode) is
	  multiplied by	a rotation matrix with the product replacing
	  the current matrix, as if glMultMatrix were called with the
	  following matrix as its argument:

		  ( xx(1-c)+c	xy(1-c)-zs  xz(1-c)+ys	 0  )
		  |					    |
		  | yx(1-c)+zs	yy(1-c)+c   yz(1-c)-xs	 0  |
		  | xz(1-c)-ys	yz(1-c)+xs  zz(1-c)+c	 0  |
		  |					    |
		  (	 0	     0		 0	 1  )
	  Where	c = cos(angle),	s = sine(angle), and ||( x,y,z )|| = 1
	  (if not, the GL will normalize this vector).
*/

	// TODO: need to distinguish between projection/model/texture matrix
	// TODO: optimize by checking, whether we are rotating around one base axis, which doesn't require whole matrix construction
	// TODO: check the Mesa implementation in m_matrix.c function _math_matrix_rotate(...)   some neat tricks maybe?


	Vector3d vector(x, y, z);
	vector.normalize();

	x = vector.x();
	y = vector.y();
	z = vector.z();

	double angle_rad = radian(angle);

	double c = cos(angle_rad);
	double s = sin(angle_rad);

	Matrix4d matrix;
/*	matrix << 	x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
					y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0,
					x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c, 0,
					0, 0, 0, 1; */


	// TODO: measure, whether this precomputation makes sense..
	double c1 = 1 - c;
	double xyc1 = x * y * c1;
	double xzc1 = x * z * c1;
	double yzc1 = y * z * c1;

	matrix <<	x*x*c1+c, xyc1-z*s, xzc1+y*s, 0,
					xyc1+z*s, y*y*c1+c, yzc1-x*s, 0,
					xzc1-y*s, yzc1+x*s, z*z*c1+c, 0,
					0, 0, 0, 1;

	_projection *= matrix;
}

void OpenGL::drawLines()
{
	drawLines_smooth();
	drawLines_flat();
}
void OpenGL::drawLines_smooth()
{
	size_t top;
	if((_linesVertexList_smooth.size() % 2) != 0)	// check if we have correct number of vertices for our lines
		top = _linesVertexList_smooth.size() - 1;
	else
		top = _linesVertexList_smooth.size();

	for(size_t i = 0; i < top; ++i)
	{
		const Vertex4& vertex1 = _linesVertexList_smooth[i];
		const Vertex4& vertex2 = _linesVertexList_smooth[++i];
//		const Color& color =	vertex2.color();
//		line(round_quick(vertex1.x()), round_quick(vertex1.y()), round_quick(vertex2.x()), round_quick(vertex2.y()), color);
		drawLine_smooth(vertex1, vertex2);
	}
}

void OpenGL::drawLine_smooth(const Vertex4& vertex1, const Vertex4& vertex2)
{
	// TODO: use some sane algorithm (bresenham with interpolation along z axis, or fixed point floating values or something like that
	Real dx = vertex1.x() - vertex2.x();
	Real adx = abs(dx);
	Real dy = vertex1.y() - vertex2.y();
	Real ady = abs(dy);
	if(adx > ady)	// lets iterate along the x axis..
	{
		const Vertex4 *v1, *v2;
		if(vertex1.x() > vertex2.x()){
			v1 = &vertex2;
			v2 = &vertex1;
		}
		else{
			v1 = &vertex1;
			v2 = &vertex2;
		}

		Color dcolor = (v2->color() - v1->color()) / adx;
		Color color = v1->color();

		Real y = v1->y();
		dy = (v2->y() - v1->y()) / adx;
		for(int x = round_quick(v1->x()); round_quick(x < v2->x()); x++)
		{
			_colorBuffer->putPixel(x, round_quick(y), color);
			color += dcolor;
			y += dy;
		}
	}
	else	// let's iterate along y
	{
		const Vertex4 *v1, *v2;
		if(vertex1.y() > vertex2.y()){
			v1 = &vertex2;
			v2 = &vertex1;
		}
		else{
			v1 = &vertex1;
			v2 = &vertex2;
		}

		Color dcolor = (v2->color() - v1->color()) / ady;
		Color color = v1->color();

		Real x = v1->x();
		dx = (v2->x() - v1->x()) / ady;
		for(int y = round_quick(v1->y()); y < round_quick(v2->y()); y++)
		{
			_colorBuffer->putPixel(round_quick(x), y, color);
			color += dcolor;
			x += dx;
		}
	}
	//	line(round_quick(vertex1.x()), round_quick(vertex1.y()), round_quick(vertex2.x()), round_quick(vertex2.y()), Cyan);
}

void OpenGL::drawLines_flat()
{
	size_t top;
	if((_linesVertexList_flat.size() % 2) != 0)	// check if we have correct number of vertices for our lines
		top = _linesVertexList_flat.size() - 1;
	else
		top = _linesVertexList_flat.size();

	for(size_t i = 0; i < top; ++i)
	{
		const Vertex4& vertex1 = _linesVertexList_flat[i];
		const Vertex4& vertex2 = _linesVertexList_flat[++i];
		const Color& color =	vertex2.color();
		line(round_quick(vertex1.x()), round_quick(vertex1.y()), round_quick(vertex2.x()), round_quick(vertex2.y()), color);
	}
}

const CanvasRGB* OpenGL::glFlush()
{
	assert(_activeVertexList == NONE);

	drawLines();
	drawTriangles();

	_linesVertexList_smooth.clear();
	_linesVertexList_flat.clear();
	_trianglesVertexList_flat.clear();
	_trianglesVertexList_smooth.clear();

	return _colorBuffer;
}

void OpenGL::glVertex4(Real x, Real y, Real z, Real w)
{
	assert(_initialized);
	assert(_activeVertexList != NONE);

	//Vertex4d* vertex = new Vertex4d(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor);
	switch (_activeVertexList)
	{
	case GL_LINES:
		if(_shadeModel == GL_SMOOTH)
			_linesVertexList_smooth.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		else
			_linesVertexList_flat.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		break;

	case GL_POLYGON:
		break;

	case GL_TRIANGLES:
		if(_shadeModel == GL_SMOOTH)
			_trianglesVertexList_smooth.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		else
			_trianglesVertexList_flat.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		break;

	case NONE:
	default:
		std::cerr<<"glVertex called while no active vertex list selected!"<<std::endl;
		break;
	}
}

void OpenGL::glColor(float r, float g, float b, float alpha)
{
	_activeColor= Color(r, g, b, alpha);
}

void OpenGL::glBegin(ActiveVertexList what)
{
	assert(_initialized);
	assert(what != NONE);
	_activeVertexList = what;
}

void OpenGL::glEnd()
{
	assert(_initialized);
	_activeVertexList = NONE;
}

void OpenGL::drawTriangles()
{
	/*
	 * TODO
	 * some info about advanced triangle rasterization:
	 * 1
	 * http://www.devmaster.net/forums/showthread.php?t=1094
	 * http://www.cs.unc.edu/~olano/papers/2dh-tri/
	 *
	 */


	// draw flat triangles:
	size_t top;
	if((_trianglesVertexList_flat.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
		top = _trianglesVertexList_flat.size() - 1;
	if((_trianglesVertexList_flat.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
			top = _trianglesVertexList_flat.size() - 1;
	else
		top = _trianglesVertexList_flat.size();

	for(size_t i = 0; i < top; i += 3)
		drawTriangle_flat(_trianglesVertexList_flat[i+0], _trianglesVertexList_flat[i+1], _trianglesVertexList_flat[i+2], _trianglesVertexList_flat[i+2].color());


	// draw smooth triangles:
	if((_trianglesVertexList_smooth.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
		top = _trianglesVertexList_smooth.size() - 1;
	if((_trianglesVertexList_smooth.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
			top = _trianglesVertexList_smooth.size() - 1;
	else
		top = _trianglesVertexList_smooth.size();

	for(size_t i = 0; i < top; i += 3)
	{
		//drawTriangle_flat(_trianglesVertexList_smooth[i+0], _trianglesVertexList_smooth[i+1], _trianglesVertexList_smooth[i+2], _trianglesVertexList_smooth[i+2].color());
		drawTriangle_smooth(_trianglesVertexList_smooth[i+0], _trianglesVertexList_smooth[i+1], _trianglesVertexList_smooth[i+2]);
	}
}

void OpenGL::drawHLine_smooth(int x0, int y, int x1, const Color& c1, const Color& c2)
{
	if(x0 == x1)
	{
		_colorBuffer->putPixel(x0, y, c1);	// TODO rethink this. probably shouldn't draw anything and if, the color should be (c1+c2)/2
		return;
	}

	int x, xmax;
	Color color, dcolor;
	if(x0 < x1)
	{
		x = x0;
		xmax = x1;
		color = c1;
		dcolor = (c2 - c1) / (x1 - x0);
	}
	else
	{
		x = x1;
		xmax = x0;
		color = c2;
		dcolor = (c1 - c2) / (x0 - x1);
	}

	while(x <= xmax)
	{
		_colorBuffer->putPixel(x, y, color);
		color += dcolor;
		x++;
	}
}

void OpenGL::drawTriangle_smooth(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{

	const Vertex4 *top = &v1;
	const Vertex4 *middle = &v2;
	const Vertex4 *bottom = &v3;

	if(bottom->y() > middle->y())
		std::swap(bottom, middle);
	if(bottom->y() > top->y())
		std::swap(bottom, top);
	if(middle->y() > top->y())
		std::swap(middle, top);


	// lets iterate from bottom to top
	// we count the float x difference for both lines we iterate
	// line 1 should be the longer (from top to bottom)

	// TODO: check, for impossible triangles etc...

	assert(bottom->y() != top->y());	// get rid of this
	Real dx1 = (top->x() - bottom->x()) / (top->y() - bottom->y());

	assert(middle->y() != bottom->y());	// get rid of this
	Real dx2 = (middle->x() - bottom->x()) / (middle->y() - bottom->y());

	Real x1 = bottom->x();
	Real x2 = bottom->x();

	Color startColor1(bottom->color());
	Color startColor2(bottom->color());

	Color colorDy1;	// color change along the top->bottom line
	Color colorDy2;	// color change along the other line

	colorDy1 = (top->color() - bottom->color()) / (top->y() - bottom->y());
	colorDy2 = (middle->color() - bottom->color()) / (middle->y() - bottom->y());

	int y = round_quick(bottom->y());
	while(y < middle->y())
	{
		drawHLine_smooth(round_quick(x1), y, round_quick(x2), startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		++y;
	}

	// now for the top 'half' of a triangle
	assert(middle->y() != top->y());	// get rid of this
	dx2 = (top->x() - middle->x()) / (top->y() - middle->y());
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	startColor2 = middle->color();
	colorDy2 = (top->color() - middle->color()) / (top->y() - middle->y());

	while(y <= top->y())
	{
		drawHLine_smooth(round_quick(x1), y, round_quick(x2), startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		++y;
	}

}

void OpenGL::drawTriangle_wired(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{
	line(round_quick(v1.x()), round_quick(v1.y()), round_quick(v2.x()), round_quick(v2.y()), v1.color());
	line(round_quick(v2.x()), round_quick(v2.y()), round_quick(v3.x()), round_quick(v3.y()), v2.color());
	line(round_quick(v3.x()), round_quick(v3.y()), round_quick(v1.x()), round_quick(v1.y()), v3.color());
}

void OpenGL::drawTriangle_flat(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3, const Color & color)
{
	// it would make sense to use fixed point arithmetics...but...
	// and incorporate bresenham algorithm, but...who will ever call this function?

	// first we sort vertices by y

	/*
	const Vertex4 *top, *middle, *bottom;

	if(v1.y() > v2.y()) {
		if( v1.y() > v3.y()) {
			top = &v1;
			if( v2.y() > v3.y()) {
				middle = &v2;
				bottom = &v3;
			}
			else {
				middle = &v3;
				bottom = &v2;
			}
		}
		else {
			top = &v3;
			middle = &v1;
			bottom = &v2;
		}
	}
	else {
		if(v2.y() > v3.y()) {
			top = &v2;
			if(v1.y() > v3.y()) {
				middle = &v1;
				bottom = &v3;
			}
			else {
				middle = &v3;
				bottom = &v1;
			}
		}
		else {
			top = &v3;
			middle = &v2;
			bottom = &v1;
		}
	}
	*/
	// might be better to sort using swap
	const Vertex4 *top = &v1;
	const Vertex4 *middle = &v2;
	const Vertex4 *bottom = &v3;

	if(bottom->y() > middle->y())
		std::swap(bottom, middle);
	if(bottom->y() > top->y())
		std::swap(bottom, top);
	if(middle->y() > top->y())
		std::swap(middle, top);


	// lets iterate from bottom to top
	// we count the float x difference for both lines we iterate
	// line 1 should be the longer (from top to bottom)

	// TODO: check, for impossible triangles etc...
	assert(bottom->y() != top->y());	// get rid of this
	Real dx1 = (top->x() - bottom->x()) / (top->y() - bottom->y());

	assert(middle->y() != bottom->y());	// get rid of this
	Real dx2 = (middle->x() - bottom->x()) / (middle->y() - bottom->y());

	Real x1 = bottom->x();
	Real x2 = bottom->x();

	int y = round_quick(bottom->y());
	while(y < middle->y())
	{
		hLine(round_quick(x1), y, round_quick(x2), color);
		x1 += dx1;
		x2 += dx2;
		++y;
	}
	// and now for the other 'half' of the triangle
	assert(middle->y() != top->y());	// get rid of this
	dx2 = (top->x() - middle->x()) / (top->y() - middle->y());
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	while(y <= top->y())
	{
		hLine(round_quick(x1), y, round_quick(x2), color);
		x1 += dx1;
		x2 += dx2;
		++y;
	}
}

void OpenGL::line(int x0, int y0, int x1, int y1, const Color& color)
{
	_colorBuffer->line(x0, y0, x1, y1, color);
}

void OpenGL::hLine(int x0, int y, int x1, const Color& color)
{
	_colorBuffer->hLine(x0, y, x1, color);
}

void OpenGL::glShadeModel(const ShadeModel& model)
{
		if(inBetweenBeginEnd())
			return;	// TODO: should return some error
		_shadeModel = model;
}

OpenGL::OpenGL()
{
	_initialized = false;
	_colorBuffer = NULL;
}

void OpenGL::init(int x, int y)
{
	_glClearColor.clear(0.0);
	_activeVertexList = NONE;
//	_colorBuffer.resize(x, y);
//	_colorBuffer = new Image2dRGB;
//	_colorBuffer->resize(x, y);
	_activeColor = Color(1, 1, 1, 1);
	_projection.setIdentity();
	_shadeModel = GL_SMOOTH;	// default mode according to ogl specification


	_initialized = true;
}

void OpenGL::glClearColor(float red, float green, float blue, float alpha)
{
	assert(_initialized);
	_glClearColor = PixelRGBA(red, green, blue, alpha);
}

OpenGL::~OpenGL()
{
	// TODO Auto-generated destructor stub
	// TODO: delete all allocated memory!!
}

}
