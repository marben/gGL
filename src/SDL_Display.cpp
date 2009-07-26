/*
 * SDL_Display.cpp
 *
 *  Created on: Mar 4, 2009
 *      Author: gix
 */

#include "SDL_Display.h"
#include <iostream>

Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void SDL_Display::putPixel(const size_t & x, const size_t & y, const ggl::PixelRGB & pixel)
{
	if(_surface_locked)
	{
		putpixel(_screen, x, y, SDL_MapRGB(_screen->format, (pixel.r()>1)?255:(pixel.r()<0)?0:pixel.r()*255, (pixel.g()>1)?255:(pixel.g()<0)?0:pixel.g()*255, (pixel.b()>1)?255:(pixel.b()<0)?0:pixel.b()*255));
	}
	else
	{
		beginWrite();
		putpixel(_screen, x, y, SDL_MapRGB(_screen->format, (pixel.r()>1)?255:(pixel.r()<0)?0:pixel.r()*255, (pixel.g()>1)?255:(pixel.g()<0)?0:pixel.g()*255, (pixel.b()>1)?255:(pixel.b()<0)?0:pixel.b()*255));
		endWrite();
	}
}

void SDL_Display::clearScreen(const ggl::PixelRGB & pixel)
{
	SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, (pixel.r()>1)?255:(pixel.r()<0)?0:pixel.r()*255, (pixel.g()>1)?255:(pixel.g()<0)?0:pixel.g()*255, (pixel.b()>1)?255:(pixel.b()<0)?0:pixel.b()*255));
}

SDL_Display::SDL_Display():
	 _canvasRGB(this), _screen(NULL), _x(0), _y(0)
{
}

void SDL_Display::init(size_t x, size_t y)
{
	std::cout<<"Initializing SDL output"<<std::endl;
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD) < 0)
		std::cerr<<"Error initializing SDL!"<<std::endl;
	_x = x;
	_y = y;

	_screen = SDL_SetVideoMode(x, y, 8, SDL_SWSURFACE);
	_surface_locked = false;

}

void SDL_Display::beginWrite()
{
	SDL_LockSurface(_screen);
	_surface_locked = true;
}

void SDL_Display::endWrite()
{
	SDL_UnlockSurface(_screen);
	_surface_locked = false;
}

void SDL_Display::redraw()
{
	SDL_UpdateRect(_screen, 0, 0, _screen->w, _screen->h);
}

SDL_Display::~SDL_Display()
{
	SDL_Quit();
}



/* putpixel && getpixel from SDL documentation */

 /*
  * Return the pixel value at (x, y)
  * NOTE: The surface must be locked before calling this!
  */
 Uint32 getpixel(SDL_Surface *surface, int x, int y)
 {
     int bpp = surface->format->BytesPerPixel;
     /* Here p is the address to the pixel we want to retrieve */
     Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

     switch (bpp) {
     case 1:
         return *p;

     case 2:
         return *(Uint16 *)p;

     case 3:
         if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
             return p[0] << 16 | p[1] << 8 | p[2];
         else
             return p[0] | p[1] << 8 | p[2] << 16;

     case 4:
         return *(Uint32 *)p;

     default:
         return 0;       /* shouldn't happen, but avoids warnings */
     } // switch
 }

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;

   default:
        break;           /* shouldn't happen, but avoids warnings */
    } // switch
}
