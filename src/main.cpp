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
  
   Camera camera(window, 0);
   Renderer renderer(&camera, window);
   
   camera.set_flag(CAMERA_FIXED); 
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

   Shape rect(shape_type::rectangle), 
         triag(shape_type::triangle), 
         circle(shape_type::circle);
   

   std::vector<Object> objects;
   state.camera = &camera;
   state.renderer = &renderer;
  
   renderer.add_shape(&rect);
   renderer.add_shape(&triag);
   renderer.add_shape(&circle);
   renderer.set_objects(&objects);
  
   state.current_collision_type |= COLLISION_FLAG_AABB;
  
   Line_t line = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
   Shape line_shape(shape_type::line, line);
   Object obj(0, &line_shape);
   obj.set_color(color::green);
   obj.set_pos({0.0, 0.0, 0.0});
   objects.push_back(obj);
  
   while (!glfwWindowShouldClose(window)){
      renderer.update();
      renderer.objects_loop(&objects);   
      check_collisions_FIXME(&objects);
      line = { -camera.get_mouse_pos(), glm::vec2(0.0f, 0.0f), };
      objects.at(0).set_line(line);
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
