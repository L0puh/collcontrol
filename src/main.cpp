#include "collcontrol.hpp"

#include <GLFW/glfw3.h>
#include <cmath>

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
   enable_if_debug();
  
   Shape shape(shape_type::circle);
   Object obj(&shape);
   Camera camera(window, 0);
   
   state.camera = &camera;
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));
   
   obj.set_color(color::red);
   obj.set_size(glm::vec3(2.0f, 2.0f, 1.0f));
   obj.set_pos(glm::vec3(0.5, 0.5, 0.0f));
   obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));
  
   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, state.bg_color);   
      camera.update();

      obj.set_pos(glm::vec3(0.0f, glm::cos(glfwGetTime()), 0.0f));
      obj.draw(camera.get_projection(-3.0f), camera.get_view());

      imgui::frame();
      imgui::draw_main();

      imgui::edit_object(&obj);

      imgui::render();

      glfwSetKeyCallback(window, key_callback);
      glfwSwapBuffers(window);
      glfwPollEvents();
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
