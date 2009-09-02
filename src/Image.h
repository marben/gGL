/*
 * Image.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "Image2d.h"
#include "ColorRGB.h"
#include "ColorRGBA.h"

namespace ggl
{

typedef Image2d<ColorRGB> Image2dRGB;
typedef Image2d<ColorRGBA> Image2dRGBA;

typedef Canvas<ColorRGB> CanvasRGB;
typedef Canvas<ColorRGBA> CanvasRGBA;

static const ColorRGB Red(1,0,0);
static const ColorRGB Green(0,1,0);
static const ColorRGB Blue(0,0,1);
static const ColorRGB Black(0,0,0);
static const ColorRGB White(1,1,1);
static const ColorRGB Cyan(0,1,1);
static const ColorRGB Magenta(1,0,1);
static const ColorRGB Yellow(1,1,0);
}

#endif /* IMAGE_H_ */
