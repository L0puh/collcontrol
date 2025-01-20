#include "collcontrol.hpp"

void Renderer::update() {
   glClearBufferfv(GL_COLOR, 0, state.bg_color);   
   
   imgui::frame();
   imgui::draw_main();

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


void check_collisions(std::vector<Object> *objects){
   for(int i = 0; i < objects->size(); i++){
      for(int j = i+1; j < objects->size(); j++){
         collision::resolve_collisions(&objects->at(j), &objects->at(i));
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
   if ((state.keys[GLFW_KEY_E] && t) || state.global_state & EDITING){
         glm::vec2 pos = camera->get_mouse_pos();
         if (collision::point_is_inside(pos, *object)){
               if (state.keys[GLFW_KEY_E] && t){ 
                  state.last_press = glfwGetTime();
                  if (!(state.global_state & IMGUI_FOCUSED) && state.global_state & EDITING) {
                     state.global_state ^= EDITING;
                     state.current_edited_id = -1;
                  }
                  else {
                     state.global_state |= EDITING;
                     state.current_edited_id = object->id;
                  }
               }  
            }
         if (state.global_state & EDITING && state.current_edited_id == object->id)
            imgui::edit_object(object);
   }
}


void Renderer::objects_loop(std::vector<Object> *objects){
      for (int i = objects->size()-1; i >= 0; i--){
         if (delete_object(&objects->at(i))) break;
         edit_object(&objects->at(i));
         drag_and_drop(&objects->at(i));
         collision::resolve_boundaries(&objects->at(i));
         update_gravity(&objects->at(i));
      }
}

void Renderer::draw_line(glm::vec3 p0, glm::vec3 p1, const GLfloat *color, GLfloat thickness){
   glLineWidth(thickness); 
   Shape line_shape(shape_type::line, {p0, p1});
   Object line(0, &line_shape);
   line.set_color(color);
   line.draw(camera->get_projection(), camera->get_view());
   glLineWidth(1.0f); 
      
}
void Renderer::update_gravity(Object *obj){
   obj->velocity += gravity * state.deltatime;
   obj->pos += glm::vec3(obj->velocity * obj->speed * state.deltatime, 0.0f);
}

void Renderer::drag_and_drop(Object *obj){
   bool is_inside;
   glm::vec2 campos; 
   
   campos = camera->get_mouse_pos();
   is_inside = collision::point_is_inside(camera->get_mouse_pos(), *obj);
   if (is_inside && (state.global_state & MOUSE_CLICKED) && (state.global_state
               & MOUSE_CHANGED) && !(state.global_state & IMGUI_FOCUSED)){
      last_pos = { campos.x - obj->pos.x, campos.y - obj->pos.y};
      state.current_dragged_id = obj->id;
      state.global_state ^= MOUSE_CHANGED;
   } 
   if ((state.global_state & MOUSE_CLICKED) && !(state.global_state & IMGUI_FOCUSED) && state.current_dragged_id == obj->id){
      obj->set_pos(glm::vec3((campos.x - last_pos.x), (campos.y - last_pos.y), 0.0f));
   } 
   if (!(state.global_state & MOUSE_CLICKED) && state.current_dragged_id != -1) 
      state.current_dragged_id = -1;
}

void Renderer::render(std::vector<Object> *objects) {
   draw_objects(objects);
   imgui::render();
   glfwSwapBuffers(window);
   glfwPollEvents();
}

