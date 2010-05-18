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
	//Vertex4_(T x, T y, T z, T w, const Color_t& color):_vertex(Matrix<T, 4, 1>(x, y, z, w)), _color(color){}
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

	//const Color_t& color()const {return _color;}
	const Color_t& getColor()const {return _color;}

	const Material& getMaterialFront()const {return _materialFront;}

	const Material& getMaterialBack()const {return _materialBack;}

	const Matrix<T, 3, 1/*, Eigen::DontAlign*/> getNormal()const {return _normal;}

	Vertex4_<T, Color_t>& operator*=(const Matrix4d& matrix) { _vertex = matrix * _vertex; return *this;}

	const Matrix<T, 4, 1/*, Eigen::DontAlign*/>& getCoordinates() const {return _vertex;}

	const Matrix<T, 4, 1/*, Eigen::DontAlign*/>& getPosition() const {return _vertex;}

	bool lightingEnabled() const {return _lightingEnabled;}

private:
	Matrix<T, 4, 1/*, Eigen::DontAlign*/> _vertex;	// TODO: figure out, how to make it possible to pass this class with aligned Eigen::Matrix to std::Vector..  (maybe write spcial copy constructor or define aligned allocator in the vector?..) .. maybe a cvs version of libeigen?

	Matrix<T, 3, 1/*, Eigen::DontAlign*/> _normal;

	Color_t _color;

	Material _materialFront, _materialBack;

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
