#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

namespace input {
   inline bool is_pressed(GLFWwindow* window, int key) {
      return glfwGetKey(window, key) == GLFW_PRESS;
   }
};


#endif 
