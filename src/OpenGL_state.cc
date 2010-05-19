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
	_normal(0,0,1),
	_normalizeNormals(false),
	_activeVertexList(NONE),
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

}
}
