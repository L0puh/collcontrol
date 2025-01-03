
#include "collcontrol.hpp"
#include "input.hpp"
#include <GLFW/glfw3.h>

glm::mat4 Camera::get_projection() {
   float aspect = (float)window_width/window_height;
   glm::mat4 proj = glm::ortho(-aspect * zoom, aspect*zoom, -aspect*zoom, aspect*zoom, -1.0f, 1.0f);
   return proj;
}


void Camera::update_movement(float deltatime){
   if (input::is_pressed(window, GLFW_KEY_W)){
      pos.y += speed * deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_S)){
      pos.y -= speed * deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_D)){
      pos.x += speed * deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_A)){
      pos.x -= speed * deltatime;
   }
}

glm::vec2 Camera::project(double x, double y) {
   double normx, normy;
   normx = (x / window_width ) *  2.0f - 1.0f; 
   normy = (y / window_height) * -2.0f + 1.0f; 
   glm::mat4 proj = glm::inverse(get_projection());
   glm::vec4 world = glm::vec4(normx, normy, 1.0f, 1.0f) * proj;
   return world;
}
glm::vec2 Camera::get_mouse_pos() {
   double x, y;
   glfwGetCursorPos(window, &x, &y);
   return project(x, y);
}
      
void Camera::update(){ 
   if (flags & CAMERA_MOVE){
      hide_cursor();
      update_movement(1.0f); //FIXME: calculate deltatime
   }   
   else if (flags & CAMERA_CHANGED){
      show_cursor();
      clear_flag(CAMERA_CHANGED);
      pos = glm::vec3(0.0f);
   }
   view = glm::translate(glm::mat4(1.0f), pos); 
}
