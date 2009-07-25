/*
 * PixelRGB.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef PIXELRGB_H_
#define PIXELRGB_H_


namespace ggl {

class PixelRGB {
public:
	PixelRGB(float r = 0, float g = 0, float b = 0):
		_r(r), _g(g), _b(b){}

	float getR()const {return _r;}
	float	getG()const {return _g;}
	float	getB()const {return _b;}

	// return color in the interval <0;255>; FIXME: probably should round() it
	int getR_int()const {return (_r>1.0)?255:(_r<0.0)?0:_r*255;}
	int getG_int()const {return (_g>1.0)?255:(_g<0.0)?0:_g*255;}
	int getB_int()const {return (_b>1.0)?255:(_b<0.0)?0:_b*255;}

	float	r()const {return getR();}
	float	g()const {return getG();}
	float	b()const {return getB();}

	void clear(const float & value = 0) {_r = _g = _b = value;}	// should this be virtual? redefined in PixelRGBA?

private:
	virtual void normalize();

private:
	float _r, _g, _b;
};

}

#endif /* PIXELRGB_H_ */
