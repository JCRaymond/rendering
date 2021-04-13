#include <movie.hpp>
#include <timer.hpp>

#include <iostream>
#include <complex>

using complex = std::complex<long double>;

int main() {
   // 720
   //const unsigned im_x = 1280;
   //const unsigned im_y = 720;

   // 1k
   const unsigned im_x = 1920;
   const unsigned im_y = 1080;

   // 2k
   //const unsigned im_x = 2560;
   //const unsigned im_y = 1440;

   // 4k
   //const unsigned im_x = 3810;
   //const unsigned im_y = 2160;

   // 8k
   //const unsigned im_x = 7620;
   //const unsigned im_y = 4320;

   // 16k
   //const unsigned im_x = 15240;
   //const unsigned im_y = 8640;

   //complex center = {0,0};
   //complex center = {-0.761575990961605,-0.084759600310245}; // Spiral!
   complex center = {-0.7746806106269039,-0.1374168856037867};
   //complex center = {-0.685, 0.3};

   long double height = 1; //0.00003;
   long double pixel_height = 2 * (height / im_y);
   complex pixel_dx = {pixel_height, 0};
   complex pixel_dy = {0,-pixel_height};
   auto frame_tl = center - (((long double)im_x) / 2) * pixel_dx - (((long double)im_y) / 2) * pixel_dy;
   auto pixel_tl = frame_tl + (0.5l*pixel_dx) + (0.5l*pixel_dy);

   const unsigned fps = 30;
   const unsigned quality = 4;
   im::movie<im_x, im_y, fps, quality> mov("zoom.mp4");

   int iterlim = 100;
   int cycle = 100;

   const long double duration = 20;
   const long double step = 1;
   const long double zoom_per_step = 0.1; // 0.00001
   const long double hmult = std::pow(zoom_per_step, 1.l/(step*fps));
   auto t = timer();
   for (int frame = 0; frame < duration*fps; frame++) {
      if (frame % (int(step*fps/3)) == 0)
         iterlim = (int)(iterlim*1.5);

      height *= hmult;
      pixel_height = 2 * (height / im_y);
      pixel_dx = {pixel_height, 0};
      pixel_dy = {0,-pixel_height};
      frame_tl = center - (((long double)im_x) / 2) * pixel_dx - (((long double)im_y) / 2) * pixel_dy;
      pixel_tl = frame_tl + (0.5l*pixel_dx) + (0.5l*pixel_dy);

      mov.write_frame([=](unsigned x, unsigned y) -> im::pixel {
         auto c = pixel_tl + ((long double)x) * pixel_dx + ((long double)y) * pixel_dy;
         
         auto z = c;
         int i = 0;
         while (i < iterlim) {
            z = z*z + c; 
            if (std::abs(z) >= 2)
               break;
            i++;
         }

         if (i == iterlim)
            return {0,0,0};

         double cperc1 = ((double)(i % cycle))/cycle;
         double cperc2 = ((double)(i)) / iterlim;
         double H = 0.5 + 0.5*cperc1;
         double S = 0.25 + 0.75*cperc1;
         double V = 0.75 + 0.25*cperc2;

         double C = V*S;
         double hm = H / 0.1666666;
         while (hm > 2)
            hm -= 2;
         hm -= 1;
         if (hm < 0)
            hm = -hm;
         double X = C*(1 - hm);
         double m = V - C;

         int c1 = (int)255*(C + m);
         int c2 = (int)(255*(X + m));
         int c3 = (int)(255*m);
         if (X < 0.166666)
            return {c1, c2, c3};
         else if (X < 0.333333) 
            return {c2, c1, c3};
         else if (X < 0.500000)
            return {c3, c1, c2};
         else if (X < 0.666666)
            return {c3, c2, c1};
         else if (X < 0.833333)
            return {c2, c3, c1};
         return {c1, c3, c2};
      });

      std::cout << "Frame:\t" << frame << " - " << height << " - " << iterlim << " - " << t.get_time() << std::endl;
   }
}
