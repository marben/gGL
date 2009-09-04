/*
 * Material.cc
 *
 *  Created on: Sep 4, 2009
 *      Author: gix
 */

#include "Material.h"

namespace ggl
{

namespace ogl
{

Material Material::operator-(const Material& mat) const
{
	Material newMaterial(*this);
	newMaterial._ambient -= mat._ambient;
	newMaterial._diffuse -= mat._diffuse;
	newMaterial._specular -= mat._specular;
	newMaterial._emission -= mat._emission;
	newMaterial._shininess -= mat._shininess;
	return newMaterial;
}

Material& Material::operator+=(const Material& mat)
{
	_ambient += mat._ambient;
	_diffuse += mat._diffuse;
	_specular += mat._specular;
	_emission += mat._emission;
	_shininess += mat._shininess;
	return *this;
}

Material Material::operator/(const float f) const
{
	Material newMaterial(*this);
	newMaterial._ambient /= f;
	newMaterial._diffuse /= f;
	newMaterial._specular /= f;
	newMaterial._emission /= f;
	newMaterial._shininess /= f;
	return newMaterial;
}

Material::Material():
	_ambient(0.2, 0.2, 0.2, 1.0), _diffuse(0.8, 0.8, 0.8, 1.0), _specular(0.0, 0.0, 0.0, 1.0),
	_emission(0.0, 0.0, 0.0, 1.0), _shininess(0.0)
{
	// TODO Auto-generated constructor stub

}

}

}
