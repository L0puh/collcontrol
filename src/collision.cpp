#include "collcontrol.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include <limits>

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
      }
      if (x->shape->type == rectangle && y->shape->type == circle){
         coll = circle_rect(*y, *x);
         if (coll.is_collide){
            x->velocity = -coll.direction;
            y->velocity = coll.direction;
            return;
         }
      }
      if(x->shape->type == circle && y->shape->type == rectangle){
         coll = circle_rect(*x, *y);
      }
      if(x->shape->type == circle && y->shape->type == triangle){
         coll = triag_circle(*y, *x);
         if (coll.is_collide){
            x->velocity = -coll.direction;
            y->velocity = coll.direction;
            return;
         }
      }
      if(x->shape->type == triangle && y->shape->type == circle){
         coll = triag_circle(*x, *y);
      }
      if(x->shape->type == rectangle && y->shape->type == rectangle){
         coll = rect_rect(*x, *y);
      }
      if(x->shape->type == triangle && y->shape->type == rectangle){
         coll = rect_triag(*y, *x);
      }
      if(x->shape->type == rectangle && y->shape->type == triangle){
         coll = rect_triag(*x, *y);
      }
      if(x->shape->type == triangle && y->shape->type == triangle){
         coll = triag_traig(*x, *y);
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
      smr = (x.size.x + y.size.x)/2.f;
      
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
      is_collide = dist <= c.size.y / 2.f;
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
      collider_rect rc; collider_triag tc;
     
      rc = get_collider_rect(r); 
      tc = get_collider_triag(t); 

      glm::vec2 pr[4][2] = {
         {rc.ld, rc.lu},
         {rc.lu, rc.ru},
         {rc.ru, rc.rd},
         {rc.rd, rc.ld}
      };
      glm::vec2 tp[4][2] = {
         {tc.up, tc.lt},
         {tc.lt, tc.rt},
         {tc.rt, tc.up},
      };
      
      for (int i = 0; i < 4; i++){
         for (int j = 0; j < 3; j++){
            if (intersect(pr[i][0], pr[i][1], tp[j][0], tp[j][1])){
               glm::vec2 dir = glm::vec2(pr[i] - tp[j]);
               return {true, {glm::normalize(dir)}};
            }
         }
      }
      return {false, {}};
   }

   glm::vec2 closest_point_segment(glm::vec2 &p, glm::vec2 a, glm::vec2 b)
   {
      float proj;
      glm::vec2 ap, ab;
      ap = p-a, ab = b-a;

      proj = glm::dot(ap, ab) / glm::dot(ab, ab);
      if (proj < 0.0f) return a;
      if (proj > 1.0f) return b;
      return a + proj * ab;

   }
   
   collision_t triag_traig(Object &t, Object &t2){
      collider_triag a,b; 
     
      a = get_collider_triag(t2); 
      b = get_collider_triag(t); 

      glm::vec2 bp[4][2] = {
         {a.up, a.lt},
         {a.lt, a.rt},
         {a.rt, a.up},
      };
      glm::vec2 ap[4][2] = {
         {b.up, b.lt},
         {b.lt, b.rt},
         {b.rt, b.up},
      };
      
      for (int i = 0; i < 3; i++){
         for (int j = 0; j < 3; j++){
            if (intersect(bp[i][0], bp[i][1], ap[j][0], ap[j][1])){
               glm::vec2 dir = glm::vec2(bp[i] - ap[j]);
               return {true, {glm::normalize(dir)}};
            }
         }
      }
      return {false, {}};

   }

   collision_t triag_circle(Object &t, Object &c){
      float mdist;
      collider_triag tc;
      tc = get_collider_triag(t);
      
      glm::vec2 closest_point, closest_seg, p, dir;
      glm::vec2 vertc[] = {tc.up, tc.lt, tc.rt};
      glm::vec2 edges[][2] = {{tc.up, tc.lt}, {tc.lt, tc.rt}, {tc.rt, tc.up}};
      
      p = c.pos;

      mdist = std::numeric_limits<float>::infinity();
      for (const auto& v: vertc){
         float d = glm::distance(p, v);
         if (d < mdist) {
            mdist = d;
            closest_point = v;
         }
      }
      for (const auto& edge : edges) {
          closest_seg = closest_point_segment(p, edge[0], edge[1]);
          float d = glm::distance(p, closest_seg);
          if (d < mdist) {
              mdist = d;
              closest_point = closest_seg;
          }
       }
      bool is_collide = mdist <= c.size.x/2.f;
      dir = glm::normalize(closest_point - p);
      return {is_collide, dir};
   }
};
