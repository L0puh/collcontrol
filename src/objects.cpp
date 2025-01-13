#include "collcontrol.hpp"
#include "glm/ext/matrix_transform.hpp"

void Object::draw(glm::mat4 proj, glm::mat4 view) {
   
   update();
   
   shader->use();
   shader->set_vec3("_color", color);
   shader->set_mat4fv("_projection", proj);
   shader->set_mat4fv("_view", view);

   if (shape->type == circle){
      shader->set_vec2("_center", center);
      shader->set_float("_radius", radius/size.x);
      shader->set_vec2("_resolution", state.camera->get_window_size());
      translate(pos);
      scale(size);
      rotate(angle, rotation_pos);
   }
   if (shape->type == shape_type::line){
      // add translation for every point, i.e for both start and end?? 
      shader->set_vec2("_start", shape->line.start);
      shader->set_vec2("_end", shape->line.end);
      model = glm::translate(model, glm::vec3(shape->line.start, 0.0f));
   } 

   shader->set_mat4fv("_model", model);
   shape->draw();
   shader->unuse();
}


