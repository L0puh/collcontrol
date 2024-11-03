#include "collcontrol.hpp"
#include "vertices.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   GLFWwindow *window = init_window(500, 500);
   enable_if_debug();
  
   Vertex vertx;

   Shader shd("shaders/default.vert", "shader/default.frag");
   vertx.create_VBO(vertices::triangle, sizeof(vertices::triangle));
   vertx.add_atrib(0, 3, GL_FLOAT, GL_FALSE, 0);

   while (!glfwWindowShouldClose(window)){
      glfwSetKeyCallback(window, key_callback);
      glfwPollEvents();

      shd.use();
      vertx.draw_VBO(GL_TRIANGLES, 3);
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
