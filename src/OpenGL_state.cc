/*
 * OpenGL_state.cc
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#include "OpenGL_state.h"

namespace ggl
{
namespace ogl
{

OpenGL_state::OpenGL_state() :
	_clearColor(0.0, 0.0, 0.0, 0.0),
	_activeColor(1.0, 1.0, 1.0, 1.0),
	_cullingEnabled(false),
	_cullFace(GL_BACK),
	_lightingEnabled(false),
	_normal(0, 0, 1),
	_normalizeNormals(false),
	_activeVertexList(NONE),
	_frontFace(GL_CCW),
	_lights(_matrices),
	_blendingEnabled(false),
	_insideBeginEnd(false)
{
	// --- Initial material state
	setFrontMaterialAmbient(Color(0.2, 0.2, 0.2, 1.0));
	setFrontMaterialDiffuse(Color(0.8, 0.8, 0.8, 1.0));
	setFrontMaterialSpecular(Color(0.0, 0.0, 0.0, 1.0));
	setFrontMaterialEmission(Color(0.0, 0.0, 0.0, 1.0));
	setFrontMaterialShininess(0.0);
	setBackMaterialAmbient(Color(0.2, 0.2, 0.2, 1.0));
	setBackMaterialDiffuse(Color(0.8, 0.8, 0.8, 1.0));
	setBackMaterialSpecular(Color(0.0, 0.0, 0.0, 1.0));
	setBackMaterialEmission(Color(0.0, 0.0, 0.0, 1.0));
	setBackMaterialShininess(0.0);
}

void OpenGL_state::setLightAmbient(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(!insideBeginEnd());

	_lights.setAmbient(lightNumber, r, g, b, a);
}

void OpenGL_state::setLightDiffuse(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(!insideBeginEnd());

	_lights.setDiffuse(lightNumber, r, g, b, a);
}

void OpenGL_state::setLightSpecular(unsigned lightNumber, float r, float g, float b, float a)
{
	assert(!insideBeginEnd());

	_lights.setSpecular(lightNumber, r, g, b, a);
}

void OpenGL_state::setLightPosition(unsigned lightNumber, const Point4d& position)
{
	assert(!insideBeginEnd());

	_lights.setPosition(lightNumber, position);
}

void OpenGL_state::setLightSpotDirection(unsigned lightNumber, const Point3d& direction)
{
	assert(!insideBeginEnd());

	_lights.setSpotDirection(lightNumber, direction);
}

void OpenGL_state::enableLight(unsigned lightNumber)
{
	assert(!insideBeginEnd());

	_lights.enableLight(lightNumber);
}

void OpenGL_state::disableLight(unsigned lightNumber)
{
	assert(!insideBeginEnd());

	_lights.disableLight(lightNumber);
}
void OpenGL_state::setGlobalAmbientLight(float r, float g, float b, float a)
{
	assert(!insideBeginEnd());

	_lights.setGlobalAmbientLight(r,g,b,a);
}

void OpenGL_state::setNormal(const Vector3& normal)
{
	this->_normal = normal;
}

}
}
