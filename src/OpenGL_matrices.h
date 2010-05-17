/*
 * OpenGL_matrices.h
 *
 *  Created on: May 17, 2010
 *      Author: gix
 */

#ifndef OPENGL_MATRICES_H_
#define OPENGL_MATRICES_H_

#include <Eigen/Core>

namespace ggl {
namespace ogl {

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

	const Matrix4d& getTextureMatrix() const {return _textureMatrix;}
	const Matrix4d& getModelViewMatrix() const {return _modelViewMatrix;}
	const Matrix4d& getProjectionMatrix() const {return _projectionMatrix;}

	void setMatrixMode(MatrixMode mode) {
		_matrixMode = mode;
		switch (mode){

		case GL_MODELVIEW:
			_activeMatrix = &_modelViewMatrix;
			break;

		case GL_PROJECTION:
			_activeMatrix = &_projectionMatrix;
			break;

		case GL_TEXTURE:
			_activeMatrix = &_textureMatrix;
			break;
		}
	}

	void loadIdentity() {_activeMatrix->setIdentity();}
	void multiplyActiveMatrix(const Matrix4d& transformation) {*_activeMatrix *= transformation; _worldMatrixDirty = true;}

private:
	Matrix4d _projectionMatrix;
	Matrix4d _modelViewMatrix;
	Matrix4d _textureMatrix;
	MatrixMode _matrixMode;

	Matrix4d* _activeMatrix;

public:
	// following is just temporary -- until the transition to the sane side is finished..
	bool _worldMatrixDirty;
	Matrix4d _worldMatrix;

	const Matrix4d& getWorldMatrix() {
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
