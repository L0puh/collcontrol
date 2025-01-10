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

void check_collisions_FIXME(std::vector<Object> *objects){
   for(int i = 0; i < objects->size(); i++){
      for(int j = i+1; j < objects->size(); j++){
         if (state.current_collision_type & COLLISION_FLAG_AABB){
            if (collision::AABB(objects->at(i), objects->at(j))){
               objects->at(i).set_color(color::red);
            } else {
               objects->at(i).set_color(color::green);
            }
         }
      }
   }
}


void Renderer::draw_objects(std::vector<Object>*objects){
   for(int i = 0; i < objects->size(); ++i){
      Object obj = objects->at(i);
      obj.draw(camera->get_projection(), camera->get_view());
   }
}

int Renderer::delete_object(Object *object){
   glm::vec2 pos = camera->get_mouse_pos();
   if (collision::point_is_inside(pos, *object) && state.keys[GLFW_KEY_D]){
      objects->erase((std::vector<Object>::iterator)object);
      return 1;
   }
   return 0;
}

void Renderer::edit_object(Object *object){
   bool t;
   t = glfwGetTime() - state.last_press >= state.cooldown;
   if ((state.keys[GLFW_KEY_E] && t) || state.edit_flag){
         glm::vec2 pos = camera->get_mouse_pos();
         if (collision::point_is_inside(pos, *object)){
               if (state.keys[GLFW_KEY_E] && t){ 
                  state.last_press = glfwGetTime();
                  if (state.edit_flag && !state.imgui_focused) {
                     state.edit_flag = 0;
                     state.current_edited_id = -1;
                  }
                  else {
                     state.edit_flag = 1;
                     state.current_edited_id = object->id;
                  }
               }  
            }
         if (state.edit_flag && state.current_edited_id == object->id)
            imgui::edit_object(object);
   }
}
void Renderer::objects_loop(std::vector<Object> *objects){
      for (int i = objects->size()-1; i >= 0; i--){
         if (delete_object(&objects->at(i))) break;
         edit_object(&objects->at(i));
         drag_and_drop(&objects->at(i));
      }
}

void Renderer::drag_and_drop(Object *obj){
   bool is_inside;
   glm::vec2 campos; 
   
   campos = camera->get_mouse_pos();
   is_inside = collision::point_is_inside(camera->get_mouse_pos(), *obj);
   if (is_inside && state.mouse_clicked && state.mouse_clicked_changed && !state.imgui_focused){
      last_pos = { campos.x - obj->pos.x, campos.y - obj->pos.y};
      state.current_dragged_id = obj->id;
      state.mouse_clicked_changed = 0;
   } 
   if (state.mouse_clicked && !state.imgui_focused && state.current_dragged_id == obj->id){
      obj->set_pos(glm::vec3((campos.x - last_pos.x), (campos.y - last_pos.y), 0.0f));
   } 
   if (!state.mouse_clicked && state.current_dragged_id != -1) 
      state.current_dragged_id = -1;
}

void Renderer::render(std::vector<Object> *objects) {
   draw_objects(objects);
   imgui::render();
   glfwSwapBuffers(window);
   glfwPollEvents();
}

