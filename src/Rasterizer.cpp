/*
 * Rasterizer.cpp
 *
 *  Created on: May 19, 2010
 *      Author: gix
 */


	/*
	 * TODO
	 * some info about advanced triangle rasterization:
	 * 1
	 * http://www.devmaster.net/forums/showthread.php?t=1094
	 * http://www.cs.unc.edu/~olano/papers/2dh-tri/
	 *
	 */

#include "Rasterizer.h"
#include <Eigen/Geometry>

namespace ggl {

namespace ogl {

Rasterizer::Rasterizer() {
	// TODO Auto-generated constructor stub
}

// This is basically what is known as "Primitive Assembly"
void Rasterizer::rasterize(const OpenGL_state& state, VertexBuffer& vertexBuffer)
{
	assert(state.getActiveVertexList() != NONE);
	assert(vertexBuffer.getCoordinateType() == OBJECT);

	_state = &state;	//TODO: this whole concept of OpenGL_state, VertexOps, VertexBuffer is wrong...need to redesign and make objects passing more sane

	switch(state.getActiveVertexList())
	{
	case  GL_TRIANGLES:
		removePendingVerticesFromTrianglesList(vertexBuffer);

		if (state.getLightingEnabled() == true)
		{
			vertexBuffer.transformToEyeCoordinates();
			shadeVertices(vertexBuffer);
		}

		vertexBuffer.transformToClipCoordinates();

		vertexBuffer.transformToNormalizedDeviceCoordinates();

		vertexBuffer.transformToWindowCoordinates(state.getViewport(), state.getDepthRange());

		switch (state.getShadeModel())
		{
		case GL_SMOOTH:
			drawSmoothTriangles(state, vertexBuffer);
			break;
		case GL_FLAT:
			break;
		}

		break;
	default:
		assert(0);	// not supported yet
		break;
	}
}

bool Rasterizer::cullFace(const Vertex4& vertex1, const Vertex4& vertex2, const Vertex4& vertex3)
{
	if(_state->getCullingEnabled() == false)
		return false;

	Matrix<Real, 3, 1> v1(vertex2.x() - vertex1.x(), vertex2.y() - vertex1.y(), vertex2.z() - vertex1.z());
	Matrix<Real, 3, 1> v2(vertex3.x() - vertex1.x(), vertex3.y() - vertex1.y(), vertex3.z() - vertex1.z());
	v1.normalize();
	v2.normalize();
	Matrix<Real, 3, 1> n = (_state->getFrontFace() == GL_CW)?v1.cross(v2):v2.cross(v1);
	Matrix<Real, 3, 1> eye(0.0,0.0,-1.0);
	double angle = eye.dot(n);

	if(angle <= 0 && (_state->getCullFace() == GL_BACK || _state->getCullFace() == GL_FRONT_AND_BACK))
		return true;

	if(angle >= 0 && (_state->getCullFace() == GL_FRONT || _state->getCullFace() == GL_FRONT_AND_BACK))
		return true;

	return false;
}

void Rasterizer::drawSmoothTriangle(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{
	const Vertex4 *top = &v1;
	const Vertex4 *middle = &v2;
	const Vertex4 *bottom = &v3;

	if(bottom->y() > middle->y())
		std::swap(bottom, middle);
	if(bottom->y() > top->y())
		std::swap(bottom, top);
	if(middle->y() > top->y())
		std::swap(middle, top);


	// lets iterate from bottom to top
	// we count the float x difference for both lines we iterate
	// line 1 should be the longer (from top to bottom)

	// TODO: check, for impossible triangles etc...

	int dy1 = static_cast<int>(top->y()) - static_cast<int>(bottom->y());
	int dy2 = static_cast<int>(middle->y()) - static_cast<int>(bottom->y());
	int dy3 = static_cast<int>(top->y()) - static_cast<int>(middle->y());

	Real dx1;
	if(bottom->y() == top->y())
		dx1 = 0.0;
	else
		dx1 = (top->x() - bottom->x()) / dy1;

	Real dx2;
	if(middle->y() == bottom->y())
		dx2 = 0.0;
	else
		dx2 = (middle->x() - bottom->x()) / dy2;


	Real x1 = bottom->x();
	Real x2 = bottom->x();
	Color bottomColor, middleColor, topColor;

	bottomColor = bottom->getColor();
	middleColor = middle->getColor();
	topColor = top->getColor();

	Color startColor1(bottomColor);
	Color startColor2(bottomColor);

	Color colorDy1 = (topColor - bottomColor) / dy1;	// color change along the top->bottom line
	Color colorDy2 = (middleColor - bottomColor) / dy2;	// color change along the other line

	double z1 = bottom->z();
	double z2 = bottom->z();

	double dz1 = (top->z() - bottom->z()) / dy1;
	double dz2 = (middle->z() - bottom->z()) / dy2;

	int y = bottom->y();
	while(y < middle->y())
	{
		drawSmoothHLine(x1, y, z1, x2, z2, startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
	// now for the top 'half' of a triangle
	if(middle->y() == top->y())
		dx2 = 0.0;
	else
		dx2 = (top->x() - middle->x()) / dy3;
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	z2 = middle->z();
	startColor2 = middleColor;
	colorDy2 = (topColor - middleColor) / dy3;
	dz2 = (top->z() - middle->z()) / dy3;


	while(y <= top->y())
	{
		drawSmoothHLine(x1, y, z1, x2, z2, startColor1, startColor2);
		startColor1 += colorDy1;
		startColor2 += colorDy2;
		x1 += dx1;
		x2 += dx2;
		z1 += dz1;
		z2 += dz2;
		++y;
	}
}

void Rasterizer::drawSmoothHLine(int x0, int y, double z0, int x1, double z1, const Color& c1, const Color& c2)
{
	if(x0 == x1)
	{
		return;
	}

	int x, xmax;
	double z;
	Color color, dcolor;
	double dz;

	if(x0 < x1)
	{
		x = x0;
		z = z0;
		xmax = x1;
		color = c1;
		dcolor = (c2 - c1) / (xmax - x);
		dz = (z1 - z0) / (xmax - x);
	}
	else
	{
		x = x1;
		z = z1;
		xmax = x0;
		color = c2;
		dcolor = (c1 - c2) / (xmax - x);
		dz = (z0 - z1) / (xmax - x);
	}

	while(x <= xmax)
	{
		putPixel(x, y, z, color);
		color += dcolor;
		z += dz;
		x++;
	}
}

void Rasterizer::drawSmoothTriangles(const OpenGL_state& state, VertexBuffer& vertexBuffer)
{
	assert(vertexBuffer.size() % 3 == 0);

	for (size_t i = 0; i < vertexBuffer.size(); i += 3)
	{
		const Vertex4& v1 = vertexBuffer[i + 0];
		const Vertex4& v2 = vertexBuffer[i + 1];
		const Vertex4& v3 = vertexBuffer[i + 2];

		if (!cullFace(v1, v2, v3))
			drawSmoothTriangle(v1, v2, v3);
	}
}

void Rasterizer::shadeVertices(VertexBuffer& buffer)
{
	assert(buffer.getCoordinateType() == EYE);

	for (size_t i = 0; i < buffer.size(); ++i)
	{
		shadeVertex(buffer[i]);
	}
}

void Rasterizer::shadeVertex(Vertex4& vertex)
{
	Color color(Black);
	const Lights& lights = _state->getLights();

	// TODO: select between front and back materials
	const Material& material = vertex.getMaterialFront();

	for (unsigned i = 0; i < lights.numberOfLights(); ++i)
	{
		const Light& light = lights[i];

		if (!light.isEnabled())
			continue;

		Vector3d vLight;

		if(light.isDirectional())	// TODO: wtf am I doing here?? (old stuff...need to re-think)
			vLight = light.getPosition().start<3>();
		else
			vLight = light.getPosition().start<3>() - vertex.getPosition().start<3>();

		vLight.normalize();

		// TODO: front/back material!!!
		double angle_cos(vertex.getNormal().dot(vLight));
		if(angle_cos <= 0)
			//return Black;
			//angle_cos *= -1;	// FIXME:::: !!!!! read the papers
			color = Black;

		color += (material.getAmbient()*light.getAmbient()) + ((material.getDiffuse() * light.getDiffuse()) * angle_cos);
	}

	color += material.getAmbient() * lights.getGlobalAmbientLight();

	vertex.setColor(color);
}

void Rasterizer::putPixel(int x, int y, float z, const ggl::ColorRGB& color)
{
	assert(_colorBuffer != NULL);

	if (x < 0 || y < 0
			|| x >= static_cast<int>(_colorBuffer->width())
			|| y >= static_cast<int>(_colorBuffer->height()))
		return;

	// TODO: use glDepthFunc and eventually the stencil...?

	const unsigned buffer_offset = x + _colorBuffer->width() * y;
	bool z_passed = false;

	z_passed = z < _zBuffer[buffer_offset];

	if (z_passed)
	{
		_zBuffer[buffer_offset] = z;
		_colorBuffer->putPixel(x, _colorBuffer->height() - y - 1, color);	//need to reverse y
	}
}

void Rasterizer::removePendingVerticesFromTrianglesList(VertexBuffer& buffer)
{
	// we work with GL_TRIANGLES list => vertices must be in triples
	size_t i = buffer.size() % 3;
	if (i > 0)
		buffer.popBack();

	if (i > 0)
		buffer.popBack();
}


}
}
