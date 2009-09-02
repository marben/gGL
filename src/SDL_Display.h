/*
 * SDL_Display.h
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#ifndef SDL_DISPLAY_H_
#define SDL_DISPLAY_H_

#include <SDL/SDL.h>
#include "ColorRGB.h"
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
	void displayImage(const ggl::Canvas<T>* image);

	void redraw();
	const size_t& width() {return _x;}
	const size_t& height() {return _y;}

	void putPixel(const size_t & x, const size_t & y, const ggl::ColorRGB & pixel);	// can be called even without ::beginWrite(), but will be slower
	void clearScreen(const ggl::ColorRGB & color);

	void beginWrite();	// when beginning to write to surface it is locked
	void endWrite();	// when end of writing to surface

	ggl::CanvasRGB* getRGBCanvas() {return &_canvasRGB;}


private:
	template<typename T>
	class SDL_Canvas : public ggl::Image2d<T>
	{
	public:
		SDL_Canvas():_display(NULL) {}
		SDL_Canvas(SDL_Display *display):_display(display){}
		const size_t& getX()const {return _display->width();}
		const size_t& getY()const {return _display->height();}

		void putPixel(const int & x, const int & y, const T & pixel);
		const T & getPixel(const int & x, const int & y)const {return ggl::Blue;}	// TODO: should do something sane here..
		void clear(const float& value = 0) {}; // TODO: implement cleaning
		void clear(const T& value) {_display->clearScreen(value);}	// TODO: implement

	private:
		SDL_Display* _display;
	};

private:
	SDL_Canvas<ggl::ColorRGB> _canvasRGB;

	SDL_Surface* _screen;
	bool _surface_locked;
	size_t _x, _y;
};

template <typename T>
void SDL_Display::SDL_Canvas<T>::putPixel(const int & x, const int & y, const T & pixel)
{
	if( (x < 0) || (y < 0) || (x >= static_cast<int>(this->width())) || (y >= static_cast<int>(this->height())))
		return;

	_display->putPixel(x, y, pixel);
}

template <typename T>
void SDL_Display::displayImage(const ggl::Canvas<T>* image)
{
	if(_x != image->getX() || _y != image->getY())
	{
		std::cerr<<"trying to display incompatible image"<<std::endl;
		return;
	}

	this->beginWrite();
	for(size_t i = 0; i < _x; ++i)
	{
		for(size_t j = 0; j < _y; ++j)
		{
			this->putPixel(i, j, image->getPixel(i,j));
		}
	}
	this->endWrite();

	this->redraw();
}

#endif /* SDL_DISPLAY_H_ */
