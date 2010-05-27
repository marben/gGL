/*
 * VertexBuffer.h
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <vector>

#include "OpenGL_matrices.h"
#include "Vertex4.h"

namespace ggl {
namespace ogl {

// The lifespan of a vertex coordinates is:
// Object Coordinates * ModelViewMatrix => Eye Coordinates
// Eye coordinates * ProjectionMatrix => Clip Coordinates
// Clip Coordinates / w => Normalized Device Coordinates
// Normalized Device Coordinates . ViewPort => Window Coordinates

enum CoordinateType {
	OBJECT, EYE, CLIP, NORMALIZED, WINDOW
};

class VertexBuffer {
public:
	VertexBuffer(const OpenGL_matrices& matrices) :
		_matrices(matrices)
	{
	}

	void add(Vertex4& vertex);

	size_t size() const {return _vertices.size();}
	void popBack() {return _vertices.pop_back();}

	void transformToEyeCoordinates();	// apply's the modelview matrix
	void transformToClipCoordinates();	// apply's the projection matrix or both modelview and projection
	void transformToNormalizedDeviceCoordinates();	// apply's the perspective division
	void transformToWindowCoordinates(const GlViewport& viewport, const GlDepthRange& depthRange);

	CoordinateType getCoordinateType() const {return _coordinateType;}
	void setCoordinateType(CoordinateType type) {_coordinateType = type;}

	Vertex4& operator[] (size_t i) {return _vertices[i];}

	void clear();

private:
	void transformVertices(const Matrix4d& matrix, CoordinateType newCoordinateType);

private:
	const OpenGL_matrices& _matrices;
	std::vector<Vertex4> _vertices;
	CoordinateType _coordinateType;
};

}

}

#endif /* VERTEXBUFFER_H_ */
