#include "collcontrol.hpp"
#include "glm/geometric.hpp"

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

   bool rect_rect(Object &x, Object &y){
      collider_rect a, b; 
      a = get_collider_rect(x), 
      b = get_collider_rect(y);
      return  a.ld.x <= b.ru.x &&
              a.ru.x >= b.ld.x &&
              a.ld.y <= b.ru.y &&
              a.ru.y >= b.ld.y;
   }
   bool circle_circle(Object &x, Object &y){
      float dx, dy, dist, smr;
      
      dx = x.pos.x - y.pos.x;
      dy = x.pos.y - y.pos.y;
      dist = (dx * dx) + (dy * dy);
      smr = x.radius + y.radius;

      return dist <= smr * smr;
   }
      
};
