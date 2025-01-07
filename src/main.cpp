#include "collcontrol.hpp"
#include "input.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
   enable_if_debug();
  
   Camera camera(window, 0);
   Renderer renderer(&camera, window);
   
   camera.set_flag(CAMERA_FIXED); 
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

      
   Shape rect(shape_type::rectangle), triag(shape_type::triangle), 
         circle(shape_type::circle);
   std::vector<Object> objects;
   state.last_frame = 0.0f;
   state.cooldown = 0.5f;
   state.camera = &camera;
   state.renderer = &renderer;
   renderer.add_shape(&rect);
   renderer.add_shape(&triag);
   renderer.add_shape(&circle);
   renderer.set_objects(&objects);
   
   bool edit_flag = 0;
   int edit_object = -1;
   float last_press = 0.0f;
   while (!glfwWindowShouldClose(window)){
      renderer.update();
         
      if ((state.keys[GLFW_MOUSE_BUTTON_LEFT] && glfwGetTime()-last_press >= state.cooldown)  || edit_flag){
         glm::vec2 pos = camera.get_mouse_pos();
         for (int i = objects.size()-1; i >= 0; i--){
            if (collision::point_is_inside(pos, objects.at(i))){
               if (state.keys[GLFW_MOUSE_BUTTON_LEFT]&& glfwGetTime()-last_press >= state.cooldown){ 
                  last_press = glfwGetTime();
                  if (edit_flag && !state.imgui_focused) edit_flag = 0;
                  else{
                     edit_flag = 1;
                     edit_object = objects.at(i).id;
                  }
               }  
            }
            if (edit_flag && edit_object == objects.at(i).id)
               imgui::edit_object(&objects.at(i));
         }
      }

      renderer.render(&objects);
   }
   
   shutdown(window);
   return 0;
}

void shutdown(GLFWwindow* window){
   glfwDestroyWindow(window);
   glfwTerminate();
   log_info("shutting down");
}

void enable_if_debug(){
#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(debug_message_callback, 0);
   log_info("debug mode is on");
#endif
}
