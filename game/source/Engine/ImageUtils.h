#ifndef _IMAGE_UTILS_H
#define _IMAGE_UTILS_H
#include "libpng/png.h"
#include "Texture.h"
void read_png_file(const char* file_name,Texture& tex, bool flipY = false);
void write_png_file(const char* file_name, png_bytep * row_pointers, int width, int height, png_byte color_type = PNG_COLOR_TYPE_RGBA, png_byte bit_depth = 32);

#endif