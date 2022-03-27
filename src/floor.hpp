#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "types.hpp"
#include "object_data.hpp"

#define SCROLL_SPEED 10.0f

void scroll_floor( uint32 floor_index, real32 dt ) {
  
  uint32 i = floor_index;
  
  positions[ i ].z += ( SCROLL_SPEED * dt );
  while( positions[ i ].z > ( floor_start_z + 4.0f ) ) {
    positions[ i ].z -= 4.0f;
  }
  
}

#endif //FLOOR_HPP
