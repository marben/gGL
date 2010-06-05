/*
 * OpenGL_matrices.h
 *
 *  Created on: May 17, 2010
 *      Author: gix
 */

#ifndef OPENGL_MATRICES_H_
#define OPENGL_MATRICES_H_

#include <Eigen/Core>
#include "GL/gl_types.h"
#include <vector>

namespace ggl {
namespace ogl {

USING_PART_OF_NAMESPACE_EIGEN

// TODO: lazy evaluation should be used for matrices,
//       but it should be eventually taken care of by the libeigen library (especially by the upcoming 3 release)

class OpenGL_matrices {

public:
	OpenGL_matrices()
	{
		_projectionMatrix.setIdentity();
		_modelViewMatrix.setIdentity();
		_textureMatrix.setIdentity();

		setMatrixMode(GL_MODELVIEW);

		_worldMatrixDirty = true;
	}

	const Matrix4& getTextureMatrix() const {return _textureMatrix;}
	const Matrix4& getModelViewMatrix() const {return _modelViewMatrix;}
	const Matrix4& getProjectionMatrix() const {return _projectionMatrix;}

	void setMatrixMode(MatrixMode mode) {
		_matrixMode = mode;
		switch (mode){

		case GL_MODELVIEW:
			_activeMatrix = &_modelViewMatrix;
			_activeMatrixStack = &_modelviewMatrixStack;
			break;

		case GL_PROJECTION:
			_activeMatrix = &_projectionMatrix;
			_activeMatrixStack = &_projectionMatrixStack;
			break;

		case GL_TEXTURE:
			_activeMatrix = &_textureMatrix;
			_activeMatrixStack = &_textureMatrixStack;
			break;
		}
	}

	void loadIdentity() {_activeMatrix->setIdentity();}
	void multiplyActiveMatrix(const Matrix4d& transformation) {
		*_activeMatrix *= transformation;
		_worldMatrixDirty = true;
	}

	void pushMatrix() {
		_activeMatrixStack->push_back(*_activeMatrix);
	}

	void popMatrix() {
		assert(_activeMatrixStack->size() > 0);
		*_activeMatrix = _activeMatrixStack->back();
		_activeMatrixStack->pop_back();
	}

private:
	Matrix4 _projectionMatrix;
	Matrix4 _modelViewMatrix;
	Matrix4 _textureMatrix;
	MatrixMode _matrixMode;

	// TODO: the top of the stack could/should act as a 'current' matrix

	std::vector<Matrix4> _projectionMatrixStack;
	std::vector<Matrix4> _modelviewMatrixStack;
	std::vector<Matrix4> _textureMatrixStack;

	Matrix4* _activeMatrix;
	std::vector<Matrix4>* _activeMatrixStack;

public:
	// following is just temporary -- until the transition to the sane side is finished..
	bool _worldMatrixDirty;
	Matrix4 _worldMatrix;

	const Matrix4& getWorldMatrix() {
		if (_worldMatrixDirty)
		{
			_worldMatrix = _projectionMatrix * _modelViewMatrix;
			_worldMatrixDirty = false;
		}
		return _worldMatrix;
	}

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}
}

#endif /* OPENGL_MATRICES_H_ */
