#version 330 core
layout (location = 0) in vec3 pos;

out vec3 out_color;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;
uniform vec3 _color;

void main()
{
   gl_Position = _projection * _view * _model * vec4(pos.x, pos.y, pos.z, 1.0f);
   out_color = _color;
}

