#ifndef STATE_H
#define STATE_H


#include <map>
class Camera;
class Renderer;
struct STATE {
   Camera *camera;
   Renderer *renderer;
   float bg_color[4];
   float deltatime;
   float last_frame;
   float cooldown; // seconds
   bool mouse_clicked;
   bool mouse_clicked_changed;
   bool imgui_focused;
   std::map<int, bool> keys;

};

extern STATE state;
#endif
