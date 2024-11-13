#include "collcontrol.hpp"

void Object::draw() {
   
   update();
   translate(pos);
   scale(size);
   //rotate();
   
   shader->use();

   //TODO:
   shader->set_mat4fv("_projection", proj);
   shader->set_mat4fv("_view", view);
   shader->set_mat4fv("_model", model);
   shader->set_vec3("_color", color);

   shape->draw();

   shader->unuse();
}
