#ifndef FLOOR_VERTEX_DATA_HPP
#define FLOOR_VERTEX_DATA_HPP

#include <cstring>
#include <cstdlib>
#include "types.hpp"

real32* get_underside_floor_vertices( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {
      -100.0f, -0.1f, -100.0f
    , -100.0f, -0.1f,   10.0f
    ,  100.0f, -0.1f,   10.0f
    ,  100.0f, -0.1f, -100.0f
  };
  memcpy( result, &vertices[ 0 ], ( value_count * sizeof( vertices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_overside_floor_vertices( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {
      -1.0f, 0.0f, -1.0f
    , -1.0f, 0.0f,  1.0f
    ,  1.0f, 0.0f,  1.0f
    ,  1.0f, 0.0f, -1.0f
  };
  memcpy( result, &vertices[ 0 ], ( value_count * sizeof( vertices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_underside_floor_normals( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 normals[ value_count ] = {
      -1.0f, 1.0f, -1.0f
    , -1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f, -1.0f
  };
  memcpy( result, &normals[ 0 ], ( value_count * sizeof( normals[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_overside_floor_normals( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 normals[ value_count ] = {
      -1.0f, 1.0f, -1.0f
    , -1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f, -1.0f
  };
  memcpy( result, &normals[ 0 ], ( value_count * sizeof( normals[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_underside_floor_colours( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 colours[ value_count ] = {
      0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
  };
  memcpy( result, &colours[ 0 ], ( value_count * sizeof( colours[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_overside_floor_colours( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 colours[ value_count ] = {
      0.87f, 0.733f, 0.129f
    , 0.87f, 0.733f, 0.129f
    , 0.87f, 0.733f, 0.129f
    , 0.87f, 0.733f, 0.129f
  };
  memcpy( result, &colours[ 0 ], ( value_count * sizeof( colours[ 0 ] ) ) );
  *count = value_count;
  return result;
}

uint16* get_underside_floor_indices( uint32* count ) {
  const uint32 value_count = 6;
  uint16* result = ( uint16* )malloc( sizeof( uint16 ) * value_count );
  uint16 indices[ value_count ] = { 
    0, 1, 2, 2, 3, 0
  };
  memcpy( result, &indices[ 0 ], ( value_count * sizeof( indices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

uint16* get_overside_floor_indices( uint32* count ) {
  const uint32 value_count = 6;
  uint16* result = ( uint16* )malloc( sizeof( uint16 ) * value_count );
  uint16 indices[ value_count ] = { 
    0, 1, 2, 2, 3, 0
  };
  memcpy( result, &indices[ 0 ], ( value_count * sizeof( indices[ 0 ] ) ) );
  *count = value_count;
  return result;
}



#endif //FLOOR_VERTEX_DATA_HPP
