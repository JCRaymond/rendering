
#ifndef FRAME_HPP
#define FRAME_HPP

#include <pixel.hpp>

#include <functional>
#include <stdlib.h>
#include <png.h>

namespace im {

   template<unsigned width, unsigned height>
   struct frame {

      pixel* _pixels;
      pixel** _pixel_cols;

      frame() {
         _pixels = new pixel[width * height];
         _pixel_cols = new pixel*[height];
         for (int j = 0; j < height; j++)
            _pixel_cols[j] = &_pixels[j*width];
      }

      void paint(std::function<pixel(unsigned, unsigned)> painter) {
         #pragma omp parallel for schedule(guided)
         for (int j = 0; j < height; j++) {
            int idx = j*width;
            for (int i = 0; i < width; i++) {
               _pixels[idx++] = painter(i,j);
            }
         }
      }

      ~frame() {
         delete[] _pixels;
         delete[] _pixel_cols;
      }
   };
}

#endif

