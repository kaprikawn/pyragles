#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "types.hpp"
#include "object_data.hpp"
#include "globals.hpp"


void scroll_floor( u32 floor_index, f32 dt ) {
  
  u32 i = floor_index;
  
  positions[ i ].z += ( SCROLL_SPEED * dt );
  while( positions[ i ].z > ( floor_start_z + 4.0f ) ) {
    positions[ i ].z -= 4.0f;
  }
  
}

#endif //FLOOR_HPP
