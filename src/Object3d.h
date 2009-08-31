/*
 * Object3d.h
 *
 *  Created on: Aug 21, 2009
 *      Author: gix
 */

#include <string>
#include <vector>

#ifndef OBJECT3D_H_
#define OBJECT3D_H_


namespace ggl
{

class Vertex{
public:
	Vertex(const double _x = 0, const double _y = 0, const double _z = 0):x(_x), y(_y), z(_z){}
	double x, y, z;
};

class Face{
public:
	Vertex a, b, c;
	Vertex normal;
};

class Object3d
{
public:
	Object3d();
	void setName(const std::string& name)	{_name = name;}
	const std::string& getName() {return _name;}


public:	// TODO: set private
	std::string _name;	// name/id of the object. not necessarily unique
	std::vector<Face> faces;
};

}	// namespace ggl
#endif /* OBJECT3D_H_ */
