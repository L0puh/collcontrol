#include "collcontrol.hpp"
#include <GL/gl.h>


int Vertex::create_VAO(){
   glGenVertexArrays(1, &VAO);
   return VAO;
}

int Vertex::cleanup(){
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   return 0;
}

int Vertex::create_VBO(const void* data, size_t size){
   bind();

   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

   return VBO;
}

int Vertex::create_EBO(const void* data, size_t size){
   bind();

   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

   with_EBO = true;

   return EBO;
}


int Vertex::draw_EBO(GLenum mode, size_t size){
   bind();
   glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
   unbind();
   return 0;
}
int Vertex::draw_VBO(GLenum mode, size_t size){
   bind();
   glDrawArrays(mode, 0, size);
   unbind();
   return 0;
}

void Vertex::add_atrib(uint id, GLint size, GLenum type,
                      GLboolean norm, GLsizei stride)
{
   bind();
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glVertexAttribPointer(id, size, type, norm, stride, (void*) (size * sizeof(float)));
   glEnableVertexAttribArray(id);
   unbind();
}
