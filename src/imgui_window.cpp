#include "collcontrol.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <cstdio>
#include <imgui/imgui.h>
#include <string>

namespace imgui {
   void init(GLFWwindow* window){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init();
   }
   void frame(){
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
   }
   void render(){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }
   void draw_main(){
      bool is_move = state.camera->flags & CAMERA_MOVE;
      ImGui::Begin("main", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("MAIN FRAME WINDOW");
         glm::vec2 mouse_pos = state.camera->get_mouse_pos();
         ImGui::Text("MOUSE COORDS: %.6f %.6f", mouse_pos.x, mouse_pos.y);
         ImGui::ColorEdit4("BG COLOR:", state.bg_color, 0);
         ImGui::Checkbox("MOVE CAMERA:", &is_move);
      }
      ImGui::End();
      //it's a hack, FIXME later 
      if (is_move && !(state.camera->flags & CAMERA_MOVE)){
         state.camera->set_flag(CAMERA_MOVE|CAMERA_CHANGED);
      } else if (!is_move && state.camera->flags & CAMERA_MOVE) {
         state.camera->clear_flag(CAMERA_MOVE);
         state.camera->set_flag(CAMERA_CHANGED);
      }

   }

   void edit_circle(Object *obj){
      float color[3] = {obj->color.x, obj->color.y, obj->color.z};
      ImGui::Text("EDIT CIRCLE");
      ImGui::SliderFloat("RADIUS##", &obj->radius, 0, 5.0f, "%.5f", 0);
      ImGui::ColorEdit3("COLOR:##", color, 0);
      ImGui::SliderFloat("POS X:##", &obj->pos.x, 0, 100.0f, "%.5f", 0);
      ImGui::SliderFloat("POS Y:##", &obj->pos.y, 0, 100.0f, "%.5f", 0);
      ImGui::SliderFloat("CC X:##", &obj->center.x, -1.0f, 1.0f, "%.9f", 0);
      ImGui::SliderFloat("CC Y:##", &obj->center.y, -1.0f, 1.0f, "%.9f", 0);
      obj->color = {color[0], color[1], color[2]};
   }
   void edit_object(Object *obj){
      ImGui::Begin("main##edit", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::PushID(obj->id);
      if (obj->shape->type == shape_type::circle){
         edit_circle(obj);
      } else {
         edit_quad(obj);
      }
      ImGui::PopID();
      ImGui::End();

   }
   void edit_quad(Object *obj){
      float color[3] = {obj->color.x, obj->color.y, obj->color.z};
      ImGui::Text("EDIT QUAD");
      ImGui::ColorEdit3("COLOR:##", color, 0);
      ImGui::SliderFloat("POS X:##", &obj->pos.x, 0, 100.0f, "%.5f", 0);
      ImGui::SliderFloat("POS Y:##", &obj->pos.y, 0, 100.0f, "%.5f", 0);
      ImGui::SliderFloat("ANGLE:##", &obj->angle, 0, 90.0f, "%.10f", 0);
      obj->color = {color[0], color[1], color[2]};
   }
};
