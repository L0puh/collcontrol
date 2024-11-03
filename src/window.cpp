#include "collcontrol.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>

GLFWwindow* init_window(int width, int height){
   GLFWwindow* window;

   if (!glfwInit())
      error_and_exit("glfw init failed");

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(width, height, "window", 0, 0);
   if (window == NULL) {
     printf("[-] error[%s:%s:%d]: %s\n",
            "/home/lopuh/code/collcontrol/src/window.cpp", __func__, 16,
            "window init failed");
     exit(-1);
   };

   glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
      error_and_exit("glad load failed");

   //TODO: glfwSetFramebufferSizeCallback(window, frame_buffer_size);

   log_info("init window");
   return window;
}


void debug_message_callback(GLenum src, GLenum type, 
                      GLuint id,GLuint sev, GLsizei len, 
                      const GLchar* msg, const GLvoid* parm)
{
   if (type == GL_DEBUG_TYPE_ERROR){
      error_and_exit(msg);
   }
}

void key_callback(GLFWwindow* window, int key, int code, int action, int mods){
   switch(key){
      case GLFW_KEY_Q:
         glfwSetWindowShouldClose(window, true);
         return;
   }
}
