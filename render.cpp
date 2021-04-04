#include <image.hpp>
#include <timer.hpp>
#include <vec.hpp>

#include <iostream>

int main() {
   auto t = timer();
   const unsigned im_x = 1920;
   const unsigned im_y = 1080;

   vec3 viewpoint = {10,10,10};
   vec3 pointing_at = {0,0,2};
   double sensordist = 1.1;
   double pixel_height = 0.001;

   auto facing = pointing_at - viewpoint;
   auto facing_norm = facing.normalized();

   auto im_vert = -(vec3(0,0,1) + facing_norm.z * facing_norm).normalized();
   auto im_horz = im_vert & facing_norm;

   auto pixel_dx = pixel_height * im_horz;
   auto pixel_dy = pixel_height * im_vert;

   auto sensor_mid = viewpoint + sensordist * facing_norm;
   auto sensor_tl = sensor_mid - (im_x / 2) * pixel_dx - (im_y / 2) * pixel_dy;
   auto pixel_tl = sensor_tl - viewpoint + 0.5*pixel_dx + 0.5*pixel_dy;

   im::image<im_x, im_y> im;

   double radius = 2;
   vec3 sphere_center = {0,0,radius};
   
   im.paint_pixels_parallel([&](unsigned x, unsigned y) -> im::pixel {
      auto dir = pixel_tl + x * pixel_dx + y * pixel_dy;

      auto d = dir.normalized();
      auto point = viewpoint;
      double dist = (sphere_center - point).norm() - radius;
      bool too_far = false;
      while (dist > 0.001) {
         point += dist * d;
         double newdist = (sphere_center - point).norm() - radius;
         if (newdist > dist) {
            too_far = true;
            break;
         }
         dist = newdist;
      }

      if (!too_far) {
         return {255*(point.x + radius)/(2*radius),255*(point.y + radius)/(2*radius),255*point.z/(2*radius)};
      }

      double d_mult = -viewpoint.z/dir.z;
      if (dir.z < 0 && d_mult < 40)
         return {255, 196, 32};
      return {32, 196, 255};
   });

   im.write("sphere.png"); 

   std::cout << "Time: " << t.get_time() << std::endl;
}
