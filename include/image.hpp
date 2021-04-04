#include <functional>
#include <stdlib.h>
#include <string>
#include <png.h>

namespace im {

   struct pixel {
      png_byte r, g, b;

      pixel() {}
      pixel(png_byte r, png_byte g, png_byte b) : r(r), g(g), b(b) {}
   };

   template<unsigned width, unsigned height>
   struct image {

   private:
      pixel* pixels;

   public:

      image() {
         pixels = new pixel[width * height];
      }

      void paint_pixels(std::function<pixel(unsigned, unsigned)> painter) {
         int idx = 0;
         for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++)
               pixels[idx++] = painter(i,j);
      }

      void paint_pixels_parallel(std::function<pixel(unsigned, unsigned)> painter) {
         #pragma omp parallel for schedule(dynamic, 16)
         for (int j = 0; j < height; j++) {
            int idx = j*width;
            for (int i = 0; i < width; i++) {
               pixels[idx++] = painter(i,j);
            }
         }
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
         
         png_byte* pixel_grid[height];
         for (int i = 0; i < height; i++)
            pixel_grid[i] = (png_byte*)&pixels[i*width];
         png_write_image(png_ptr, pixel_grid);
         png_write_end(png_ptr, NULL);
         png_destroy_write_struct(&png_ptr, &info_ptr);

         fclose(f);
         f = NULL;

         return 0;
      }

      ~image() {
         delete[] pixels;
      }
   };

}
