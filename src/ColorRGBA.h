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

// TODO: implement using eigen::Matrix .. free vectorization of addition and such...
class ColorRGBA : public ColorRGB
{
public:
	ColorRGBA(const float & r = 0, const float & g = 0, const float & b = 0, const float & a = 1):
		ColorRGB(r, g, b), _alpha(a){}
	ColorRGBA(const ColorRGB & pixel):
		ColorRGB(pixel), _alpha(1){}

	float	getAlpha()const {return _alpha;}
	float getA()const {return getAlpha();}
	void setAlpha(float a) {_alpha = a;}
	void setA(float a) {setAlpha(a);}
	float	alpha()const {return _alpha;}
	float	a()const {return getAlpha();}

	virtual void clear(const float & value = 0) {_alpha = value; ColorRGB::clear(value);}
	virtual void set(float r, float g, float b, float a) {setR(r);setG(g);setB(b);setA(a);}


	//FIXME: following color-mixing may not make sense, but is used somewhere...re-think..
	virtual ColorRGBA operator-(const ColorRGBA& p2)const {return ColorRGBA(this->r() - p2.r(), this->g() - p2.g(), this->b() - p2.b(), this->a() - p2.a());}
	virtual ColorRGBA& operator+=(const ColorRGBA& p2) {_r += p2.r(); _g += p2.g(); _b += p2.b(); _alpha += p2.a();return *this;}
	virtual ColorRGBA& operator-=(const ColorRGBA& p2) {_r -= p2.r(); _g -= p2.g(); _b -= p2.b(); _alpha -= p2.a();return *this;}
	virtual ColorRGBA& operator/=(const float f) {_r /= f; _g /= f; _b /= f; _alpha /= f;return *this;}

protected:
	float _alpha;
};

inline ColorRGBA operator/(const ColorRGBA& p, const float f) {
	return ColorRGBA(p.r()/f, p.g()/f, p.b()/f, p.alpha()/f);
}

inline ColorRGBA operator*(const ColorRGBA& p, const float f) {	// we don't multiply the alpha value!!
	return ColorRGBA(p.r()*f, p.g()*f, p.b()*f, p.alpha());
}

inline ColorRGBA operator +(const ColorRGBA& a, const ColorRGBA& b) {
	return ColorRGBA(a.getR() + b.getR(), a.getG() + b.getG(), a.getB() + b.getB(), a.getA() + b.getA());
}

inline ColorRGBA operator *(const ColorRGBA& a, const ColorRGBA& b) {
	return ColorRGBA(a.getR() * b.getR(), a.getG() * b.getG(), a.getB() * b.getB(), a.getA() * b.getA());
}

}

#endif /* PIXELRGBA_H_ */
