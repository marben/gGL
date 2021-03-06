/*
 * VertexBuffer.cpp
 *
 *  Created on: May 18, 2010
 *      Author: gix
 */

#include <Eigen/LU>
#include "VertexBuffer.h"

namespace ggl
{
namespace ogl
{

void VertexBuffer::transformToEyeCoordinates(bool normalizeNormals)
{
	// TODO: normals should be transformed in one run along with the vertex bases .. probably better for cache memory

	assert(_coordinateType == OBJECT);

	transformVertices(_matrices.getModelViewMatrix(), EYE);

	// now we need to transform the normals
	// Nice explanation of the the way to transform normals can be found here:
	// http://www.songho.ca/opengl/gl_normaltransform.html
	// and here:
	// http://www.opengl.org/documentation/specs/version1.1/glspec1.1/node26.html


	Matrix3 modelviewInverseTransposed;
	//modelviewInverseTransposed = _matrices.getModelViewMatrix().corner<3,3>(Eigen::TopLeft);
	_matrices.getModelViewMatrix().corner<3,3>(Eigen::TopLeft).computeInverse(&modelviewInverseTransposed);
	modelviewInverseTransposed.transposeInPlace();


	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		Vertex4& vert = _vertices[i];

		vert.multiplyNormal(modelviewInverseTransposed);	// vec(transposed) * Matrix == Matrix(transposed) * vec

		if (normalizeNormals)
			vert.normalizeNormal();
	}
}

void VertexBuffer::transformToWindowCoordinates(const GlViewport& viewport, const GlDepthRange& depthRange)
{
	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		Vertex4& vertex = _vertices[i];

		vertex.x() = (vertex.x() + 1)*(viewport.w / 2) + viewport.x;
		vertex.y() = (vertex.y() + 1)*(viewport.h / 2) + viewport.y;

		// z is mapped to zNear -> zFar
		// http://www.songho.ca/opengl/gl_transform.html
		// TODO: uncomment and test
		vertex.z() = ((depthRange.zFar - depthRange.zNear)/2) * vertex.z() + (depthRange.zFar + depthRange.zNear) / 2;
	}
}

void VertexBuffer::transformToClipCoordinates()
{
	//in case the modelview matrix hasn't been applied yet, we multiply projection and modelview matrix and transform directly to clip coordinates
	if (_coordinateType == OBJECT)
	{
		Matrix4d matrix = _matrices.getProjectionMatrix() * _matrices.getModelViewMatrix();
		transformVertices(matrix, CLIP);
	}
	else
	{
		assert(_coordinateType == EYE);

		transformVertices(_matrices.getProjectionMatrix(), CLIP);
	}
}

void VertexBuffer::transformToNormalizedDeviceCoordinates()
{
	assert(_coordinateType == CLIP);

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		_vertices[i].applyPerspectiveDivision();
	}
}

//multiplies each vector with the matrix and set's se coordinateType to newCoordinateType
void VertexBuffer::transformVertices(const Matrix4d& matrix, CoordinateType newCoordinateType)
{
	std::vector<Vertex4>::iterator vertexIterator, end;

	for (vertexIterator = _vertices.begin(), end = _vertices.end(); vertexIterator != end; ++vertexIterator)
	{
		(*vertexIterator) *= matrix;
	}

	_coordinateType = newCoordinateType;
}

void VertexBuffer::clear()
{
	_vertices.clear();
}

void VertexBuffer::add(Vertex4& vertex)
{
	_vertices.push_back(vertex);
}


}	// namespace ogl
}
