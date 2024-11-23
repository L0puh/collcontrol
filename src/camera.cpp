
#include "collcontrol.hpp"

glm::mat4 Camera::get_projection(float zoom) {
   float aspect = (float)window_width/window_height;
   glm::mat4 proj = glm::ortho(-aspect * zoom, aspect*zoom, -1.0f*zoom, 1.0f*zoom, -1.0f, 1.0f);
   return proj;
}
