#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <map>

class Camera;
class Renderer;

#define MOUSE_CLICKED 0b10000
#define MOUSE_CHANGED 0b01000
#define IMGUI_FOCUSED 0b00100
#define EDITING       0b00010
#define GRAVITY       0b00001

#define CAMERA_3D      0b1000
#define CAMERA_FIXED   0b0100
#define CAMERA_MOVE    0b0010
#define CAMERA_CHANGED 0b0001



struct STATE {
   Camera *camera;
   Renderer *renderer;
   float bg_color[4];
   float deltatime  = 0.0f;
   float last_frame = 0.0f; // for deltatime
   float cooldown   = 0.4f; // seconds
   float last_press = 0.0f; // seconds 
   uint16_t global_state = 0.0f;
   int current_edited_id = -1; 
   int current_dragged_id = -1; 
   std::map<int, bool> keys;
};

extern STATE state;
#endif
