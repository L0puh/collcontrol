#include "collcontrol.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
   enable_if_debug();
  
   Shape shape(shape_type::triangle);
   Camera camera(window, 0);
   camera.set_flag(CAMERA_FIXED); 
   state.camera = &camera;
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

      
   glm::vec2 last;

   std::vector<Object> objects;
   int current_id = -1;
   float last_press = 0.0f;
   state.last_frame = 0.0f;
   state.cooldown = 0.5f;

   Renderer renderer(&camera, window);

   while (!glfwWindowShouldClose(window)){
      renderer.update();
      
      if (state.keys[GLFW_KEY_T] && state.mouse_clicked && glfwGetTime()-last_press>state.cooldown){
         last_press = glfwGetTime();
         Object obj(objects.size(), &shape);
         obj.set_color(color::white);
         obj.set_size(glm::vec3(2.0f, 2.0f, 1.0f));
         obj.set_pos(glm::vec3(camera.get_mouse_pos(), 1.0f));
         obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));
         objects.push_back(obj);
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
