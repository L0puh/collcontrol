#ifndef STATE_H
#define STATE_H


#include <map>
class Camera;
struct STATE {
   Camera *camera;
   float bg_color[4];
   float deltatime;
   float last_frame;
   float cooldown; // seconds
   bool mouse_clicked;
   bool mouse_clicked_changed;
   std::map<int, bool> keys;

};

extern STATE state;
#endif
