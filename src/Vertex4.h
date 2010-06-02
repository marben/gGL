/*
 * Vertex4.h
 *
 *  Created on: Jul 21, 2009
 *      Author: gix
 */


#ifndef VERTEX4_H_
#define VERTEX4_H_

#include <Eigen/Core>

#include <iostream>

#include "Material.h"
#include "GL/gl_types.h"

USING_PART_OF_NAMESPACE_EIGEN

namespace ggl {
namespace ogl
{


template<typename T, typename Color_t>
class Vertex4_
{
public:
	Vertex4_(const Matrix<T, 4, 1>& vector, const Matrix<T, 3, 1>& normal, const Color_t& color, const Material& frontMaterial, const Material& backMaterial, bool lightingEnabled):
		_vertex(vector), _normal(normal), _color(color),
		_materialFront(frontMaterial), _materialBack(backMaterial), _lightingEnabled(lightingEnabled){}

	T x()const {return _vertex[0];}
	T y()const {return _vertex[1];}
	T z()const {return _vertex[2];}
	T w()const {return _vertex[3];}

	T& x(){return _vertex[0];}
	T& y(){return _vertex[1];}
	T& z(){return _vertex[2];}
	T& w(){return _vertex[3];}

	const Color_t& getColor()const {return _color;}
	void setColor(const Color_t color) {_color = color;}

	const Material& getMaterialFront()const {return _materialFront;}

	const Material& getMaterialBack()const {return _materialBack;}

	const Matrix<T, 3, 1>& getNormal()const {return _normal;}

	void normalizeNormal() {_normal.normalize();}	// FIXME: delete

	//Vertex4_<T, Color_t>& operator*=(const Matrix4d& matrix) { _vertex = matrix * _vertex; _normal = matrix.corner<3,3>(Eigen::TopLeft) * _normal; return *this;}
	Vertex4_<T, Color_t>& operator*=(const Matrix4d& matrix) { _vertex = matrix * _vertex; return *this;}

	void multiplyNormal(const Matrix<T, 3, 3>& matrix)
	{
		_normal = matrix * _normal;
	}

	const Matrix<T, 4, 1>& getCoordinates() const {return _vertex;}

	const Matrix<T, 4, 1>& getPosition() const {return _vertex;}

	bool lightingEnabled() const {return _lightingEnabled;}	// FIXME: get rid of this

	void applyPerspectiveDivision() {
		assert(w() != 0.0);
		_vertex /= w();	// TODO: is it faster to divide all 4 values, or just x, y, z ??
	}

private:
	Matrix<T, 4, 1> _vertex;

	Matrix<T, 3, 1> _normal;

	Color_t _color;

	Material _materialFront, _materialBack;	// TODO: material should probably be property of the state, not of the vertex

	bool _lightingEnabled;

// this is for libeigen:
	enum { NeedsToAlign = (sizeof(Matrix<T, 4, 1>)%16)==0 };
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
};

typedef Vertex4_<Real, Color> Vertex4;

}	// namespace ogl
} // namespace ggl

#endif /* VERTEX4_H_ */
