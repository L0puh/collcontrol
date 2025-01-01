#include "collcontrol.hpp"

void Object::draw(glm::mat4 proj, glm::mat4 view) {
   
   update();
   translate(pos);
   scale(size);
   rotate(angle, rotation_pos);
   
   shader->use();

   shader->set_mat4fv("_projection", proj);
   shader->set_mat4fv("_view", view);
   shader->set_mat4fv("_model", model);
   shader->set_vec3("_color", color);

   if (shape->shape == circle){
      shader->set_vec2("_center", shape->center);
      shader->set_float("_radius", size.x/50.0f);
      shader->set_vec2("_resolution", state.camera->get_window_size());
   }

   shape->draw();
   shader->unuse();
}


