#ifndef VERTICES_H
#define VERTICES_H

#include <sys/types.h>

namespace vertices {
   const float triangle[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f,  -0.5f, 0.0f,
      0.0f,   0.5f, 0.0f
   };
   const float line[] = {
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
   };
   const float rectangle[] = {
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left
   };
};

namespace indices {
   const uint rectangle[] = {
      0, 1, 2,
      0, 2, 3
   };
};

#endif
