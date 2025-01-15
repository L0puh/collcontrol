#version 330 core

out vec4 color;
in mat4 _mpv;
uniform vec3 _color;
uniform vec2 _center;
uniform float _radius;
uniform vec2 _resolution;

void main() {

   float alpha = 0.0;
   vec4 centerNDC = _mpv * vec4(_center, 1.0f, 1.0f);

   vec2 uv = (gl_FragCoord.xy/_resolution) * 2.0f - 1.0f;
   
   if (distance(uv.xy, centerNDC.xy) <= _radius){
      color = vec4(_color, 1.0f);
   } else
      color = vec4(0.0, 0.0, 0.0, 0.0f);

}
