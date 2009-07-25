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


private:
	float _alpha;
};

}

#endif /* PIXELRGBA_H_ */
