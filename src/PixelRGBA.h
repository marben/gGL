/*
 * PixelRGBA.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef PIXELRGBA_H_
#define PIXELRGBA_H_

#include "PixelRGB.h"

namespace ggl
{

class PixelRGBA : public PixelRGB
{
public:
	PixelRGBA(const float & r = 0, const float & g = 0, const float & b = 0, const float & a = 1):
		PixelRGB(r, g, b), _alpha(a){}
	PixelRGBA(const PixelRGB & pixel):
		PixelRGB(pixel), _alpha(1){}

	float	getAlpha()const {return _alpha;}
	float	alpha()const {return _alpha;}
	float	a()const {return getAlpha();}

	virtual void clear(const float & value = 0) {_alpha = value; PixelRGB::clear(value);}
	//virtual void clear(const float& value = 0, const float& alpha = 1) {PixelRGB::clear(value); _alpha = alpha;}

	virtual PixelRGBA operator-(const PixelRGBA& p2)const {return PixelRGBA(this->r() - p2.r(), this->g() - p2.g(), this->b() - p2.b(), this->a() - p2.a());}
	virtual PixelRGBA& operator+=(const PixelRGBA& p2){_r += p2.r(); _g += p2.g(); _b += p2.b(); _alpha += p2.a();return *this;}

protected:
	float _alpha;
};

inline PixelRGBA operator/(const PixelRGBA& p, const float f) {
	return PixelRGBA(p.r()/f, p.g()/f, p.b()/f, p.alpha()/f);
}

}

#endif /* PIXELRGBA_H_ */
