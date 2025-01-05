#include "collcontrol.hpp"
#include <vector>

void Renderer::update() {
   imgui::frame();
   imgui::draw_main();

   glClearBufferfv(GL_COLOR, 0, state.bg_color);   
   update_deltatime();
   camera->update();
   imgui::create_objects_popup();
}

void Renderer::create_new_object(shape_type type, glm::vec2 pos){
   Shape *shape;
   for (int i = 0; i < shapes.size(); i++){
      if (shapes.at(i)->type == type){
         shape = shapes.at(i);
         break;
      }
   }
   if (shape == NULL) return;
   Object obj(objects->size(), shape);
   obj.set_color(color::white);
   obj.set_size(glm::vec3(2.0f, 2.0f, 1.0f));
   obj.set_pos(glm::vec3(pos, 1.0f));
   obj.set_rotation(glm::radians(180.0f),  glm::vec3(0.0f, 0.0f, 1.0f));
   objects->push_back(obj);
}

void Renderer::render(std::vector<Object> *objects) {
   //FIXME: refactor this...
   for(int i = 0; i < objects->size(); i++){
      Object obj = objects->at(i);
      obj.draw(camera->get_projection(), camera->get_view());
      if (state.mouse_clicked){
         if (collision::point_is_inside(camera->get_mouse_pos(), obj) 
            && (current_id == -1 || current_id == obj.id)){
            glm::vec2 p = camera->get_mouse_pos();
            if (state.mouse_clicked_changed){
               last_pos = { p.x - obj.pos.x, p.y - obj.pos.y};
               current_id = obj.id;
               state.mouse_clicked_changed = 0;
            }
            obj.set_pos(glm::vec3((p.x - last_pos.x),
                                  (p.y - last_pos.y), 
                                  0.0f));
         }
      } else current_id = -1;
      imgui::edit_object(&obj);
      objects->at(i) = obj;
   }
   imgui::render();
   glfwSwapBuffers(window);
   glfwPollEvents();
}

