#include "collcontrol.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vert_src, std::string frag_src) {
   uint vtx, frg;

   compile(&vtx, vert_src, GL_VERTEX_SHADER);
   compile(&frg, frag_src, GL_FRAGMENT_SHADER);

   id = glCreateProgram();
   link(vtx, frg);
   glDeleteShader(vtx);
   glDeleteShader(frg);

}

Shader::~Shader(){
   cleanup();
}

std::string Shader::load_src(std::string& src){
   std::string res;
   std::ifstream file;
   std::stringstream stream;
  
   try {
      file.open(src.c_str());
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      
      stream << file.rdbuf();
      file.close();
      res = stream.str();
   }
   catch(std::ifstream::failure error){
      error_and_exit(error.what());
   }
   return res;
}

void Shader::compile(uint *shader, std::string src, GLenum type){
   uint sh;
   const char* file_char;
   int res; char info[512];
   
   sh = glCreateShader(type);
   std::string file = load_src(src);
   file_char = file.c_str();
  
   glShaderSource(sh, 1, &file_char, NULL);
   glCompileShader(sh);

   glGetShaderiv(sh, GL_COMPILE_STATUS, &res);
   if (!res) {
      glGetShaderInfoLog(sh, 512, NULL, info);
      char* msg = (char*) malloc(512 * 2);
      sprintf(msg, "file: %s, error: %s\n", src.c_str(), info);
      error_and_exit(msg);
      free(msg);
   } 
   if (shader == NULL) error_and_exit("no shader provided");
   *shader = sh;
   
   log_info("load shader");
   return;
}

void Shader::link(uint vrt, uint frag){
   int res; char info[512];

   glAttachShader(id, vrt);
   glAttachShader(id, frag);
   glLinkProgram(id);
   
   glGetProgramiv(id, GL_LINK_STATUS, &res);
   if (!res) {
      glGetProgramInfoLog(id, 512, NULL, info);
      error_and_exit(info);
   } 
   log_info("link shader");
   return;

}
void Shader::cleanup() {
   glDeleteProgram(id);
}

const int Shader::get_location(char* name){
   int location;
   if (cached_locations[name]) 
      return cached_locations[name];
   location = glGetUniformLocation(id, name);
   if (location == -1) 
      error_and_exit(name);
   cached_locations[name] = location;
   return location;
}