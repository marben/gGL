/*
 * SDL_Display.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef SDL_DISPLAY_H_
#define SDL_DISPLAY_H_

#include <SDL/SDL.h>
#include "PixelRGB.h"
#include "Image.h"
#include <iostream>


// a class to display images in sdl output
class SDL_Display
{
public:
	SDL_Display();
	void init(size_t x, size_t y);

	virtual ~SDL_Display();

	template <typename T>
	void displayImage(const ggl::Image2d<T> &image);

	void redraw();

	void putPixel(const size_t & x, const size_t & y, const ggl::PixelRGB & pixel);	// can be called even without ::beginWrite(), but will be slower

	void beginWrite();	// when beginning to write to surface it is locked
	void endWrite();	// when end of writing to surface

private:
	SDL_Surface* _screen;
	bool _surface_locked;
	size_t _x, _y;
};

template <typename T>
void SDL_Display::displayImage(const ggl::Image2d<T> &image)
{
	if(_x != image.getX() || _y != image.getY())
	{
		std::cerr<<"trying to display incompatible image"<<std::endl;
		return;
	}

	this->beginWrite();
	for(size_t i = 0; i < _x; ++i)
	{
		for(size_t j = 0; j < _y; ++j)
		{
			this->putPixel(i, j, image.getPixel(i,j));
		}
	}
	this->endWrite();

	this->redraw();
}

#endif /* SDL_DISPLAY_H_ */
