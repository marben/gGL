/*
 * Parser_Obj.cc
 *
 *  Created on: Aug 20, 2009
 *      Author: gix
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "Loader_Obj.h"
using namespace std;

namespace ggl
{

Loader_Obj::Loader_Obj(const char filename[])
{
	file.open(filename);

	if(!file.is_open())
	{
		cerr<<"error loading file"<<endl;
		return;
	}
	else
	{
		cout<<"file "<<filename<<" loaded"<<endl;
	}

	string lineStart;

	while( !file.eof() )
	{
		file>>lineStart;
		if(lineStart == "o")
		{
			loadObject();
		}
		else
			getline(file, lineStart);
	}
}

Object3d
Loader_Obj::getObject(const std::string name)
{
	for(unsigned int i = 0; i<objects.size(); i++)
	{
		if(objects[i].getName() == name)
			return objects[i];
	}
	cerr<<"couldn't find object "<<name<<endl;
	return Object3d();
}

void
Loader_Obj::loadObject()
{
	Object3d obj;
	string str;

	stringstream ss;

	double x, y, z;
	Face face;

	file>>str;
	cout<<"Nacitam objekt: "<<str<<endl;
	obj._name = str;
	file>>str;

	while( !file.eof() && str != "o")
	{
		if(str == "v")
		{
			//cout<<"loading vertex ";
			file>>x>>y>>z;
			//cout<<"["<<x<<", "<<y<<", "<<z<<"]"<<endl;
			vert.push_back(Vertex(x, y, z));
		}

		if(str == "vn")
		{
			//cout<<"loading normal"<<endl;
			file>>x>>y>>z;
			normals.push_back(Vertex(x, y, z));
		}


		if(str == "f")	// face bude vzdy trojuhelnik!!
		{
			Vertex vertex;

			//cout<<"pridavam face:";

			file>>str;
			vertex = loadFaceTriplet(str);
			face.a = vert[(size_t)(vertex.x)-1];
			//cout<<"["<<face.a.x<<", "<<face.a.y<<", "<<face.a.z<<" ] ";

			file>>str;
			vertex = loadFaceTriplet(str);
			face.b = vert[(size_t)(vertex.x)-1];
			//cout<<"["<<face.b.x<<", "<<face.b.y<<", "<<face.b.z<<" ] ";

			file>>str;
			vertex = loadFaceTriplet(str);
			face.c = vert[(size_t)(vertex.x)-1];
			//cout<<"vertex.x je:"<<vertex.x<<" ";
			//cout<<"["<<face.c.x<<", "<<face.c.y<<", "<<face.c.z<<" ] "<<endl;
			face.normal = normals[(size_t)(vertex.z)-1];
			//cout<<"nastavuju facu normalu: "<<vertex.z<<endl;


			obj.faces.push_back(face);

		}

		file>>str;
	}

	objects.push_back(obj);
	cout<<"prave nacteny objekt ma "<<objects[0].faces.size()<<" trojuhelniku"<<endl;

	if(str=="o")
	{
		cerr<<"FIXME: more objects in a file. should fix the loader function..."<<endl;
		//loadObject();
	}
}

Vertex
Loader_Obj::loadFaceTriplet(std::string str)
{
	Vertex vertex;

	string s1, s2, s3;

	unsigned int slash1, slash2;
	unsigned int i = 0;
	while( (i<str.npos) && (str[i] != '/'))
		++i;
	slash1 = i;
	s1 = str.substr(0, slash1);
	++i;
	while( (i<str.npos) && (str[i] != '/'))
		++i;
	slash2 = i;
	if(slash2 - (slash1+1) > 0)
		s2 = str.substr(slash1+1, i);
	else
		s2 = "";
	s3 = str.substr(slash2+1, str.size());

	if(s1.empty())
		vertex.x = 0.0;
	else
	{
		stringstream ss1; // porad novy, protoze recyklovat jeden delalo problemy
		ss1<<s1;
		ss1>>vertex.x;
	}

	if(s2.empty())
		vertex.y = 0.0;
	else
	{
		stringstream ss2;
		ss2<<s2;
		ss2>>vertex.y;
	}

	if(s3.empty())
		vertex.z = 0.0;
	else
	{
		stringstream ss3;
		ss3<<s3;
		int wtf;
		ss3>>wtf;
		vertex.z = wtf;
		//cout<<"normala je v pohode:"<<vertex.z<<"|"<<s3<<"delka:"<<s3.size()<<endl;
	}

	//cout<<"triplet:"<<s1<<"/"<<s2<<"/"<<s3<<endl;
	return vertex;
}

Loader_Obj::~Loader_Obj()
{
	// TODO Auto-generated destructor stub
}

}
