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
      }
      ImGui::End();
   }
};
