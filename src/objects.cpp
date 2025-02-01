#include "collcontrol.hpp"

void Object::draw(glm::mat4 proj, glm::mat4 view) {
   
   update();

   if (shape->type != line){
      translate(pos);
      scale(size);
      rotate(angle, rotation_pos);
   }
   
   shader->use();
   shader->set_vec3("_color", color);
   shader->set_mat4fv("_projection", proj);
   shader->set_mat4fv("_view", view);
   shader->set_mat4fv("_model", model);

   if (shape->type == circle){
      shader->set_vec2("_center", center);
      shader->set_float("_radius", radius);
      shader->set_vec2("_resolution", state.camera->get_window_size());
   }

   shape->draw();
   shader->unuse();
}

void Object::set_random_color(){
   int num = get_random_number(0, colors.size()); 
   set_color(colors.at(num));
}

void Object::set_random_size(){
   float sz = get_random_number(1.0f, 2.0f);
   set_size({sz, sz, 0.0f});
}
void Object::scale(glm::vec3 scaler){
   model = glm::scale(model, scaler);
}
void Object::translate(glm::vec3 pos){
   model = glm::translate(model, pos);
}
void Object::rotate(float angle, glm::vec3 pos){
   model = glm::rotate(model, angle, pos);
}
