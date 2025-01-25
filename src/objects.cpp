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


