#ifndef STATE_H
#define STATE_H


class Camera;
struct STATE {
   Camera *camera;
   float bg_color[4];
   bool mouse_clicked;
   bool mouse_clicked_changed;
   bool create_triangle;
};

extern STATE state;
#endif
