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

namespace ggl
{ namespace ogl {

class OpenGL_state
{
public:

	const ColorRGBA& getClearColor() const {return _clearColor;}
	void setClearColor(const ColorRGBA& clearColor) {_clearColor = clearColor;}
	void setClearColor(float r, float g, float b, float a) {_clearColor = ColorRGBA(r, g, b, a);}

	void setCullingEnabled(bool enabled) {_cullingEnabled = enabled;}
	bool getCullingEnabled() const {return _cullingEnabled;}

	const CullFace& getCullFace() const {return _cullFace;}
	void setCullFace(const CullFace& cullFace) {_cullFace = cullFace;}

	void setLightingEnabled(bool enabled) {_lightingEnabled = enabled;}
	bool getLightingEnabled() const {return _lightingEnabled;}

	//  ----- Matrix stufff
	void setMatrixMode(MatrixMode mode) {_matrices.setMatrixMode(mode);}
	const Matrix4d& getProjectionMatrix() const {return _matrices.getProjectionMatrix();}
	const Matrix4d& getTextureMatrix() const {return _matrices.getTextureMatrix();}
	const Matrix4d& getModelViewMatrix() const {return _matrices.getModelViewMatrix();}
	void loadIdentity() {_matrices.loadIdentity();}
	void multiplyActiveMatrix(const Matrix4d& transformation) {_matrices.multiplyActiveMatrix(transformation);}


	const Matrix4d& getWorldMatrix() {return _matrices.getWorldMatrix();}

public:
	OpenGL_state() :
		_clearColor(0.0, 0.0, 0.0, 0.0),
		_cullingEnabled(false),
		_cullFace(GL_BACK),
		_lightingEnabled(false)
	{

	}

private:
	ColorRGBA _clearColor;
	bool _cullingEnabled;
	CullFace _cullFace;
	bool _lightingEnabled;

	OpenGL_matrices _matrices;
};

}
}

#endif /* OPENGL_STATE_H_ */
