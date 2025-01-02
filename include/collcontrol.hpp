#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "state.hpp"
#include "vertices.hpp"

#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]%s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}
#ifdef DEBUG_MODE
#define log_info(msg) {printf("[+] INFO [%s:%d]: %s\n", __func__, __LINE__, msg);}
#else
#define log(msg) {}
#endif

#define LEN(n) sizeof(n)/sizeof(n[0])


namespace color {
   const GLfloat blue[]   = {0.0f, 0.0f, 1.0f, 1.0f};
   const GLfloat red[]    = {1.0f, 0.0f, 0.0f, 1.0f};
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
      bool with_EBO = false;
   public:
      int create_VAO();
      int create_EBO(const void* data, size_t);
      int create_VBO(const void* data, size_t);

      void bind()   { glBindVertexArray(VAO);}
      void unbind() { glBindVertexArray(0);  }
      
      void add_atrib(uint id, GLint size, GLenum type);
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      int cleanup();
};

class Shader {
   private:
      uint id;
      std::unordered_map<std::string, uint> cached_locations;

   public:
      Shader() {};
      Shader(std::string vert_src, std::string frag_src);
      ~Shader();

   public:
      void compile(uint *shader, std::string src, GLenum type);
      void link(uint vrt, uint frag);
      void init_shader(std::string vert_src, std::string frag_src);

      void cleanup();
      std::string load_src(std::string&);

      const uint get_id() { return id; }
      const int get_location(std::string name);
      
      void use()    { glUseProgram(id); }
      void unuse()  { glUseProgram(0);  }

      void set_mat4fv(std::string location, glm::mat4x4 mat) { 
         glUniformMatrix4fv(get_location(location), 1, GL_FALSE, glm::value_ptr(mat));
      }
      void set_vec3(std::string location, glm::vec3 vec){
         glUniform3f(get_location(location), vec.x, vec.y, vec.z);
      }
      void set_vec2(std::string location, glm::vec2 vec){
         glUniform2f(get_location(location), vec.x, vec.y);
      }
      void set_float(std::string location, float x){
         glUniform1f(get_location(location), x);
      }
};

typedef enum {
   square,
   triangle,
   circle

} shape_type;

class Shape {
   public:
      uint size;
      Vertex vertex;
      shape_type shape; 
      GLenum mode = GL_TRIANGLES;
      Shader shader;

   public:
      Shape(shape_type type): shape(type)
      {
         set_shape(type);
         switch(type){
            case circle:
               shader.init_shader("shaders/circle.vert", "shaders/circle.frag");
               vertex.create_VBO(vertices::square, sizeof(vertices::square));
               vertex.create_EBO(indices::square, sizeof(indices::square));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
            case square:
               shader.init_shader("shaders/default.vert", "shaders/default.frag");
               vertex.create_VBO(vertices::square, sizeof(vertices::square));
               vertex.create_EBO(indices::square, sizeof(indices::square));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
            case triangle:
               shader.init_shader("shaders/default.vert", "shaders/default.frag");
               vertex.create_VBO(vertices::triangle, sizeof(vertices::triangle));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
         }

      };
      ~Shape(){};

   public:
      void set_size(uint s)        { size = s; }
      void set_mode(GLenum m)      { mode = m; }
      void set_shape(shape_type t) { 
         shape = t;
         switch(shape){
            case shape_type::triangle:
               size = LEN(vertices::triangle);
               break;
            case shape_type::square:
            case shape_type::circle:
               size = LEN(vertices::square);
              break;
         }
      }

      void draw(){
         if (vertex.with_EBO){
            vertex.bind();
            vertex.draw_EBO(mode, size);
            vertex.unbind();
         } else {
            vertex.bind();
            vertex.draw_VBO(mode, size);
            vertex.unbind();
         }
   }
};

class Camera {
   public:
      GLFWwindow* window;
      bool is_3D;
      glm::mat4 view;
      glm::vec3 pos; 
      float speed, rotation_speed, rotation, zoom;
      double window_width, window_height;
      
   public:
      Camera(GLFWwindow* window, bool is_3D): window(window),
         speed(1.2f), rotation_speed(1.2f), rotation(0.0f), 
         is_3D(is_3D), view(glm::mat4(1.0f)), pos(glm::vec3(0.0f)), 
         zoom(-3.0f){}
   public:
      glm::vec2 get_window_size(){ return glm::vec2(window_width, window_height); }
      glm::vec2 project(double x, double y) {
         double normx, normy;
         normx = (x / window_width ) *  2.0f - 1.0f; 
         normy = (y / window_height) * -2.0f + 1.0f; 
         glm::mat4 proj = glm::inverse(get_projection());
         glm::vec4 world = glm::vec4(normx, normy, 1.0f, 1.0f) * proj;
         return world;
      }

      glm::vec2 get_mouse_pos() {
         double x, y;
         glfwGetCursorPos(window, &x, &y);
         return project(x, y);
      }

      void update(){ 
         view = glm::mat4(1.0f);
         view = glm::translate(view, pos);
      }
      void set_pos(glm::vec3 pos) { this->pos = pos; }
      glm::mat4 get_view() { return view;}
      void set_zoom(float zoom) { this->zoom = zoom; }
      glm::mat4 get_projection();
};


class Object {
   public:
      Shader *shader;
      Shape  *shape;
      
      float angle = 0.0f;
      glm::mat4 model, view, proj;
      glm::vec3 pos, size, color, rotation_pos;
      glm::vec2 center = {0.0f, 0.0};
      float radius = 0.5f;


   public:
      Object(Shape *shape): shader(&shape->shader), shape(shape) {};
      ~Object() {};

   public:
     
      void set_pos(glm::vec3 pos) { this->pos = pos; }
      void set_size(glm::vec3 size) { this->size = size; }
      void set_rotation(float angle, glm::vec3 pos) { 
         this->angle = angle;
         this->rotation_pos = pos;
      }
      void set_color(const GLfloat *color) { 
         this->color = glm::vec3(color[0], color[1], color[2]); 
      }
      
      void update() { model = glm::mat4(1.0f); }
      void draw(glm::mat4 proj, glm::mat4 view);


   private: 
      void scale(glm::vec3 scaler);
      void translate(glm::vec3 pos);
      void rotate(float angle, glm::vec3 pos);

};

namespace imgui {
   void init(GLFWwindow* window);
   void frame();
   void render();
   void draw_main();
   void edit_circle(Object *obj);
   void edit_object(Object *obj);
   void edit_quad(Object *obj);

}

void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                            GLsizei, const GLchar*, const GLvoid*);
void key_callback(GLFWwindow*, int, int, int, int);

GLFWwindow* init_window(int width, int height);


#endif 
