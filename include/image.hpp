
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <pixel.hpp>
#include <frame.hpp>

#include <functional>
#include <stdlib.h>
#include <png.h>

namespace im {

   template<unsigned width, unsigned height>
   struct image {

      im::frame<width, height> _image;

      inline void paint_frame(std::function<pixel(unsigned, unsigned)> painter) {
         _image.paint(painter);
      }

      int write(const char* fname) {
         FILE* f = fopen(fname, "wb");
         if (!f) {
            return 1;
         }

         png_structp png_ptr;
         png_infop info_ptr;

         png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
         if (!png_ptr) {
            fclose(f);
            return 2;
         }

         info_ptr = png_create_info_struct(png_ptr);
         if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            fclose(f);
            return 3;
         }

         png_init_io(png_ptr, f);
         png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
         png_write_info(png_ptr, info_ptr);
         
         png_write_image(png_ptr, (png_byte**)_image._pixel_cols);
         png_write_end(png_ptr, NULL);
         png_destroy_write_struct(&png_ptr, &info_ptr);

         fclose(f);
         f = NULL;

         return 0;
      }
   };
}

#endif

