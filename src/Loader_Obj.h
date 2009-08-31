/*
 * Parser_Obj.h
 *
 *  Created on: Aug 20, 2009
 *      Author: gix
 */

#ifndef PARSER_OBJ_H_
#define PARSER_OBJ_H_

#include <vector>
#include <fstream>

#include "Object3d.h"

// TODO: rewrite using boost::spirit

namespace ggl
{

class Loader_Obj
{
public:
	Loader_Obj(const char filename[]);
	virtual ~Loader_Obj();
	Object3d getObject(const std::string name);
private:
	void loadObject();
	Vertex loadFaceTriplet(std::string);
	std::ifstream file;

	std::vector<Object3d> objects;	// list of all read objects	// TODO only load objects on request

	std::vector<Vertex> vert;
	std::vector<Vertex> normals;
};


}	// namespace ggl
#endif /* PARSER_OBJ_H_ */
