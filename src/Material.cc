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

Material::Material():
	_ambient(0.2, 0.2, 0.2, 1.0), _diffuse(0.8, 0.8, 0.8, 1.0), _specular(0.0, 0.0, 0.0, 1.0),
	_emission(0.0, 0.0, 0.0, 1.0), _shininess(0.0)
{
	// TODO Auto-generated constructor stub

}

}

}
