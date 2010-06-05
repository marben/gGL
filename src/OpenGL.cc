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
#include <Eigen/Geometry>
#include <limits>	// to get the maximal value of a type

#include "OpenGL_settings.h"

#define PI 3.1415926535897932384626433832795	// tell me of a better way...
inline double radian(const double degree)
{
	return degree*(static_cast<double>(PI)/180.0);
}

inline double cot(double rad)
{
  return (1 / tan(rad));
}


namespace ggl {
namespace ogl
{


void OpenGL::clearColorBuffer()
{
	_colorBuffer->clear(_state.getClearColor());
}

void OpenGL::glLoadIdentity()
{
	//_activeMatrix->setIdentity();
	//updateWorldMatrix();
	_state.loadIdentity();
}

void OpenGL::glTranslate(Real x, Real y, Real z)
{
	Matrix4d trMatrix;
	trMatrix	<<	1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;

	//*_activeMatrix *= trMatrix;
	//updateWorldMatrix();
	_state.multiplyActiveMatrix(trMatrix);
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

	// TODO: optimize by checking, whether we are rotating around one base axis, which doesn't require whole matrix construction
	// TODO: check the Mesa implementation in m_matrix.c function _math_matrix_rotate(...)   some neat tricks maybe?


	if(inBetweenBeginEnd())
		return;

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

	//*_activeMatrix *= matrix;
	//updateWorldMatrix();
	_state.multiplyActiveMatrix(matrix);
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
		drawLine_smooth(vertex1, vertex2);
	}
}

void OpenGL::drawLine_smooth(const Vertex4& vertex1, const Vertex4& vertex2)
{	// TODO: should be using some bresenham implementation or something like that..

	Real dx = vertex2.x() - vertex1.x();
	Real dy = vertex2.y() - vertex1.y();
	Real adx = abs(dx);
	Real ady = abs(dy);
	const Vertex4* v1;
	const Vertex4* v2;
	if(adx>ady)	// we will iterate along the x axis
	{
		if(vertex1.x() < vertex2.x())
		{
			v1 = &vertex1;
			v2 = &vertex2;
		}
		else{
			v1 = &vertex2;
			v2 = &vertex1;
		}

		Real y( v1->y() );
		Real z( v1->z() );
		Color color( v1->getColor() );

		Color dcolor( (v2->getColor() - v1->getColor()) / (v2->x()-v1->x()) );
		Real dy( (v2->y()- v1->y()) / (v2->x() - v1->x()) );
		Real dz( (v2->z() - v1->z()) / (v2->x() - v1->x()) );

		for(int x = v1->x(); x <= v2->x(); ++x)	// TODO: check, whether storing the precomputed division aside is faster..
		{
			putPixel(x, y, z, color);	// warning: don't round() values for putpixel! values have to be rounded down like in int(float) cast, to be pixel precise..
			z += dz;
			y += dy;
			color += dcolor;
		}
	}
	else	// we will iterate along y axis
	{
		if(vertex1.y() < vertex2.y())
		{
			v1 = &vertex1;
			v2 = &vertex2;
		}
		else {
			v1 = &vertex2;
			v2 = &vertex1;
		}

		Real x( v1->x() );
		Real z( v1->z() );
		Color color( v1->getColor() );

		Color dcolor( (v2->getColor() - v1->getColor()) / (v2->y() - v1->y()) );
		Real dx( (v2->x() - v1->x()) / (v2->y() - v1->y()) );
		Real dz( (v2->z() - v1->z()) / (v2->y() - v1->y()) );

		for(int y = v1->y(); y <= v2->y(); ++y)
		{
			putPixel(x, y, z, color);
			x += dx;
			z += dz;
			color += dcolor;
		}
	}
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
		drawLine_flat(vertex1, vertex2, vertex2.getColor());
	}
}

void OpenGL::drawLine_flat(const Vertex4& vertex1, const Vertex4& vertex2, const Color& color)
{
	Real dx = vertex2.x() - vertex1.x();
	Real dy = vertex2.y() - vertex1.y();
	Real adx = abs(dx);
	Real ady = abs(dy);
	const Vertex4* v1;
	const Vertex4* v2;

	if(adx>ady)	// we will iterate along the x axis
	{
		if(vertex1.x() < vertex2.x())
		{
			v1 = &vertex1;
			v2 = &vertex2;
		}
		else{
			v1 = &vertex2;
			v2 = &vertex1;
		}

		Real y( v1->y() );
		Real z( v1->z() );

		Real dy( (v2->y()- v1->y()) / (v2->x() - v1->x()) );
		Real dz( (v2->z() - v1->z()) / (v2->x() - v1->x()) );

		for(int x = v1->x(); x <= v2->x(); ++x)
		{
			putPixel(x, y, z, color);
			z += dz;
			y += dy;
		}
	}
	else	// we will iterate along y axis
	{
		if(vertex1.y() < vertex2.y())
		{
			v1 = &vertex1;
			v2 = &vertex2;
		}
		else {
			v1 = &vertex2;
			v2 = &vertex1;
		}

		Real x( v1->x() );
		Real z( v1->z() );

		Real dx( (v2->x() - v1->x()) / (v2->y() - v1->y()) );
		Real dz( (v2->z() - v1->z()) / (v2->y() - v1->y()) );

		for(int y = v1->y(); y <= v2->y(); ++y)
		{
			putPixel(x, y, z, color);
			x += dx;
			z += dz;
		}

	}
}

void OpenGL::applyProjectionMatrix(std::vector<Vertex4>& vertices)
{
	for(size_t i = 0; i < vertices.size(); ++i)
		vertices[i] *= _state.getProjectionMatrix();
}

void OpenGL::applyViewportTransformation(Vertex4& vertex)
{
	/*
	vertex.x() = (vertex.x() + 1)*(_viewport.width/2 + _viewport.x);
	vertex.y() = (vertex.y() + 1)*(_viewport.height/2 + _viewport.y);
	*/
}

void OpenGL::applyViewportTransformation(std::vector<Vertex4>& vertices)
{
	for(size_t i = 0; i < vertices.size(); ++i)
		applyViewportTransformation(vertices[i]);
}

void OpenGL::applyPerspectiveDivision(std::vector<Vertex4>& vertices)
{
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		Real w = vertices[i].w();
		assert(w != 0.0);
		if(w == 1)
			continue;
		vertices[i].x() /= w;
		vertices[i].y() /= w;
		vertices[i].z() /= w;
	}
}

const CanvasRGB* OpenGL::glFlush()
{
	assert(_activeVertexList == NONE);

	applyPerspectiveDivision(_linesVertexList_flat);
	applyPerspectiveDivision(_linesVertexList_smooth);
	applyPerspectiveDivision(_trianglesVertexList_flat);
	applyPerspectiveDivision(_trianglesVertexList_smooth);

	applyViewportTransformation(_linesVertexList_flat);
	applyViewportTransformation(_linesVertexList_smooth);
	applyViewportTransformation(_trianglesVertexList_flat);
	applyViewportTransformation(_trianglesVertexList_smooth);


	drawLines();
	drawTriangles();

	_linesVertexList_smooth.clear();
	_linesVertexList_flat.clear();
	_trianglesVertexList_flat.clear();
	_trianglesVertexList_smooth.clear();

	return _colorBuffer;
}

/*
void OpenGL::addTriangleVertex_smooth(Real x, Real y, Real z, Real w)
{
	++_smoothTriangleVertexCounter;
	Vertex4 transformedVertex = Vertex4(_state.getWorldMatrix() * Matrix<double, 4, 1>(x, y, z, w), _state.getNormal(), _state.getActiveColor(), _state.getFrontMaterial(), _state.getBackMaterial(), _state.getLightingEnabled());

	_trianglesVertexList_smooth.push_back(transformedVertex);

	if(_smoothTriangleVertexCounter == 3)
	{
		if(_state.getCullingEnabled())
		{
			size_t length = _trianglesVertexList_smooth.size();
			const Vertex4& v1 = _trianglesVertexList_smooth[length-3];
			const Vertex4& v2 = _trianglesVertexList_smooth[length-2];
			const Vertex4& v3 = _trianglesVertexList_smooth[length-1];
			if(cullFace(v1,v2, v3))
			{
				_trianglesVertexList_smooth.pop_back();
				_trianglesVertexList_smooth.pop_back();
				_trianglesVertexList_smooth.pop_back();
			}
		}
		_smoothTriangleVertexCounter = 0;
	}
}
*/


/*
void OpenGL::addTriangleVertex_flat(Real x, Real y, Real z, Real w)
{
	++_flatTriangleVertexCounter;
	_trianglesVertexList_flat.push_back(Vertex4(_state.getWorldMatrix() * Matrix<double, 4, 1>(x, y, z, w), _state.getNormal(), _state.getActiveColor(), _state.getFrontMaterial(), _state.getBackMaterial(), _state.getLightingEnabled()));

	if(_flatTriangleVertexCounter == 3)
	{
		if(_state.getCullingEnabled())
		{
			size_t length = _trianglesVertexList_flat.size();
			const Vertex4& v1 = _trianglesVertexList_flat[length-3];
			const Vertex4& v2 = _trianglesVertexList_flat[length-2];
			const Vertex4& v3 = _trianglesVertexList_flat[length-1];
			if(cullFace(v1, v2, v3))
			{
				_trianglesVertexList_flat.pop_back();
				_trianglesVertexList_flat.pop_back();
				_trianglesVertexList_flat.pop_back();
			}
		}
		_flatTriangleVertexCounter = 0;
	}
}
*/

void OpenGL::glVertex4(Real x, Real y, Real z, Real w)
{
	_vertexOps.addVertex(x, y, z, w);
	/*
	assert(_initialized);
	assert(_activeVertexList != NONE);

	switch (_activeVertexList)
	{
	case GL_LINES:
		if(_shadeModel == GL_SMOOTH)
			_linesVertexList_smooth.push_back(Vertex4(_state.getWorldMatrix() * Matrix<Real, 4, 1>(x, y, z, w), _normal, _activeColor, _materialFront, _materialBack, _state.getLightingEnabled()));
		else
			_linesVertexList_flat.push_back(Vertex4(_worldMatrix * Matrix<Real, 4, 1>(x, y, z, w), _normal, _activeColor, _materialFront, _materialBack, _state.getLightingEnabled()));
		break;

	case GL_POLYGON:
		break;

	case GL_TRIANGLES:
		if(_shadeModel == GL_SMOOTH)
			addTriangleVertex_smooth(x, y, z, w);
		else	// flat
			addTriangleVertex_flat(x, y, z, w);	// if lighting is disabled
		break;

	case NONE:
	default:
		std::cerr<<"glVertex called while no active vertex list selected!"<<std::endl;
		break;
	}
	*/
}

void OpenGL::enableCulling(bool b)
{
	_state.setCullingEnabled(b);
}

/*
void OpenGL::enableNormalsNormalization(bool b)
{
	if(inBetweenBeginEnd()){	// TODO: should throw some error
		return;
	}

	_normalizeNormals = b;
}*/

void OpenGL::enableLighting(bool b)
{
	if(inBetweenBeginEnd()){	// TODO: should throw some error
		return;
	}

	_state.setLightingEnabled(b);
}

/*
void OpenGL::enableLight(int n, bool enable)
{
	if(n < 0 || n > (int)available_lights_number - 1) {
		std::cerr<<"enabling unavailable light number: "<<n<<std::endl;
		return;
	}


	_lights[n].enable(enabled);


	(enable)?_lights.enableLight(n):_lights.disableLight(n);
}
*/

/*
bool OpenGL::cullFace(const Vertex4& vertex1, const Vertex4& vertex2, const Vertex4& vertex3)
{
	if(_state.getCullingEnabled() == false)
		return false;

	Matrix<Real, 3, 1> v1(vertex2.x() - vertex1.x(), vertex2.y() - vertex1.y(), vertex2.z() - vertex1.z());
	Matrix<Real, 3, 1> v2(vertex3.x() - vertex1.x(), vertex3.y() - vertex1.y(), vertex3.z() - vertex1.z());
	v1.normalize();
	v2.normalize();
	Matrix<Real, 3, 1> n = (_frontFace == GL_CW)?v1.cross(v2):v2.cross(v1);
	Matrix<Real, 3, 1> eye(0.0,0.0,-1.0);
	double angle = eye.dot(n);

	if(angle <= 0 && (_state.getCullFace() == GL_BACK || _state.getCullFace() == GL_FRONT_AND_BACK))
		return true;

	if(angle >= 0 && (_state.getCullFace() == GL_FRONT || _state.getCullFace() == GL_FRONT_AND_BACK))
		return true;

	return false;
}*/


void OpenGL::glColor(float r, float g, float b, float a)
{
	_state.setActiveColor(Color(r, g, b, a));
}

void OpenGL::glBegin(ActiveVertexList what)
{
	/*
	assert(_initialized);
	assert(what != NONE);
	_activeVertexList = what;
	updateWorldMatrix();	// we need to update world matrix...it might have change outside glBegin glEnd calls;

	switch(what) {
	case GL_TRIANGLES:
		for(int i = 0; i < 2; ++i){	// we solve the case of not obtaining enough vertices in one glBegin -> glEnd interval
			if(_smoothTriangleVertexCounter != 0)
			{
				--_smoothTriangleVertexCounter;
				_trianglesVertexList_smooth.pop_back();
			}
			if(_flatTriangleVertexCounter != 0)
			{
				--_flatTriangleVertexCounter;
				_trianglesVertexList_flat.pop_back();
			}
		}
		break;

	default:
		break;
	}
	*/
	_vertexOps.glBegin(what);
}

void OpenGL::glEnd()
{
	//assert(_initialized);
	//_activeVertexList = NONE;

	_vertexOps.glEnd();
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
		drawTriangle_flat(_trianglesVertexList_flat[i+0], _trianglesVertexList_flat[i+1], _trianglesVertexList_flat[i+2], _trianglesVertexList_flat[i+2].getColor());

	// draw smooth triangles:
	if((_trianglesVertexList_smooth.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
		top = _trianglesVertexList_smooth.size() - 1;
	if((_trianglesVertexList_smooth.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
			top = _trianglesVertexList_smooth.size() - 1;
	else
		top = _trianglesVertexList_smooth.size();

	for(size_t i = 0; i < top; i += 3)
	{
		//if(_trianglesVertexList_smooth[i].lightingEnabled())
		//	drawTriangle_smooth_shaded(_trianglesVertexList_smooth[i+0], _trianglesVertexList_smooth[i+1], _trianglesVertexList_smooth[i+2]);
		//else
			drawTriangle_smooth(_trianglesVertexList_smooth[i+0], _trianglesVertexList_smooth[i+1], _trianglesVertexList_smooth[i+2]);
	}
}

void OpenGL::drawHLine_smooth(int x0, int y, double z0, int x1, double z1, const Color& c1, const Color& c2)
{
	if(x0 == x1)
	{
		return;
	}

	int x, xmax;
	double z;
	Color color, dcolor;
	double dz;

	if(x0 < x1)
	{
		x = x0;
		z = z0;
		xmax = x1;
		color = c1;
		dcolor = (c2 - c1) / (xmax - x);
		dz = (z1 - z0) / (xmax - x);
	}
	else
	{
		x = x1;
		z = z1;
		xmax = x0;
		color = c2;
		dcolor = (c1 - c2) / (xmax - x);
		dz = (z0 - z1) / (xmax - x);
	}

	while(x <= xmax)
	{
		putPixel(x, y, z, color);
		color += dcolor;
		z += dz;
		x++;
	}
}

/*
void OpenGL::drawHLine_smooth_shaded(int x0, int y, double z0, int x1, double z1, const Point3d& normal, const Material& matFront1, const Material& matFront2, const Material& matBack1, const Material& matBack2)
{
	if(x0 == x1)
	{
		return;
	}

	int x, xmax;
	double z;
	Material materialFront, materialBack, dMaterialFront, dMaterialBack;
	double dz;

	if(x0 < x1)
	{
		x = x0;
		z = z0;
		xmax = x1;
		//color = c1;
		materialFront = matFront1;
		materialBack = matBack1;
		//dcolor = (c2 - c1) / (xmax - x);
		dMaterialFront = (matFront2 - matFront1) / (xmax - x);
		dMaterialBack = (matBack2 - matBack1) / (xmax - x);
		dz = (z1 - z0) / (xmax - x);
	}
	else
	{
		x = x1;
		z = z1;
		xmax = x0;
		//color = c2;
		materialFront = matFront2;
		materialBack = matBack2;
		//dcolor = (c1 - c2) / (xmax - x);
		dMaterialFront = (matFront1 - matFront2) / (xmax - x);
		dMaterialBack = (matBack1 - matBack2) / (xmax - x);
		dz = (z0 - z1) / (xmax - x);
	}

	while(x <= xmax)
	{
		//putPixel(x, y, z, color);
		//color += dcolor;
		putPixel_shaded(x, y, z, normal, materialFront, materialBack);
		materialFront += dMaterialFront;
		materialBack += dMaterialBack;
		z += dz;
		x++;
	}
}
*/

void OpenGL::drawTriangle_smooth(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{

	bool shading = v1.lightingEnabled();
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

	int dy1 = static_cast<int>(top->y()) - static_cast<int>(bottom->y());
	int dy2 = static_cast<int>(middle->y()) - static_cast<int>(bottom->y());
	int dy3 = static_cast<int>(top->y()) - static_cast<int>(middle->y());

	Real dx1;
	if(bottom->y() == top->y())
		dx1 = 0.0;
	else
		dx1 = (top->x() - bottom->x()) / dy1;

	Real dx2;
	if(middle->y() == bottom->y())
		dx2 = 0.0;
	else
		dx2 = (middle->x() - bottom->x()) / dy2;


	Real x1 = bottom->x();
	Real x2 = bottom->x();
	Color bottomColor, middleColor, topColor;

	if(shading)
	{
		bottomColor = shade(*bottom);
		middleColor = shade(*middle);
		topColor = shade(*top);
	}
	else
	{
		bottomColor = bottom->getColor();
		middleColor = middle->getColor();
		topColor = top->getColor();
	}

	Color startColor1(bottomColor);
	Color startColor2(bottomColor);

	Color colorDy1 = (topColor - bottomColor) / dy1;	// color change along the top->bottom line
	Color colorDy2 = (middleColor - bottomColor) / dy2;	// color change along the other line

	double z1 = bottom->z();
	double z2 = bottom->z();

	double dz1 = (top->z() - bottom->z()) / dy1;
	double dz2 = (middle->z() - bottom->z()) / dy2;

	int y = bottom->y();
	while(y < middle->y())
	{
		drawHLine_smooth(x1, y, z1, x2, z2, startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
	// now for the top 'half' of a triangle
	if(middle->y() == top->y())
		dx2 = 0.0;
	else
		dx2 = (top->x() - middle->x()) / dy3;
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	z2 = middle->z();
	startColor2 = middleColor;
	colorDy2 = (topColor - middleColor) / dy3;
	dz2 = (top->z() - middle->z()) / dy3;


	while(y <= top->y())
	{
		drawHLine_smooth(x1, y, z1, x2, z2, startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
}


/* following function is basically a phong shading(except it is needed to add normal interpolation). I will use gouraud shading,
 * so im not going to continue working on it, but will leave it in place in case it is useful some day...
 */
/*void OpenGL::drawTriangle_smooth_shaded(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
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

	int dy1 = static_cast<int>(top->y()) - static_cast<int>(bottom->y());
	int dy2 = static_cast<int>(middle->y()) - static_cast<int>(bottom->y());
	int dy3 = static_cast<int>(top->y()) - static_cast<int>(middle->y());

	Real dx1;
	if(bottom->y() == top->y())
		dx1 = 0.0;
	else
		dx1 = (top->x() - bottom->x()) / dy1;

	Real dx2;
	if(middle->y() == bottom->y())
		dx2 = 0.0;
	else
		dx2 = (middle->x() - bottom->x()) / dy2;


	Real x1 = bottom->x();
	Real x2 = bottom->x();

	//Color startColor1(bottom->getColor());
	//Color startColor2(bottom->getColor());
	Material startMaterialFront1(bottom->getMaterialFront());
	Material startMaterialBack1(bottom->getMaterialBack());
	Material startMaterialFront2(bottom->getMaterialFront());
	Material startMaterialBack2(bottom->getMaterialBack());

	Color colorDy1 = (top->getColor() - bottom->getColor()) / dy1;	// color change along the top->bottom line
	Color colorDy2 = (middle->getColor() - bottom->getColor()) / dy2;	// color change along the other line
	Material materialFrontDy1 = (top->getMaterialFront() - bottom->getMaterialFront()) / dy1;
	Material materialBackDy1 = (top->getMaterialBack() - bottom->getMaterialBack()) / dy1;
	Material materialFrontDy2 = (middle->getMaterialFront() - bottom->getMaterialFront()) / dy2;
	Material materialBackDy2 = (middle->getMaterialBack() - bottom->getMaterialBack()) / dy2;

	double z1 = bottom->z();
	double z2 = bottom->z();

	double dz1 = (top->z() - bottom->z()) / dy1;
	double dz2 = (middle->z() - bottom->z()) / dy2;

	int y = bottom->y();
	while(y < middle->y())
	{
		//drawHLine_smooth(x1, y, z1, x2, z2, startColor1, startColor2);
		//startColor1 += colorDy1;
		//startColor2 += colorDy2;
		drawHLine_smooth_shaded(x1, y, z1, x2, z2, v3.getNormal(), startMaterialFront1, startMaterialFront2, startMaterialBack1, startMaterialBack2);
		startMaterialFront1 += materialFrontDy1;
		startMaterialBack1 += materialBackDy1;
		startMaterialFront2 += materialFrontDy2;
		startMaterialBack2 += materialBackDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
	// now for the top 'half' of a triangle
	if(middle->y() == top->y())
		dx2 = 0.0;
	else
		dx2 = (top->x() - middle->x()) / dy3;
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	z2 = middle->z();
	//startColor2 = middle->getColor();
	startMaterialFront2 = middle->getMaterialFront();
	startMaterialBack2 = middle->getMaterialBack();

	//colorDy2 = (top->getColor() - middle->getColor()) / dy3;
	materialFrontDy2 = (top->getMaterialFront() - middle->getMaterialFront()) / dy3;
	materialBackDy2 = (top->getMaterialBack() - middle->getMaterialBack()) / dy3;
	dz2 = (top->z() - middle->z()) / dy3;


	while(y <= top->y())
	{
		//drawHLine_smooth(x1, y, z1, x2, z2, startColor1, startColor2);
		//startColor1 += colorDy1;
		//startColor2 += colorDy2;
		drawHLine_smooth_shaded(x1, y, z1, x2, z2, v3.getNormal(), startMaterialFront1, startMaterialFront2, startMaterialBack1, startMaterialBack2);
		startMaterialFront1 += materialFrontDy1;
		startMaterialBack1 += materialBackDy1;
		startMaterialFront2 += materialFrontDy2;
		startMaterialBack2 += materialBackDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
}*/

Color OpenGL::shade(const Vertex4& vertex) const
{
	/*

	Color color(Black);
	const Material* material;

	for(size_t i = 0; i < _lights.size() ; i++)
	{
		const Light *light = &_lights[i];

		if(light->isEnabled())
		{
			Vector3d vLight;

			if(light->isDirectional())
				vLight = light->getPosition().start<3>();
			else
				vLight = light->getPosition().start<3>() - vertex.getPosition().start<3>();

			vLight.normalize();
			double angle_cos(vertex.getNormal().dot(vLight));

			// TODO: select between front and back materials
			material = &vertex.getMaterialFront();

			if(angle_cos <= 0)
				//return Black;
				angle_cos *= -1;

			// TODO: specular shading
			// TODO: ambient light should enlighten both sides of a triangle
			color += (material->getAmbient()*light->getAmbient()) + ((material->getDiffuse() * light->getDiffuse()) * angle_cos);
		}
	}
	return color;

	*/
	return Cyan;
}

void OpenGL::glLightModelAmbient(Real r, Real g, Real b, Real a)
{
	_lightModelAmbient->setAmbient(r,g,b,a);
}

void OpenGL::glNormal(Real x, Real y, Real z)
{
	_state.setNormal(Vector3(x, y, z));
	/*
	_normal << x, y, z;
	updateWorldMatrix();	// this is necessary, since _world matrix is recounted on lazy basis only on the Call of glBegin()
								// while glNormal can be called outside glBegin glEnd, we need to recount _worldMatrix now

	_normal = _state.getWorldMatrix().corner<3,3>(Eigen::TopLeft) * _normal;

	if(_normalizeNormals)
		_normal.normalize();
		*/
}

void OpenGL::drawTriangle_wired(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{
	// TODO: this is probably wrong. instead of rounding, static_cast should be called (rounding down)
	line(round_quick(v1.x()), round_quick(v1.y()), round_quick(v2.x()), round_quick(v2.y()), v1.getColor());
	line(round_quick(v2.x()), round_quick(v2.y()), round_quick(v3.x()), round_quick(v3.y()), v2.getColor());
	line(round_quick(v3.x()), round_quick(v3.y()), round_quick(v1.x()), round_quick(v1.y()), v3.getColor());
}

void OpenGL::drawTriangle_flat(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3, const Color & color)
{
	// might be better to sort using swap TODO: test
	const Vertex4 *top = &v1;
	const Vertex4 *middle = &v2;
	const Vertex4 *bottom = &v3;

	if(bottom->y() > middle->y())
		std::swap(bottom, middle);	// TODO: use xor instead?
	if(bottom->y() > top->y())
		std::swap(bottom, top);
	if(middle->y() > top->y())
		std::swap(middle, top);


	// lets iterate from bottom to top
	// we count the float x difference for both lines we iterate
	// line 1 should be the longer (from top to bottom)

	// TODO: check, for impossible triangles etc...

	int dy1 = static_cast<int>(top->y()) - static_cast<int>(bottom->y());
	int dy2 = static_cast<int>(middle->y()) - static_cast<int>(bottom->y());
	int dy3 = static_cast<int>(top->y()) - static_cast<int>(middle->y());

	Real dx1;
	if(bottom->y() == top->y())
		dx1 = 0.0;
	else
		dx1 = (top->x() - bottom->x()) / dy1;

	Real dx2;
	if(middle->y() == bottom->y())
		dx2 = 0.0;
	else
		dx2 = (middle->x() - bottom->x()) / dy2;

	Real x1 = bottom->x();
	Real x2 = bottom->x();

	double z1 = bottom->z();
	double z2 = bottom->z();

	double dz1 = (top->z() - bottom->z()) / (top->y() - bottom->y());
	double dz2 = (middle->z() - bottom->z()) / (middle->y() - bottom->y());

	int y = bottom->y();
	while(y < middle->y())
	{
		drawHLine_flat(x1, y, z1, x2, z2, color);
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
	// and now for the other 'half' of the triangle
	if(middle->y() == top->y())
		dx2 = 0.0;
	else
		dx2 = (top->x() - middle->x()) / dy3;

	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	z2 = middle->z();
	dz2 = (top->z() - middle->z()) / (top->y() - middle->y());
	while(y <= top->y())
	{
		drawHLine_flat(x1, y, z1, x2, z2, color);
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
}

void OpenGL::drawHLine_flat(int x0, int y, double z0, int x1, double z1, const Color& color)
{
	if(x0 == x1)
		return;
	if(x0 < x1)
	{
		double z = z0;
		double dz = (z1 - z0)/(x1 - x0);
		for(int x = x0; x <= x1; ++x)
		{
			putPixel(x, y, z, color);
			z += dz;
		}
	}
	else
	{
		double z = z1;
		double dz = (z0 - z1)/(x0 - x1);
		for(int x = x1; x <= x0; ++x)
		{
			putPixel(x, y, z, color);
			z += dz;
		}
	}

}

void OpenGL::line(int x0, int y0, int x1, int y1, const Color& color)
{
	_colorBuffer->line(x0, y0, x1, y1, color);
}


void OpenGL::glShadeModel(const ShadeModel& model)
{
		//if(inBetweenBeginEnd())
		//	return;	// TODO: should return some error
		//_shadeModel = model;
	_state.setShadeModel(model);
}

void OpenGL::glMatrixMode(MatrixMode mode)
{
	_state.setMatrixMode(mode);

	/*
	_matrixMode = mode;
	switch (mode)
	{
	case GL_PROJECTION:
		_activeMatrix = &_projectionMatrix;
		break;

	case GL_MODELVIEW:
		_activeMatrix = &_modelViewMatrix;
		break;

	case GL_TEXTURE:
		_activeMatrix = &_textureMatrix;

		break;


	default:
		break;
	}
	*/
}

void OpenGL::glOrtho(Real left,Real right,Real bottom,Real top,Real zNear,Real zFar)
{
	Matrix4d matrix;

	// TODO: optimize .. precomputations etc..?

	matrix <<	2/(right - left), 0, 0, -(right + left)/(right - left),
					0, 2/(top - bottom), 0, -(top + bottom)/(top - bottom),
					0, 0, -2/(zFar - zNear), -(zFar + zNear)/(zFar - zNear),
					0, 0, 0, 1;

	//*_activeMatrix *= matrix;
	//updateWorldMatrix();
	_state.multiplyActiveMatrix(matrix);
}

OpenGL::OpenGL() :
	_vertexOps(_state, _rasterizer)
	//_lights(_state.getMatrices())
{
	_initialized = false;
	_colorBuffer = NULL;
}

void OpenGL::init(int x, int y)
{
	_x = x;
	_y = y;
	_activeVertexList = NONE;

	updateWorldMatrix();
//	_shadeModel = GL_SMOOTH;
//	_frontFace = GL_CCW;
	glViewport(0, 0, x, y);

//	_smoothTriangleVertexCounter = 0;
//	_flatTriangleVertexCounter = 0;
	_zBuffer = new ZBuffer_t[x*y];

	_initialized = true;
	glMatrixMode(GL_MODELVIEW);

	_depthFunc = GL_LESS;

//	initLights();

	_rasterizer.setBuffers(_colorBuffer, _zBuffer);

}

void OpenGL::glClearColor(float red, float green, float blue, float alpha)
{
	assert(_initialized);
	_state.setClearColor(ColorRGBA(red, green, blue, alpha));
}

void OpenGL::glCullFace(CullFace mode)
{
	_state.setCullFace(mode);
}

void OpenGL::glFrontFace(FrontFace mode)
{
	_state.setFrontFace(mode);
}

void OpenGL::putPixel(int x, int y, double z, const ggl::ColorRGB& color)
{
	if( x < 0 || y < 0 || x >= _x || y >= _y)
		return;

	/*
	 * let's do the clamping here for now, but this may not be necessary later, when correct
	 * clamping and clipping is implemented
	 * TODO: think this through...
	 */
	if (z < -1.0)
		z = -1.0;
	else if (z > 1.0)
		z = 1.0;


	bool z_passed = false;
	const unsigned buffer_offset = x + _x * y;

	switch (_depthFunc)
	{
	case GL_NEVER:
		z_passed = true;

	case GL_LESS:
		z_passed = z < _zBuffer[buffer_offset];
		break;

	case GL_EQUAL:
		z_passed = z == _zBuffer[buffer_offset];
		break;

	case GL_LEQUAL:
		z_passed = z <= _zBuffer[buffer_offset];
		break;

	case GL_GREATER:
		z_passed = z > _zBuffer[buffer_offset];
		break;

	case GL_NOTEQUAL:
		z_passed = z != _zBuffer[buffer_offset];
		break;

	case GL_GEQUAL:
		z_passed = z >= _zBuffer[buffer_offset];
		break;

	case GL_ALWAYS:
		z_passed = true;
		break;
	}

	if (z_passed)
	{
		_zBuffer[buffer_offset] = z;
		_colorBuffer->putPixel(x, _y - y - 1, color);	//need to reverse y

		/*
		 * now for some tests
		 */

		//if (x == 328 && y == _y - 241 - 1)
		//	std::cout<<" x == "<<x<<"   y == "<<y<<"   z =="<<z<<"   b == "<<color.b()<<std::endl;
	}

}

/*
void OpenGL::putPixel_shaded(int x, int y, double z, const Point3d& normal, const Material& materialFront, const Material& materialBack)
{
	if( x < 0 || y < 0 || x >= _x || y >= _y)
		return;

	if(z > _zBuffer[x + _x * y])
	{
		_zBuffer[x + _x * y] = z;
		_colorBuffer->putPixel(x, _y - y - 1, materialFront.getDiffuse());
	}
}
*/

void OpenGL::clearZBuffer()
{
	// TODO: put zFar instead of this value

	// z-buffer is filled with minimal(lowest z value) and pixel with the highest value is considered the closest
	// this probably doesn't correspond to other opengl implementations using unsigned int z buffer (like http://www.sjbaker.org/steve/omniv/love_your_z_buffer.html )

	// FIXME: TODO: create cleared buffer and then just memcpy it to the zBuffer
	for(int i = 0; i < _x*_y; ++i)
		_zBuffer[i] = 1.0;
		//_zBuffer[i] = std::numeric_limits<ZBuffer_t>::max();
}

void OpenGL::gluPerspective(Real fovy, Real aspect, Real zNear, Real zFar)
{	// TODO: glu functions shouldn't be in opengl
	Matrix4d matrix;
	double f = cot(radian(fovy) / 2.0);

	matrix << 	f/aspect, 0, 0, 0,
					0, f, 0, 0,
					0, 0, (zFar + zNear) / (zNear - zFar), 2 * (zFar * zNear) / (zNear - zFar),
					0, 0, -1, 0;

	// todo: implement gluPerspective using glFrustum

	//*_activeMatrix *= matrix;
	//updateWorldMatrix();
	_state.multiplyActiveMatrix(matrix);
}

void OpenGL::glScale(Real x, Real y, Real z)
{
	Matrix4d matrix;
	matrix <<	x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1;

	//*_activeMatrix *= matrix;
	//updateWorldMatrix();

	_state.multiplyActiveMatrix(matrix);
}

/*
void OpenGL::setLightPosition(int light, const Point4d& position)
{
	_lights[light].setPosition(_state.getModelViewMatrix() * position);
}
*/

/*
void OpenGL::setLightSpotDirection(int light, const Point3d& direction)
{
	_lights[light].setSpotDirection(_state.getModelViewMatrix().block<3,3>(0,0) * direction);
}
*/

/*
void OpenGL::initLights()
{
	for (unsigned i = 0; i < available_lights_number; ++i)
	{
		_lights.push_back( Light() );
	}

	_lights.push_back( Light() );
	_lightModelAmbient = &_lights.back();
	_lightModelAmbient->setAmbient(0.2, 0.2, 0.2, 1);
	_lightModelAmbient->setDiffuse(0, 0, 0, 1);
	_lightModelAmbient->setSpecular(0, 0, 0, 1);
	_lightModelAmbient->enable();	// ambient light from nowhere is enabled by default

	for(unsigned int i = 0; i < available_lights_number; ++i)
	{
		setLightAmbient(i, 0, 0, 0, 1);
		setLightDiffuse(i, 0, 0, 0, 1);
		setLightSpecular(i, 0, 0, 0, 1);
	}

	// GL_LIGHT0 is special - enabled by default...
	setLightDiffuse(0, 1, 1, 1, 1);
	setLightSpecular(0, 1, 1, 1, 1);
	//enableLight(0, true);	// LIGHT0 seems to be enabled by default	-- no, it doesn't
}
*/

OpenGL::~OpenGL()
{
	delete _zBuffer;
}

}
}
