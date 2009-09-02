/*
 * PixelRGBA.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef COLORRGBA_H_
#define COLORRGBA_H_

#include "ColorRGB.h"

namespace ggl
{

class ColorRGBA : public ColorRGB
{
public:
	ColorRGBA(const float & r = 0, const float & g = 0, const float & b = 0, const float & a = 1):
		ColorRGB(r, g, b), _alpha(a){}
	ColorRGBA(const ColorRGB & pixel):
		ColorRGB(pixel), _alpha(1){}

	float	getAlpha()const {return _alpha;}
	float	alpha()const {return _alpha;}
	float	a()const {return getAlpha();}

	virtual void clear(const float & value = 0) {_alpha = value; ColorRGB::clear(value);}
	//virtual void clear(const float& value = 0, const float& alpha = 1) {ColorRGB::clear(value); _alpha = alpha;}

	virtual ColorRGBA operator-(const ColorRGBA& p2)const {return ColorRGBA(this->r() - p2.r(), this->g() - p2.g(), this->b() - p2.b(), this->a() - p2.a());}
	virtual ColorRGBA& operator+=(const ColorRGBA& p2){_r += p2.r(); _g += p2.g(); _b += p2.b(); _alpha += p2.a();return *this;}

protected:
	float _alpha;
};

inline ColorRGBA operator/(const ColorRGBA& p, const float f) {
	return ColorRGBA(p.r()/f, p.g()/f, p.b()/f, p.alpha()/f);
}

}

#endif /* PIXELRGBA_H_ */
