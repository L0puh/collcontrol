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


   bool convex(Object &x, Object &y){
      return 0;
   }

   bool resolve_boundaries(Object *obj){
      glm::vec2 screen, size, w;
      
      w = state.camera->get_window_size();
      screen = state.camera->unproject(obj->pos);
      size = obj->size;

      if (screen.x - size.x < 0 || screen.x + size.x > w.x ){ 
         obj->velocity.x *= -1;
      }
      if (screen.y - size.y < 0 || screen.y + size.y > w.y) {
         obj->velocity.y *= -1;
      }
      return (screen.x - size.x <= 0 || screen.x + size.x >= w.x ||
              screen.y - size.y <= 0 || screen.y + size.y >= w.y);
   }

   void resolve_collisions(Object *x, Object *y){
      collision_t coll;
      if (x->shape->type == circle && y->shape->type == circle){
         coll = circle_circle(*x, *y);
         if (coll.is_collide){
            x->velocity = coll.direction;
            y->velocity = -coll.direction;
         }
      }
      if (x->shape->type == rectangle && y->shape->type == circle){
         coll = circle_rect(*y, *x);
      }
      if(x->shape->type == circle && y->shape->type == rectangle){
         coll = circle_rect(*x, *y);
      }
      if(x->shape->type == rectangle && y->shape->type == rectangle){
         coll = rect_rect(*x, *y);
      }
     
      if(x->shape->type == triangle && y->shape->type == rectangle){
         coll = rect_triag(*y, *x);
         if (coll.is_collide){
            x->velocity = glm::normalize(x->velocity - coll.direction);
            y->velocity = glm::normalize(y->velocity + coll.direction);
         }
         return;
      }
      if(x->shape->type == rectangle && y->shape->type == triangle){
         coll = rect_triag(*x, *y);
         if (coll.is_collide){
            x->velocity = glm::normalize(x->velocity - coll.direction);
            y->velocity = glm::normalize(y->velocity + coll.direction);
         }
         return;
      }
      
      if (coll.is_collide){
         x->velocity = coll.direction;
         y->velocity = -coll.direction;
      }
   }
   collision_t circle_circle(Object &x, Object &y){
      bool is_collide;
      glm::vec2 direction;
      float dx, dy, dist, smr, angle;
      
      dx = x.pos.x - y.pos.x;
      dy = x.pos.y - y.pos.y;
      dist = (dx * dx) + (dy * dy);
      smr = (x.size.x + y.size.x)/2.5f;
      
      is_collide = dist <= smr * smr;
      if (is_collide) {
         angle = atan2(dy, dx);
         direction.x = cos(angle);
         direction.y = sin(angle);
       } 
      return {is_collide, direction};
   }

   collision_t circle_rect(Object &c, Object &r){
      bool is_collide;
      collider_rect a;
      float dist, dx, dy, angle;
      glm::vec2 closest = c.pos, dir;

      a = get_collider_rect(r);
      closest.x = (closest.x < a.ld.x) ? a.ld.x: closest.x;
      closest.y = (closest.y < a.ld.y) ? a.ld.y: closest.y;

      closest.x = (closest.x > a.ru.x) ? a.ru.x: closest.x;
      closest.y = (closest.y > a.ru.y) ? a.ru.y: closest.y;
      
      dist = glm::distance(glm::vec2(c.pos), closest);
      is_collide = dist <= c.size.y / 2.5f;
      if (is_collide){
         dx = c.pos.x - closest.x;
         dy = c.pos.y - closest.y;
         if (dx != 0 || dy != 0){
            angle = atan2(dy, dx);
            dir = {cos(angle), sin(angle)};
         }
      }
      return {is_collide, dir};
   }
   collision_t rect_rect(Object &x, Object &y){
      bool is_collide;
      collider_rect a, b; 
      glm::vec2 v, dir;
      
      a = get_collider_rect(x), 
      b = get_collider_rect(y);
     
      is_collide = a.ld.x <= b.ru.x &&
                 a.ru.x >= b.ld.x &&
                 a.ld.y <= b.ru.y &&
                 a.ru.y >= b.ld.y;
      if (is_collide) {
         v = x.pos - y.pos;
         if (glm::length(v) > 0) 
            dir = glm::normalize(v);
         }
      return {is_collide, dir};
   }

   bool intersect(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d){
      /* two line segments intersect */
      float det, t, u;
      
       det = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
       if (det == 0) return false; 

       t = ((c.x - a.x) * (d.y - c.y) - (c.y - a.y) * (d.x - c.x)) / det;
       u = -((a.x - c.x) * (b.y - a.y) - (a.y - c.y) * (b.x - a.x)) / det;

       return (t > 0 && t < 1 && u > 0 && u < 1);
   }

   collision_t rect_triag(Object &r, Object &t){
      /* FIXME: works, but needs debuging*/
      collider_rect rect;
      collider_triag tri; 

      glm::vec2 dir;
      
      tri = get_collider_triag(t);
      rect = get_collider_rect(r);

      glm::vec2 rect_edges[4][2] = {
         {rect.ld, rect.lu},
         {rect.lu, rect.ru},
         {rect.ru, rect.rd},
         {rect.rd, rect.ld}
      };

      glm::vec2 tri_edges[3][2] = {
         {tri.lt, tri.rt},
         {tri.rt, tri.ru},
         {tri.ru, tri.lt}
      };

    for (int i = 0; i < 4; ++i) 
        for (int j = 0; j < 3; ++j) 
            if (intersect(rect_edges[i][0], rect_edges[i][1], 
                          tri_edges[j][0], tri_edges[j][1])) 
            {
               dir.y = rect_edges[i][1].x - rect_edges[i][0].x;
               dir.x = rect_edges[i][1].y - rect_edges[i][0].y;
               return {true, glm::normalize(dir)}; 
            }

    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 4; ++j) 
            if (intersect(tri_edges[i][0], tri_edges[i][1], 
                           rect_edges[j][0], rect_edges[j][1])) 
            {
               dir.y = tri_edges[i][1].x - tri_edges[i][0].x;
               dir.x = tri_edges[i][1].y - tri_edges[i][0].y;
               return {true, glm::normalize(dir)}; 
            }
         
      return {false, dir};
      
   }
      
};
