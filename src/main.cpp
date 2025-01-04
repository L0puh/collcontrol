#include "collcontrol.hpp"
#include "input.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
   enable_if_debug();
  
   Shape shape(shape_type::triangle);
   Object obj(&shape);
   Camera camera(window, 0);
   camera.set_flag(CAMERA_FIXED); 
   state.camera = &camera;
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));
   
   obj.set_color(color::red);
   obj.set_size(glm::vec3(2.0f, 2.0f, 1.0f));
   obj.set_pos(glm::vec3(0.5, 0.5, 0.0f));
   obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));

      
   glm::vec2 last;
   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, state.bg_color);   
      camera.update();
      
      obj.draw(camera.get_projection(), camera.get_view());

      if (state.mouse_clicked){
         if (collision::point_is_inside(camera.get_mouse_pos(), &obj)){
            glm::vec2 p = camera.get_mouse_pos();
            if (state.mouse_clicked_changed){
               last = { p.x - obj.pos.x, p.y - obj.pos.y};
               state.mouse_clicked_changed = 0;
            }
            obj.set_pos(glm::vec3(p.x - last.x, p.y - last.y, 0.0f));
         }
      }


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
