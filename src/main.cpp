#include "collcontrol.hpp"
#include "vertices.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   GLFWwindow *window = init_window(500, 500);
   enable_if_debug();
  
   Shader shd("shaders/default.vert", "shaders/default.frag");
   
   Vertex vertx;
   Camera camera(window, 0);
   vertx.create_VBO(vertices::triangle, sizeof(vertices::triangle));
   vertx.add_atrib(0, 3, GL_FLOAT, GL_FALSE, 0);
   
   Shape shape(&vertx);
   shape.set_mode(GL_TRIANGLES);
   shape.set_size(LEN(vertices::triangle));
   shape.set_shape(shape_type::triangle);
   
   Object obj(&shd, &shape);
   obj.set_color(color::red);
   obj.set_size(glm::vec3(1.0f, 1.0f, 0.0f));
   obj.set_pos(glm::vec3(0.5, 0.5, 0.0f));
   obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));

   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, color::blue);   
      camera.update();

      obj.set_pos(glm::vec3(0.0f, glm::cos(glfwGetTime()), 0.0f));
      obj.draw(camera.get_projection(-3.0f), camera.get_view());

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
