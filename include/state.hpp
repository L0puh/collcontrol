#ifndef STATE_H
#define STATE_H

class Camera;
struct STATE {
   Camera *camera;
   float bg_color[4];
};

extern STATE state;
#endif
