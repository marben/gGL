/*
 * PngWriter.h
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#ifndef PNGWRITER_H_
#define PNGWRITER_H_

#include <iostream>
#include <png.h>
#include <stdexcept>

#include "Image.h"

namespace ggl
{

class PngWriter
{
public:
	PngWriter(std::string outputFile = ""){_filename = outputFile;}

	template<typename T>
	void write(const ggl::Image2d<T>& image) throw(std::exception);

	void outputFile(const std::string output){_filename = output;}
	std::string outputFile()const {return _filename;}

private:
	std::string _filename;
};

template<typename T>
void PngWriter::write(const ggl::Image2d<T>& image) throw(std::exception)
{
	FILE* f;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	f = fopen(_filename.c_str(),"wb");
	if(f == NULL)
	{
		throw std::runtime_error("Opening png file for writing failed");
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL){
		fclose(f);
		throw std::runtime_error("Creating png header structure failed");
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL){
		png_destroy_write_struct(&png_ptr,NULL);
		fclose(f);
		throw std::runtime_error("Creating png info structure failed");
	}

	if(setjmp(png_jmpbuf(png_ptr))){	// libpng way of handling errors
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(f);
		throw std::runtime_error("Some error inside lipng occured during png export..");
	}

	png_init_io(png_ptr, f);

	png_set_IHDR(png_ptr, info_ptr, image.width(), image.height(), 8, PNG_COLOR_TYPE_RGB,
		 	PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_text pngtext[1];	// FIXME: should add something like Title, Author, ....
	pngtext[0].key = (char*)"Software:";
	pngtext[0].text = (char*)"GiX's ggl library output";
	pngtext[0].compression = PNG_TEXT_COMPRESSION_NONE;

	png_set_text(png_ptr, info_ptr, pngtext, 1);
	png_write_info(png_ptr, info_ptr);

	png_bytep p_row;
	p_row = (png_byte*)malloc((size_t) (image.width()*3));	// we write row by row
	if(p_row == NULL){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(f);
		throw std::runtime_error("Allocation of memory for image write row failed..");
	}

	for(size_t i=0; i<image.height(); ++i){
		for(size_t j=0; j<image.width(); ++j){
			p_row[3*j]=image.getPixel(j, i).getR_int();
			p_row[3*j+1]=image.getPixel(j, i).getG_int();
			p_row[3*j+2]=image.getPixel(j, i).getB_int();
		}
		png_write_row(png_ptr, p_row);
	}
	png_write_end(png_ptr, info_ptr);

	free(p_row);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(f);
}

}

#endif /* PNGWRITER_H_ */
