#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform vec2 _start;
uniform vec2 _end;

int _DATA {
   mat4 mvp;
   vec3 pos;
} data_in[];

void main() {
    vec4 start = vec4(_start, 0.0, 1.0);
    vec4 end = vec4(_end, 0.0, 1.0);

    gl_Position = _start;
    EmitVertex();
    gl_Position = _end; 
    EmitVertex();
    EndPrimitive();
}
