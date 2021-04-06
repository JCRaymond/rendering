
#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <frame.hpp>
#include <pixel.hpp>

#include <functional>
#include <png.h>
#include <stdlib.h>
#include <string>

namespace im {

   template<unsigned width, unsigned height, unsigned fps = 60, unsigned quality = 5>
   struct movie {

      frame<width, height> _frame;
      FILE *_movie;

      movie(std::string filename) {
         _movie = popen(std::format("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb{} -s {}x{} -r {} -i - -f mp4 -q:v {} -an -vcodec mpeg4 {}", sizeof(png_byte)*24, width, height, fps, quality, filename), "w");
      }

      inline void write_frame() {
         fwrite(_frame.pixels, sizeof(png_byte), height*width*3, _movie);
      }

      inline void write_frame(std::function<pixel(unsigned, unsigned)> painter) {
         _frame.paint(painter);
         write_frame()
      }

      ~movie() {
         fflush(_movie);
         pclose(_movie);
      }
   };
}

#endif

