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

   shape->draw();
   shader->unuse();
}


