#ifndef STATE_H
#define STATE_H


#include <cstdint>
#include <map>

class Camera;
class Renderer;

struct STATE {
   uint8_t current_collision_type;

   Camera *camera;
   Renderer *renderer;
   float bg_color[4];
   float deltatime;
   float last_frame = 0.0f; // for deltatime
   float cooldown   = 0.4f; // seconds
   float last_press = 0.0f; // seconds 
   int current_edited_id = -1; 
   int current_dragged_id = -1; 
   std::map<int, bool> keys;
   // FIXME: add bitwise for flags
   bool mouse_clicked;
   bool mouse_clicked_changed;
   bool imgui_focused;
   bool edit_flag; 
};

extern STATE state;
#endif
