#include "collcontrol.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <imgui/imgui.h>

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
      ImGui::Begin("main", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("MAIN FRAME WINDOW");
         ImGui::ColorEdit4("BG COLOR:", state.bg_color, 0);
      }
      ImGui::End();
   }

   void edit_circle(Object *obj){
      float color[3] = {obj->color.x, obj->color.y, obj->color.z};
      ImGui::Begin("main", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("EDIT CIRCLE");
         ImGui::SliderFloat("RADIUS", &obj->radius, 0, 5.0f, "%.5f", 0);
         ImGui::ColorEdit3("COLOR:", color, 0);
         ImGui::SliderFloat("POS X:", &obj->pos.x, 0, 100.0f, "%.5f", 0);
         ImGui::SliderFloat("POS Y:", &obj->pos.y, 0, 100.0f, "%.5f", 0);
         ImGui::SliderFloat("CC X:", &obj->center.x, -1.0f, 1.0f, "%.9f", 0);
         ImGui::SliderFloat("CC Y:", &obj->center.y, -1.0f, 1.0f, "%.9f", 0);
      }
      ImGui::End();
      obj->color = {color[0], color[1], color[2]};
   }
   void edit_object(Object *obj){
      if (obj->shape->shape == shape_type::circle){
         edit_circle(obj);
      } else {
         edit_quad(obj);
      }

   }
   void edit_quad(Object *obj){
      float color[3] = {obj->color.x, obj->color.y, obj->color.z};
      ImGui::Begin("main", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("EDIT QUAD");
         ImGui::ColorEdit3("COLOR:", color, 0);
         ImGui::SliderFloat("POS X:", &obj->pos.x, 0, 100.0f, "%.5f", 0);
         ImGui::SliderFloat("POS Y:", &obj->pos.y, 0, 100.0f, "%.5f", 0);
         ImGui::SliderFloat("ANGLE:", &obj->angle, 0, 90.0f, "%.10f", 0);
      }
      ImGui::End();
      obj->color = {color[0], color[1], color[2]};
   }
};
