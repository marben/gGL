/*
 * Image.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "Image2d.h"
#include "PixelRGB.h"
#include "PixelRGBA.h"

namespace ggl
{

typedef Image2d<PixelRGB> Image2dRGB;
typedef Image2d<PixelRGBA> Image2dRGBA;

typedef Canvas<PixelRGB> CanvasRGB;
typedef Canvas<PixelRGBA> CanvasRGBA;

static const PixelRGB Red(1,0,0);
static const PixelRGB Green(0,1,0);
static const PixelRGB Blue(0,0,1);
static const PixelRGB Black(0,0,0);
static const PixelRGB White(1,1,1);
static const PixelRGB Cyan(0,1,1);
static const PixelRGB Magenta(1,0,1);
static const PixelRGB Yellow(1,1,0);
}

#endif /* IMAGE_H_ */
