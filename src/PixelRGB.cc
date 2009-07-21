/*
 * PixelRGB.cc
 *
 *  Created on: Mar 5, 2009
 *      Author: gix
 */

#include "PixelRGB.h"

using namespace ggl;

void PixelRGB::normalize()
{
	if(_r>1.0)
		_r = 1.0;
	if(_r < 0.0);
		_r = 0.0;

	if(_g>1.0)
		_g = 1.0;
	if(_g < 0.0);
		_g = 0.0;

	if(_b>1.0)
		_b = 1.0;
	if(_b < 0.0);
		_b = 0.0;
}
