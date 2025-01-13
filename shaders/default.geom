#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;
uniform vec2 _start;
uniform vec2 _end;

void main() {
    vec4 start = vec4(_start, 0.0, 1.0);
    vec4 end = vec4(_end, 0.0, 1.0);

    gl_Position = _projection * _view * _model * start;
    EmitVertex();
    gl_Position = _projection * _view * _model * end;
    EmitVertex();
    EndPrimitive();
}
