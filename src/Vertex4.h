/*
 * Vertex4.h
 *
 *  Created on: Jul 21, 2009
 *      Author: gix
 */

#include <Eigen/Core>

#include <iostream>

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
	//Vertex4(const Vertex4& vertex){std::cerr<<"Vertex4 copy constructor called"<<std::endl;}

	T x()const {return _vertex[0];}
	T y()const {return _vertex[1];}
	T z()const {return _vertex[2];}
	T w()const {return _vertex[3];}
	const Color_t& color()const {return _color;}

private:
	Matrix<T, 4, 1, Eigen::DontAlign> _vertex;	// TODO: figure out, how to make it possible to pass this class with aligned Eigen::Matrix to std::Vector..  (maybe write spcial copy constructor or define aligned allocator in the vector?..) .. maybe a cvs version of libeigen?
	Color_t _color;

// this is for libeigen:
	enum { NeedsToAlign = (sizeof(Matrix<T, 4, 1>)%16)==0 };
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
};


} // namespace ggl

#endif /* VERTEX4_H_ */
