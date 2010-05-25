/*
 * OpenGL_state.h
 *
 *  Created on: May 17, 2010
 *      Author: gix
 */

#ifndef OPENGL_STATE_H_
#define OPENGL_STATE_H_

#include "GL/gl_types.h"
#include "OpenGL_matrices.h"
#include "Material.h"

namespace ggl
{ namespace ogl {


class OpenGL_state
{
public:
	OpenGL_state();

	const ColorRGBA& getClearColor() const {return _clearColor;}
	void setClearColor(const ColorRGBA& clearColor) {_clearColor = clearColor;}
	void setClearColor(float r, float g, float b, float a) {_clearColor = ColorRGBA(r, g, b, a);}

	void setCullingEnabled(bool enabled) {_cullingEnabled = enabled;}
	bool getCullingEnabled() const {return _cullingEnabled;}

	const CullFace& getCullFace() const {return _cullFace;}
	void setCullFace(const CullFace& cullFace) {_cullFace = cullFace;}

	void setLightingEnabled(bool enabled) {_lightingEnabled = enabled;}
	bool getLightingEnabled() const {return _lightingEnabled;}

	//  ----- Matrix stuff
	void setMatrixMode(MatrixMode mode) {_matrices.setMatrixMode(mode);}
	const Matrix4d& getProjectionMatrix() const {return _matrices.getProjectionMatrix();}
	const Matrix4d& getTextureMatrix() const {return _matrices.getTextureMatrix();}
	const Matrix4d& getModelViewMatrix() const {return _matrices.getModelViewMatrix();}
	void loadIdentity() {_matrices.loadIdentity();}
	void multiplyActiveMatrix(const Matrix4d& transformation) {assert(!insideBeginEnd()); _matrices.multiplyActiveMatrix(transformation);}


	const Matrix4d& getWorldMatrix() {return _matrices.getWorldMatrix();}	// TODO: get rid of this
	const OpenGL_matrices& getMatrices() const {return _matrices;}

    const Point3d& getNormal() const {return _normal;}
    void setNormal(const Point3d& normal) { this->_normal = _normal; if (_normalizeNormals) _normal.normalize();}

    void setNormalizeNormals(bool normalize) {_normalizeNormals = normalize;}
    bool getNormalizeNormals() const {return _normalizeNormals;}

    void setActiveColor(const Color& color) {_activeColor = color;}
    const Color& getActiveColor() const {return _activeColor;}

    void setShadeModel(const ShadeModel& shadeModel) {assert(!insideBeginEnd()); _shadeModel = shadeModel;}
    const ShadeModel& getShadeModel() const {return _shadeModel;}

    // ----- Materials
    const Material& getFrontMaterial() const {return _frontMaterial;}
    const Material& getBackMaterial() const {return _backMaterial;}

    void setFrontMaterialAmbient(const Color& color) {_frontMaterial.setAmbient(color);}
	void setFrontMaterialDiffuse(const Color& color) {_frontMaterial.setDiffuse(color);}
	void setFrontMaterialSpecular(const Color& color) {_frontMaterial.setSpecular(color);}
	void setFrontMaterialEmission(const Color& color) {_frontMaterial.setEmission(color);}
	void setFrontMaterialShininess(float shininess) {_frontMaterial.setShininess(shininess);}
    void setBackMaterialAmbient(const Color& color) {_backMaterial.setAmbient(color);}
	void setBackMaterialDiffuse(const Color& color) {_backMaterial.setDiffuse(color);}
	void setBackMaterialSpecular(const Color& color) {_backMaterial.setSpecular(color);}
	void setBackMaterialEmission(const Color& color) {_backMaterial.setEmission(color);}
	void setBackMaterialShininess(float shininess) {_backMaterial.setShininess(shininess);}


	void setViewport(int x, int y, unsigned w, unsigned h)	{_viewport.x = x; _viewport.y = y; _viewport.w = w; _viewport.h = h;}
	const GlViewport& getViewport()const {return _viewport;}

	void setDepthRange(Real zNear, Real zFar) {_depthRange.zNear = zNear; _depthRange.zFar = zFar;}
	const GlDepthRange& getDepthRange() const {return _depthRange;}

	// and now for some not so kosher stuff:  (TODO get rid of these dirty hacks??)
	void insideBeginEnd(bool inside) {_insideBeginEnd = inside;}
	bool insideBeginEnd() {return _insideBeginEnd;}

	void setActiveVertexList(const ActiveVertexList& activeList) {_activeVertexList = activeList;}
	const ActiveVertexList& getActiveVertexList() const {return _activeVertexList;}

private:
	ColorRGBA _clearColor;
	Color _activeColor;
	bool _cullingEnabled;
	CullFace _cullFace;
	bool _lightingEnabled;
	Point3d _normal;
	bool _normalizeNormals;
	Material _frontMaterial, _backMaterial;
	ShadeModel _shadeModel;
	GlViewport _viewport;
	GlDepthRange _depthRange;

	OpenGL_matrices _matrices;
	ActiveVertexList _activeVertexList;
	bool _insideBeginEnd;
};

}
}

#endif /* OPENGL_STATE_H_ */
