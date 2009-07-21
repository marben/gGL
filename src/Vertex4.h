/*
 * Vertex4.h
 *
 *  Created on: Jul 21, 2009
 *      Author: gix
 */

#include <Eigen/Core>

#ifndef VERTEX4_H_
#define VERTEX4_H_

namespace ggl{

USING_PART_OF_NAMESPACE_EIGEN

template<typename T, typename Color_t>
class Vertex4
{
public:
	Vertex4(T x, T y, T z, T w, const Color_t& color):_vertex(Matrix<T, 4, 1>(x, y, z, w)), _color(color){}
	Vertex4(const Matrix<T, 4, 1>& vector, const Color_t& color):_vertex(vector), _color(color){}

	T x()const {return _vertex[0];}
	T y()const {return _vertex[1];}
	T z()const {return _vertex[2];}
	T w()const {return _vertex[3];}
	const Color_t& color()const {return _color;}

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	Matrix<T, 4, 1> _vertex;	// 4d vector
	Color_t _color;
};


} // namespace ggl

#endif /* VERTEX4_H_ */
