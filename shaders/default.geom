#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform vec2 _start;
uniform vec2 _end;


void main() {
    gl_Position = (gl_in[0].gl_Position +  vec4(_start, 0.0, 1.0f));
    EmitVertex();
    gl_Position = (gl_in[0].gl_Position +  vec4(_end, 0.0, 1.0f));
    EmitVertex();
    EndPrimitive();
}
