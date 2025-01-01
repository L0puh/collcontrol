#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;

out mat4 _mpv;

void main()
{
   _mpv = _projection * _view * _model;
   gl_Position = _mpv * vec4(pos.x, pos.y, pos.z, 1.0f);
}

