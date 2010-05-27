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
#include "ColorRGBA.h"
#include "Image.h"
#include "Vertex4.h"
#include "Light.h"	// TODO: remove include
#include "OpenGL_state.h"
#include "VertexOps.h"
#include "Rasterizer.h"
//#include "Lights.h"


namespace ggl {
namespace ogl
{

inline int round(Real x){	// TODO: put this in some global header
	return static_cast<int>( (x>0.0) ? x + 0.5 : x - 0.5 );
}

inline int round_quick(Real x){	// TODO:here should be some fast..not so precise function .. (SSE4 supports some fast rounding)
	return round(x);
}

USING_PART_OF_NAMESPACE_EIGEN	// some libeigen suff....

class OpenGL
{
public:
	OpenGL();
	void init(int x, int y);
	virtual ~OpenGL();
	void setColorBuffer(CanvasRGB* colorBuffer) {_colorBuffer = colorBuffer; _rasterizer.setBuffers(_colorBuffer, _zBuffer);}

	void clearColorBuffer();
	void clearZBuffer();
	void enableCulling(bool b = true);
	void setNormalizeNormals(bool enable) {_state.setNormalizeNormals(enable);}
	void disableCulling() {enableCulling(false);}
	void enableLighting(bool b = true);
	void disableLighting() {enableLighting(false);}

	void enableLight(int n) {_state.enableLight(n);}
	void disableLight(int n) {_state.disableLight(n);}

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
	//void glViewport(Real x, Real y, Real width, Real height) { _viewport.x = x; _viewport.y = y; _viewport.width = width; _viewport.height = height;}
	void glViewport(Real x, Real y, Real w, Real h) {_state.setViewport(x, y, w, h);}
	void glCullFace(CullFace mode);
	void glFrontFace(FrontFace mode);
	void glScale(Real x, Real y, Real z);
	void glNormal(Real x, Real y, Real z);
	void glLightModelAmbient(Real r, Real g, Real b, Real a);
	void glDepthFunc(DepthFunc function) {_depthFunc = function;}

	void gluPerspective(Real fovy, Real aspect, Real zNear, Real zFar);

	const Matrix4d& getModelViewMatrix() const {return _state.getModelViewMatrix();}
	const Matrix4d& getProjectionMatrix() const {return _state.getProjectionMatrix();}
	const Matrix4d& getTextureMatrix() const {return _state.getTextureMatrix();}

	void applyProjectionMatrix(std::vector<Vertex4>& vertices);	// multiples all vertices by a projection matrix
	void applyViewportTransformation(Vertex4& vertex);
	void applyViewportTransformation(std::vector<Vertex4>& vertices);
	void applyPerspectiveDivision(std::vector<Vertex4>& vertices);

	void setLightAmbient(unsigned lightNumber, float r, float g, float b, float a) {_state.setLightAmbient(lightNumber, r, g, b, a);}
	void setLightDiffuse(unsigned lightNumber, float r, float g, float b, float a) {_state.setLightDiffuse(lightNumber, r, g, b, a);}
	void setLightSpecular(unsigned lightNumber, float r, float g, float b, float a) {_state.setLightSpecular(lightNumber, r, g, b, a);}
	void setLightPosition(unsigned lightNumber, const Point4d& position) {_state.setLightPosition(lightNumber, position);}
	void setLightSpotDirection(unsigned lightNumber, const Point3d& direction) {_state.setLightSpotDirection(lightNumber, direction);}

	void setFrontMaterialAmbient(const Color& color) {_state.setFrontMaterialAmbient(color);}
	void setFrontMaterialDiffuse(const Color& color) {_state.setFrontMaterialDiffuse(color);}
	void setFrontMaterialSpecular(const Color& color) {_state.setFrontMaterialSpecular(color);}
	void setFrontMaterialEmission(const Color& color) {_state.setFrontMaterialEmission(color);}

	void setBackMaterialAmbient(const Color& color) {_state.setBackMaterialAmbient(color);}
	void setBackMaterialDiffuse(const Color& color) {_state.setBackMaterialDiffuse(color);}
	void setBackMaterialSpecular(const Color& color) {_state.setBackMaterialSpecular(color);}
	void setBackMaterialEmission(const Color& color) {_state.setBackMaterialEmission(color);}

private:
	void drawLines();
	void drawTriangles();
	void drawLines_smooth();
	void drawLines_flat();

	void drawTriangle_wired(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);
	void drawTriangle_flat(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3, const Color & color);
	void drawTriangle_smooth(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);
	//void drawTriangle_smooth_shaded(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3);

	void drawHLine_smooth(int x0, int y, double z0, int x1, double z1, const Color& c1, const Color& c2);
	void drawHLine_flat(int x0, int y, double z0, int x1, double z1, const Color& color);
	//void drawHLine_smooth_shaded(int x0, int y, double z0, int x1, double z1, const Point3d& normal, const Material& matFront1, const Material& matFront2, const Material& matBack1, const Material& matBack2);

	void line(int x0, int y0, int x1, int y1, const Color& color);	// 2D line on a drawing surface
	void putPixel(int x, int y, double z, const ggl::ColorRGB& color);	// puts pixel on color buffer and sets z buffer
	Color shade(const Vertex4& vertex)const;

	void drawLine_smooth(const Vertex4& vertex1, const Vertex4& vertex2);
	void drawLine_flat(const Vertex4& vertex1, const Vertex4& vertex2, const Color& color);

	bool inBetweenBeginEnd(){return _activeVertexList != NONE;}

	bool cullFace(const Vertex4& v1, const Vertex4& v2, const Vertex4& v3);

	void addTriangleVertex_smooth(Real x, Real y, Real z, Real w);
	void addTriangleVertex_flat(Real x, Real y, Real z, Real w);
	void updateWorldMatrix() {_worldMatrixDirty = true;}	// sets the flag so that we know to recount the world matrix
	//void initLights();	// sets lights to default state according to opengl specification

private:

private:
	bool _initialized;

private:
	ActiveVertexList _activeVertexList;	// the active vertex list(set by glBegin())
	std::vector<Vertex4> _linesVertexList_smooth, _linesVertexList_flat, _trianglesVertexList_flat, _trianglesVertexList_smooth;
	CanvasRGB* _colorBuffer;
	Matrix4d _worldMatrix;	// projectionMatrix * modelViewMatrix // TODO: change calls to _worldMatrix to worldMatrix() function and implement lazy evaluation
	bool _worldMatrixDirty;
	//ShadeModel _shadeModel;
	ZBuffer_t* _zBuffer;
	int _x, _y;	// resolution we are working with
	//struct {Real x; Real y; Real width; Real height;} _viewport;
	//FrontFace _frontFace;
	//Point3d _normal;	// FIXME: delete
	//std::vector<Light> _lights;
	Light* _lightModelAmbient;	// TODO: make it possible to disable this light
	DepthFunc _depthFunc;

	//int _smoothTriangleVertexCounter;	// only for use by addTrianglVertex_smooth
	//int _flatTriangleVertexCounter;	// only for use by addTrianglVertex_flat

	OpenGL_state _state;	// the state -- TODO: most of OpenGL class private members should move here
	VertexOps _vertexOps;	// operations on vertices are done here - coordinate transformations, lighting..
	Rasterizer _rasterizer;
	//Lights _lights;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}
}
#endif /* OPENGL_H_ */
