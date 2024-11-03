#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sys/types.h>
#include <unordered_map>

#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]: %s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}
#ifdef DEBUG_MODE
#define log_info(msg) {printf("[+] INFO [%s:%d]: %s\n", __func__, __LINE__, msg);}
#else
#define log(msg) {}
#endif

namespace color {
   const GLfloat blue[]   = {0.0f, 0.0f, 1.0f, 1.0f};
   const GLfloat red[]    = {1.0f, 0.0f, 1.0f, 1.0f};
   const GLfloat white[]  = {1.0f, 1.0f, 1.0f, 1.0f};
   const GLfloat black[]  = {0.0f, 0.0f, 0.0f, 1.0f};
   const GLfloat green[]  = {0.0f, 1.0f, 0.0f, 1.0f};
   const GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
}

class Vertex { 
   private:
      uint VAO, VBO, EBO;
   public:
      Vertex()  { VAO = create_VAO(); }
      ~Vertex() { cleanup(); }
   public:
      int create_VAO();
      int create_EBO(const void* data, size_t);
      int create_VBO(const void* data, size_t);

      void bind()   { glBindVertexArray(VAO);}
      void unbind() { glBindVertexArray(0);  }
      
      void add_atrib(uint id, GLint size, GLenum type,
                      GLboolean norm, GLsizei stride); 
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      int cleanup();
};

class Shader {
   private:
      uint id;
      std::unordered_map<std::string, uint> cached_locations;

   public:
      Shader(std::string vert_src, std::string frag_src);
      ~Shader();

   public:
      void compile(uint *shader, char* src, GLenum type);
      void link(uint vrt, uint frag);

      void cleanup();
      void load_src();

      const uint get_id() { return id; }
      const int get_location(char* name);
      
      void use()    { glUseProgram(id); }
      void unuse()  { glUseProgram(0);  }
};



void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                            GLsizei, const GLchar*, const GLvoid*);
void key_callback(GLFWwindow*, int, int, int, int);

GLFWwindow* init_window(int width, int height);


#endif 
