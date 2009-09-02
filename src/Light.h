#ifndef LIGHT_H_
#define LIGHT_H_

#include "GL/gl_types.h"

namespace ggl {
namespace ogl
{

class Light
{
public:
	Light():_enabled(false), _position(0,0,1,0), _ambient(0,0,0,1), _diffuse(0,0,0,1), _specular(0,0,0,1), _spot_direction(0,0, -1){}
	void enable(bool enabled = true) {_enabled = enabled;}
	void disable() {enable(false);}
	void setAmbient(float r, float g, float b, float a) {_ambient.setR(r); _ambient.setG(g); _ambient.setB(b); _ambient.setA(a);}
	void setDiffuse(float r, float g, float b, float a) {_diffuse.setR(r); _diffuse.setG(g); _diffuse.setB(b); _diffuse.setA(a);}
	void setSpecular(float r, float g, float b, float a) {_specular.setR(r); _specular.setG(g); _specular.setB(b); _specular.setA(a);}
	void setPosition(const Point4d& position) { _position = position;}
	void setSpotDirection(const Point3d& direction) {_spot_direction = direction;}
private:
	bool _enabled;
	Point4d _position;
	ColorRGBA _ambient, _diffuse, _specular;
	Point3d _spot_direction;
};

}
}


#endif
