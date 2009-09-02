/*
 * PixelRGB.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef COLORRGB_H_
#define COLORRGB_H_


namespace ggl {

class ColorRGB {
public:
	ColorRGB(float r = 0, float g = 0, float b = 0):
		_r(r), _g(g), _b(b){}

	float getR()const {return _r;}
	float	getG()const {return _g;}
	float	getB()const {return _b;}

	// return normalized
	float getR_norm()const {return (_r>1.0)?1:(_r<0.0)?0:_r;}
	float	getG_norm()const {return (_g>1.0)?1:(_g<0.0)?0:_g;}
	float	getB_norm()const {return (_b>1.0)?1:(_b<0.0)?0:_b;}

	// return color in the interval <0;255>; TODO: probably should round() it
	int getR_int_norm()const {return (_r>1.0)?255:(_r<0.0)?0:_r*255;}
	int getG_int_norm()const {return (_g>1.0)?255:(_g<0.0)?0:_g*255;}
	int getB_int_norm()const {return (_b>1.0)?255:(_b<0.0)?0:_b*255;}

	// return int without normalization
	int getR_int()const {return _r*255;}
	int getG_int()const {return _g*255;}
	int getB_int()const {return _b*255;}

	float	r()const {return getR();}
	float	g()const {return getG();}
	float	b()const {return getB();}

	float	r_norm()const {return getR_norm();}
	float	g_norm()const {return getG_norm();}
	float	b_norm()const {return getB_norm();}

	float	r_int_norm()const {return getR_int_norm();}
	float	g_int_norm()const {return getG_int_norm();}
	float	b_int_norm()const {return getB_int_norm();}

	void clear(const float & value = 0) {_r = _g = _b = value;}	// should this be virtual? redefined in PixelRGBA?

	virtual ColorRGB operator-(const ColorRGB& p2)const {return ColorRGB(this->r() - p2.r(), this->g() - p2.g(), this->b() - p2.b());}
	virtual ColorRGB& operator+=(const ColorRGB& p2){_r += p2.r(); _g += p2.g(); _b += p2.b();return *this;}

private:
	virtual void normalize();

protected:
	float _r, _g, _b;
};

inline ColorRGB operator/(const ColorRGB& p, const float f) {
	return ColorRGB(p.r()/f, p.g()/f, p.b()/f);
}

}

#endif /* PIXELRGB_H_ */
