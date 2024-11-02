#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdio>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]: %s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}
#ifdef DEBUG_MODE
#define log_info(msg) {printf("[+] INFO [%s:%d]: %s\n", __func__, __LINE__, msg);}
#else
#define log(msg) {}
#endif


namespace color {
   const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
}

void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                            GLsizei, const GLchar*, const GLvoid*);
void key_callback(GLFWwindow*, int, int, int, int);

GLFWwindow* init_window(int width, int height);


#endif 
