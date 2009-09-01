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


namespace ggl {
namespace ogl
{

const int available_lights_number = 12;	// TODO: check docs/other ogl implementations and eventually increase

typedef double Real;	// this is the default floating type, we will use in opengl

inline int round(Real x){	// TODO: put this in some global header
	return static_cast<int>( (x>0.0) ? x + 0.5 : x - 0.5 );
}
inline int round_quick(Real x){	// TODO:here should be some extremely fast..not so precise function ..
	return round(x);
	//return static_cast<int>(x);
}

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff....

typedef PixelRGBA Color;
typedef Vertex4_<Real, Color> Vertex4;
typedef Matrix<Real, 3, 1, Eigen::DontAlign> Point3d;

class Light
{
public:
	Light():_enabled(false){}
	void enable(bool enabled = true) {_enabled = enabled;}
	void disable() {enable(false);}
private:
	Color _color;
	Point3d _position;
	bool _enabled;
};

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();
	void setColorBuffer(CanvasRGB* colorBuffer) {_colorBuffer = colorBuffer;}

	void clearColorBuffer();
	void clearZBuffer();
	void enableCulling(bool b = true);
	void enableNormalsNormalization(bool b = true);
	void disableNormalsNormalization() {enableNormalsNormalization(false);}
	void disableCulling() {enableCulling(false);}
	void enableLighting(bool b = true);
	void disableLighting() {enableLighting(false);}
	void enableLight(int n, bool enabled = true);
	void disableLight(int n) {enableLight(n, false);}

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
	void glMatrixMode(MatrixMode mode);
	void glOrtho(Real left,Real right,Real bottom,Real top,Real zNear,Real zFar);
	void glViewport(Real x, Real y, Real width, Real height) { _viewport.x = x; _viewport.y = y; _viewport.width = width; _viewport.height = height;}
	void glCullFace(CullFace mode);
	void glFrontFace(FrontFace mode);
	void glScale(Real x, Real y, Real z);
	void glNormal(Real x, Real y, Real z);

	void gluPerspective(Real fovy, Real aspect, Real zNear, Real zFar);

	const Matrix4d& getModelViewMatrix() const {return _modelViewMatrix;}
	const Matrix4d& getProjectionMatrix() const {return _projectionMatrix;}
	const Matrix4d& getTextureMatrix() const {return _textureMatrix;}

	void applyProjectionMatrix(std::vector<Vertex4>& vertices);	// multiples all vertices by a projection matrix
	void applyViewportTransformation(Vertex4& vertex);
	void applyViewportTransformation(std::vector<Vertex4>& vertices);
	void applyPerspectiveDivision(std::vector<Vertex4>& vertices);

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

	bool cullFace(const Vertex4& v1, const Vertex4& v2, const Vertex4& v3);

	void addTriangleVertex_smooth(Real x, Real y, Real z, Real w);
	void addTriangleVertex_flat(Real x, Real y, Real z, Real w);
	void updateWorldMatrix() {_worldMatrixDirty = true;}	// sets the flag so that we know to recount the world matrix
	void countWorldMatrix() {if(_worldMatrixDirty){_worldMatrix = _projectionMatrix * _modelViewMatrix;_worldMatrixDirty = false;}}

private:
	bool _initialized;

private:
	PixelRGBA _glClearColor;
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4> _linesVertexList_smooth, _linesVertexList_flat, _trianglesVertexList_flat, _trianglesVertexList_smooth;
	Color _activeColor;
	CanvasRGB* _colorBuffer;
	Matrix4d _projectionMatrix, _modelViewMatrix, _textureMatrix;	// projection matrix
	Matrix4d _worldMatrix;	// projectionMatrix * modelViewMatrix // TODO: change calls to _worldMatrix to worldMatrix() function and implement lazy evaluation
	bool _worldMatrixDirty;
	Matrix4d* _activeMatrix;	// pointer to active matrix
	ShadeModel _shadeModel;
	MatrixMode _matrixMode;
	double* _zBuffer;
	int _x, _y;	// resolution we are working with
	struct {Real x; Real y; Real width; Real height;} _viewport;
	bool _cullingEnabled;
	bool _normalizeNormals;	// set with GL_NORMALIZE
	bool _lightingEnabled;	// true if lighting was enables
	CullFace _cullFace;
	FrontFace _frontFace;
	Point3d _normal;	// the active normal
	Light _lights[available_lights_number];


	int _smoothTriangleVertexCounter;	// only for use by addTrianglVertex_smooth
	int _flatTriangleVertexCounter;	// only for use by addTrianglVertex_flat

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}
}
#endif /* OPENGL_H_ */
