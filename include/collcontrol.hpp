#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdint>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <limits>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <vector>

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
   const GLfloat bg[]     = {0.1f, 0.1f, 0.1f, 1.0f};
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
      int create_EBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);
      int create_VBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);

      void bind()   { glBindVertexArray(VAO);}
      void unbind() { glBindVertexArray(0);  }
      
      void add_atrib(uint id, GLint size, GLenum type);
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      void update_data(const void *data, size_t);
      int cleanup();
};

class Shader {
   private:
      uint id;
      std::unordered_map<std::string, uint> cached_locations;

   public:
      Shader() {};
      Shader(std::string vert_src, std::string frag_src, std::string geom_src="");
      ~Shader();

   public:
      void compile(uint *shader, std::string src, GLenum type);
      void link(uint vrt, uint frag, uint geom);
      void link(uint vrt, uint frag);
      void init_shader(std::string vert_src, 
                        std::string frag_src, 
                        std::string geom_src="");
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
   rectangle,
   triangle,
   circle,
   line

} shape_type;

struct Line_t {
   glm::vec2 start;
   glm::vec2 end;
};

class Shape {
   public:
      uint size;
      Vertex vertex;
      shape_type type; 
      GLenum mode = GL_TRIANGLES;
      Shader shader;

   public:
      Shape(shape_type type, Line_t line={}): type(type)
      {
         set_shape(type);
         switch(type){
            case shape_type::circle:
               shader.init_shader("shaders/circle.vert", "shaders/circle.frag");
               vertex.create_VBO(vertices::rectangle, sizeof(vertices::rectangle));
               vertex.create_EBO(indices::rectangle, sizeof(indices::rectangle));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
            case shape_type::rectangle:
               shader.init_shader("shaders/default.vert", "shaders/default.frag");
               vertex.create_VBO(vertices::rectangle, sizeof(vertices::rectangle));
               vertex.create_EBO(indices::rectangle, sizeof(indices::rectangle));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
            case shape_type::triangle:
               shader.init_shader("shaders/default.vert", "shaders/default.frag");
               vertex.create_VBO(vertices::triangle, sizeof(vertices::triangle));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
            case shape_type::line:
               shader.init_shader("shaders/default.vert",
                                  "shaders/default.frag");
               glm::vec2 p0 = line.start; 
               glm::vec2 p1 = line.end; 
               const float vertices[] = {
                  p0.x, p0.y, 0.0f,
                  p1.x, p1.y, 0.0f
               };
               vertex.create_VBO(vertices, sizeof(vertices));
               vertex.add_atrib(0, 3, GL_FLOAT);
               break;
         }

      };
      ~Shape(){};

   public:
      void set_size(uint s)        { size = s; }
      void set_mode(GLenum m)      { mode = m; }
      void set_shape(shape_type t) { 
         type = t;
         switch(type){
            case shape_type::triangle:
              size = LEN(vertices::triangle);
              break;
            case shape_type::rectangle:
            case shape_type::circle:
              size = LEN(vertices::rectangle); 
              break;
            default:
              mode = GL_LINES;
              size = LEN(vertices::line);
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
      float offset = 1.5f;
      uint8_t flags;
      glm::mat4 view;
      glm::vec3 pos; 
      float speed, rotation_speed, rotation, zoom;
      double window_width, window_height;
      
   public:
      Camera(GLFWwindow* window, uint8_t flags): window(window),
         speed(1.0f), rotation_speed(1.2f), rotation(0.0f), 
         flags(flags), view(glm::mat4(1.0f)), pos(glm::vec3(0.0f)), 
         zoom(-3.0f){}
   public:
      void update_movement();
      glm::vec2 unproject(glm::vec2 pos);
      glm::vec2 project(double x, double y);
      
      glm::vec2 get_mouse_pos();
      glm::mat4 get_projection();
      void update();

   public:
      void hide_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      void show_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      void set_pos(glm::vec3 pos)   { this->pos   = pos; }
      void set_speed(float speed)   { this->speed = speed; }
      void set_zoom(float zoom)     { this->zoom  = zoom; }
      glm::vec2 get_window_size()   { return glm::vec2(window_width, window_height); }
      glm::mat4 get_view()          { return view;}
      void clear_flag(uint8_t flag) { flags &= ~flag; }
      void set_flag(uint8_t flag)   { flags |= flag; }

};

class Object {
   public:
      int id;
      uint8_t flags;
      Shader *shader;
      Shape  *shape;
      bool is_static = false;
      std::vector<GLfloat const*> colors = { color::blue, color::red,
                                             color::green, color::white, 
                                             color::yellow };
        
      float angle = 0.0f;
      float mass = 10.0f;
      glm::mat4 model, view, proj;
      glm::vec3 size, color, rotation_pos;
      glm::vec2 center   = {0.0f, 0.0f};
      glm::vec2 velocity = {1.0f, 1.0f}, force = glm::vec2(0.0f), pos;
      float speed = 2.0f;
      float radius = glm::vec2(glm::normalize(size)/2.0f).y;


   public:
      Object(int id, Shape *shape): shader(&shape->shader), shape(shape), id(id) {};
      ~Object() {};

   public:
      void set_pos(glm::vec3 pos) { this->pos = pos; }
      void set_size(glm::vec3 size) { 
         this->radius = size.y * 0.15;
         this->size = size; 
      }
      void set_rotation(float angle, glm::vec3 pos) { 
         this->angle = angle;
         this->rotation_pos = pos;
      }
      void set_color(glm::vec3 color) {
         this->color = color; 
      }
      void set_color(const GLfloat *color) { 
         this->color = glm::vec3(color[0], color[1], color[2]); 
      }
      void update() { model = glm::mat4(1.0f); }
      void draw(glm::mat4 proj, glm::mat4 view);
      void set_random_color();
      void set_random_size();


   private: 
      void scale(glm::vec3 scaler);
      void translate(glm::vec3 pos);
      void translate(glm::vec2 pos);
      void rotate(float angle, glm::vec3 pos);

};

class Renderer {
   Camera *camera;
   GLFWwindow *window;
   glm::vec2 last_pos; 
   
   std::vector<Shape*> shapes;
   std::vector<Object> *objects;
  
   public:
      glm::vec2 gravity = {0.0f, 0.9f};

   public:
      Renderer(Camera *cam, GLFWwindow *win): 
               camera(cam), window(win){}
   public:

      void render(std::vector<Object> *objects);
      void update();
      void add_shape(Shape *shape){ shapes.push_back(shape); }

   public: //objects 
      
      void draw_line(glm::vec3 p0, glm::vec3 p1, const GLfloat *color, GLfloat thickness=1.0f);
      void objects_loop(std::vector<Object> *objects);
      void update_gravity(Object *obj);
      void edit_object(Object *object);
      int  delete_object(Object *object);
      
      void drag_and_drop(Object *object);
      void create_new_object(shape_type type, glm::vec2 pos);
      void set_objects(std::vector<Object> *objects) {this->objects = objects;}
      void draw_objects(std::vector<Object>*objects);
};


namespace collision {
   
   struct collider_rect { glm::vec2 ru, lu, ld, rd; };
   struct collider_triag{ glm::vec2 up, rt, lt; };
   struct collision_t { bool is_collide; glm::vec2 direction; 
                  float depth = -std::numeric_limits<float>::infinity();}; 
  
   bool point_is_inside(glm::vec2 pos, Object &obj);
   collider_triag get_collider_triag(Object &obj);
   collider_rect  get_collider_rect(Object &obj);
   
   collision_t collision_with_world(Object *obj);
   collision_t rect_rect(Object &x, Object &y);
   collision_t circle_circle(Object &x, Object &y);
   collision_t circle_rect(Object &c, Object &r);
   collision_t rect_triag(Object &r, Object &t);
   collision_t triag_circle(Object &t, Object &c);
   collision_t triag_traig(Object &t, Object &t2);
   
   collision_t detect_collision(Object *x, Object *y);
   void resolve_collision(Object *x, Object *y, collision_t coll);
   bool intersect(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
   glm::vec2 closest_point_segment(glm::vec2 &p, glm::vec2 a, glm::vec2 b);
};

namespace imgui {

   void init(GLFWwindow* window);
   void frame();
   void render();
   void draw_main();
   void edit_circle(Object *obj);
   void edit_object(Object *obj);
   void edit_quad(Object *obj);
   void create_objects_popup();
   void update_focused();

}

void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                            GLsizei, const GLchar*, const GLvoid*);
void update_deltatime();
GLFWwindow* init_window(int width, int height);
void check_collisions(std::vector<Object> *objects);

inline int get_random_number(int from, int to){
   int seed, num;
   seed = static_cast<int>(state.deltatime * 1000000); 
   num = (seed * 1103515245 + 12345) % (to-from);
   if (num < from) num = from;
   return num + from;
}

inline float get_random_number(float from, float to){
   int seed;
   float num;
   
   seed = static_cast<int>(state.deltatime * 1000000);
   num = (seed * 1103515245 + 12345) % (int)((to - from) * 1000000);
   num = from + num / 1000000.0f;
   if (num < from) num = from;
   return num;
}
#endif 
