/*
 * Material.h
 *
 *  Created on: Sep 4, 2009
 *      Author: gix
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GL/gl_types.h"

namespace ggl
{

namespace ogl
{

class Material
{
public:
	Material();
	void setAmbient(const Color& color) {_ambient = color;}
	void setDiffuse(const Color& color) {_diffuse = color;}
	void setSpecular(const Color& color) {_specular = color;}
	void setEmission(const Color& color) {_emission = color;}
	void setShininess(float shininess) {_shininess = clamp <0, 128> (shininess);}

	const Color& getAmbient() const {return _ambient;}
	const Color& getDiffuse() const {return _diffuse;}
	const Color& getSpecular() const {return _specular;}
	const Color& getEmission() const {return _emission;}
	float getShininess() const {return _shininess;}

	Material operator-(const Material& mat) const;
	Material operator/(const float f) const;
	Material& operator+=(const Material& mat);

private:
	Color _ambient, _diffuse, _specular, _emission;
	float _shininess;	//specular exponent  [0, 128]; default = 0
};

}

}

#endif /* MATERIAL_H_ */
