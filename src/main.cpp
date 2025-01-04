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
  
   Shape shape(shape_type::triangle);
   Camera camera(window, 0);
   camera.set_flag(CAMERA_FIXED); 
   state.camera = &camera;
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

      
   glm::vec2 last;

   std::vector<Object> objects;
   int current_id = -1;
   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, state.bg_color);   
      camera.update();
      imgui::frame();
      imgui::draw_main();

      for(int i = 0; i < objects.size(); i++){
         Object obj = objects.at(i);
         obj.draw(camera.get_projection(), camera.get_view());
         if (state.mouse_clicked){
            if (collision::point_is_inside(camera.get_mouse_pos(), obj) && (current_id == -1 || current_id == obj.id)){
               glm::vec2 p = camera.get_mouse_pos();
               if (state.mouse_clicked_changed){
                  last = { p.x - obj.pos.x, p.y - obj.pos.y};
                  current_id = obj.id;
                  state.mouse_clicked_changed = 0;
               }
               obj.set_pos(glm::vec3(p.x - last.x, p.y - last.y, 0.0f));
               objects.at(i) = obj;
            }
         } else current_id = -1;
         imgui::edit_object(&obj);
      }
      if (state.create_triangle){
         // FIXME: add delay between clicking (aka delta time?)
         // and change create_triangle flag to something different.
         // perhaps add bitwise flags
         state.create_triangle = 0;
         Object obj(objects.size(), &shape);
         obj.set_color(color::white);
         obj.set_size(glm::vec3(2.0f, 2.0f, 1.0f));
         obj.set_pos(glm::vec3(camera.get_mouse_pos(), 1.0f));
         obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));
         objects.push_back(obj);
      }

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
