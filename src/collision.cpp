#include "collcontrol.hpp"

namespace collision {
   bool point_is_inside(glm::vec2 p, Object &obj){
      glm::vec2 sz = obj.size, pos = obj.pos;
      if (obj.shape->type == shape_type::rectangle){
         bool t1, t2, t3, t4;
         glm::vec2 ru, lu, ld, rd;

         
         ru = {pos.x + sz.x/2, pos.y + sz.y/2};
         lu = {ru.x - sz.x, ru.y};
         ld = {lu.x, lu.y - sz.y};
         rd = {ru.x, ru.y - sz.y};

         t1 = ru.x >= p.x && ru.y >= p.y;
         t2 = lu.x <= p.x && lu.y >= p.y;
         t3 = rd.x >= p.x && rd.y <= p.y;
         t4 = ld.x <= p.x && ld.y <= p.y;

         return (t1 && t2 && t3 && t4);
      } else if (obj.shape->type == shape_type::triangle){
         float d, l, u, r;
         glm::vec2 up, rt, lt;
         
         up = {pos.x, pos.y - sz.y/2};
         lt = {up.x + sz.x/2, up.y+sz.y};
         rt = {up.x - sz.x/2, up.y+sz.y};
         
         d = (up.y - rt.y) * (lt.x - rt.x) + 
             (rt.x - up.x) * (lt.y - rt.y);
         
         l = ((up.y - rt.y) * (p.x - rt.x) + 
              (rt.x - up.x) * (p.y - rt.y)) / d;
         u = ((rt.y - lt.y) * (p.x - rt.x) + 
              (lt.x - rt.x) * (p.y - rt.y)) / d;
         r = 1 - l - u;
        
         return l >= 0 && u >= 0 && r >= 0;

      } else if (obj.shape->type == shape_type::circle){
         float r, dist;
         
         dist = (p.x - pos.x) * (p.x - pos.x) +
                (p.y - pos.y) * (p.y - pos.y);

         r = obj.radius;
         return dist <= r*r;
      }
      return 0;
   }
   collider_rect get_collider_rect(Object &obj){
      glm::vec2 ru, lu, ld, rd;
      glm::vec3 sz = obj.size, pos = obj.pos;

      ld = {pos.x - sz.x/2.0f, pos.y - sz.y/2.0f};
      ru = {pos.x + sz.x/2.0f, pos.y + sz.y/2.0f};
      lu = {ld.x, ru.y};
      rd = {ru.x, ld.y};

      return {ru, lu, ld, rd};
   }

   collider_triag get_collider_triag(Object &obj){
      float d, l, u, r;
      glm::vec2 up, rt, lt;
      glm::vec3 pos = obj.pos, sz = obj.size;

      up = {pos.x, pos.y + sz.y / 2.0f};
      lt = {pos.x - sz.x / 2.0f, pos.y - sz.y / 2.0f};
      rt = {pos.x + sz.x / 2.0f, pos.y - sz.y / 2.0f};

      return {up, rt, lt};
   }

   bool rect_rect(Object &x, Object &y){
      collider_rect a, b; 
      
      a = get_collider_rect(x), 
      b = get_collider_rect(y);
     
      return  a.ld.x <= b.ru.x &&
              a.ru.x >= b.ld.x &&
              a.ld.y <= b.ru.y &&
              a.ru.y >= b.ld.y;
   }

   bool convex(Object &x, Object &y){
      

      return 0;
   }

   bool detect_boundaries(Object &obj){
      glm::vec2 screen, size, w;
      
      w = state.camera->get_window_size();
      screen = state.camera->unproject(obj.pos);
      size = obj.size / 2.0f;
      return (screen.x - size.x <= 0 || screen.x + size.x >= w.x ||
              screen.y - size.y <= 0 || screen.y + size.y >= w.y);
   }

   bool rect_triag(Object &r, Object &t){
      collider_rect a;
      collider_triag b; 
      
      a = get_collider_rect(r), 
      b = get_collider_triag(t);
    
      // FIXME:
      return  a.ld.x <= b.ru.x &&
              a.ru.x >= b.lt.x &&
              a.ld.y <= b.ru.y &&
              a.ru.y >= b.lt.y;
   }
   bool circle_circle(Object &x, Object &y){
      float dx, dy, dist, smr;
      
      dx = x.pos.x - y.pos.x;
      dy = x.pos.y - y.pos.y;
      dist = (dx * dx) + (dy * dy);
      smr = (x.size.x + y.size.x)/2.5f;

      return dist <= smr * smr;
   }
   bool circle_rect(Object &c, Object &r){
      float dist;
      collider_rect a;
      glm::vec2 closest = c.pos;

      a = get_collider_rect(r);
      closest.x = (closest.x < a.ld.x) ? a.ld.x: closest.x;
      closest.y = (closest.y < a.ld.y) ? a.ld.y: closest.y;

      closest.x = (closest.x > a.ru.x) ? a.ru.x: closest.x;
      closest.y = (closest.y > a.ru.y) ? a.ru.y: closest.y;
      
      dist = glm::distance(glm::vec2(c.pos), closest);
      return dist <= c.size.y / 2.5f;

   }
      
};
