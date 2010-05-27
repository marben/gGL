/*
 * Lights.h
 *
 *  Created on: May 26, 2010
 *      Author: gix
 */

#include <vector>

#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "Light.h"
#include "OpenGL_matrices.h"

namespace ggl {
namespace ogl {

static const int NUMBER_OF_LIGHTS = 8;	// TODO: put global settings somewhere else...

class Lights {
public:
	Lights(const OpenGL_matrices& matrices, int numberOfLights = NUMBER_OF_LIGHTS);

	void setAmbient(unsigned lightNumber, float r, float g, float b, float a);
	void setDiffuse(unsigned lightNumber, float r, float g, float b, float a);
	void setSpecular(unsigned lightNumber, float r, float g, float b, float a);
	void setPosition(unsigned lightNumber, const Point4d& position);
	void setSpotDirection(unsigned lightNumber, const Point3d& direction);

	void enableLight(unsigned lightNumber);
	void disableLight(unsigned lightNumber);

	void setGlobalAmbientLight(float r, float g, float b, float a) {_globalAmbientLight = ColorRGBA(r, g, b, a);}
	const Color& getGlobalAmbientLight() const {return _globalAmbientLight;}

	unsigned numberOfLights() const {return _lights.size();}

	const Light& operator[](size_t i) const {return _lights[i];}

private:
	std::vector<Light> _lights;

	ColorRGBA _globalAmbientLight;	// the ambient light set by glLightModel's GL_LIGHT_AMBIENT

	const OpenGL_matrices& _matrices;
};

}

}

#endif /* LIGHTS_H_ */
