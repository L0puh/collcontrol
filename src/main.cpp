#include "collcontrol.h"
#include <GLFW/glfw3.h>

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   GLFWwindow *window = init_window(500, 500);
   enable_if_debug();
   
   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glfwSetKeyCallback(window, key_callback);

      /*TODO: draw(); */

      glClearBufferfv(GL_COLOR, 0, color::blue);
      glfwSwapBuffers(window);
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
