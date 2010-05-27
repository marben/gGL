/*
 * Lights.cc
 *
 *  Created on: May 26, 2010
 *      Author: gix
 */

#include "Lights.h"

namespace ggl {

namespace ogl {

Lights::Lights(const OpenGL_matrices& matrices, int numberOfLights) :
		_globalAmbientLight(0.2, 0.2, 0.2, 1.0),
		_matrices(matrices)
{
	for (int i = 0; i < numberOfLights; ++i)
	{
		_lights.push_back(Light());
	}

	// GL_LIGHT0 is special by default
	setDiffuse(0, 1, 1, 1, 1);
	setSpecular(0, 1, 1, 1, 1);
}

void Lights::setPosition(unsigned lightNumber, const Point4d& position)
{
	assert(lightNumber < numberOfLights());

	const Matrix4d& modelviewMatrix = _matrices.getModelViewMatrix();

	_lights[lightNumber].setPosition(modelviewMatrix * position);
}

void Lights::enableLight(unsigned lightNumber)
{
	assert(lightNumber < numberOfLights());

	_lights[lightNumber].enable();
}

void Lights::disableLight(unsigned lightNumber)
{
	assert(lightNumber < numberOfLights());

	_lights[lightNumber].disable();
}

void Lights::setSpotDirection(unsigned lightNumber, const Point3d& direction)
{
	assert(lightNumber < numberOfLights());

	const Matrix4d& modelviewMatrix = _matrices.getModelViewMatrix();

	_lights[lightNumber].setSpotDirection(modelviewMatrix.block<3,3>(0,0) * direction);
}

void Lights::setAmbient(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(lightNumber < numberOfLights());

	_lights[lightNumber].setAmbient(r, g, b, a);
}

void Lights::setDiffuse(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(lightNumber < numberOfLights());

	_lights[lightNumber].setDiffuse(r, g, b, a);
}

void Lights::setSpecular(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(lightNumber < numberOfLights());

	_lights[lightNumber].setSpecular(r, g, b, a);
}


}	// namespace ogl
}
