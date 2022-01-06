#ifndef FLOOR_VERTEX_DATA_HPP
#define FLOOR_VERTEX_DATA_HPP

#include <cstring>
#include <cstdlib>
#include "types.hpp"

const uint32 tile_count_x  = 10;
const uint32 tile_count_z  = 10;

const int32 initial_x_offset = -20;
const int32 initial_z_offset = -20;

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
  
  const uint32 value_count = ( tile_count_x * tile_count_z * 12 * 2 );
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {};
  
  uint32 i = 0;
  for( int32 x = 0; x < ( tile_count_x * 4 ); x += 4 ) {
    for( int32 z = 0; z < ( tile_count_z * 4 ); z += 4 ) {
      
      real32 initial_x = ( real32 )x;
      real32 initial_z = ( real32 )z;
      
      initial_x += initial_x_offset;
      initial_z += initial_z_offset;
      
      vertices[ i++ ] = initial_x;
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = initial_z;
      
      vertices[ i++ ] = initial_x;
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = ( initial_z + 2.0f );
      
      vertices[ i++ ] = ( initial_x + 2.0f );
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = ( initial_z + 2.0f );
      
      vertices[ i++ ] = ( initial_x + 2.0f );
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = initial_z;
      
      initial_x += 2.0f;
      initial_z += 2.0f;
      
      vertices[ i++ ] = initial_x;
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = initial_z;
      
      vertices[ i++ ] = initial_x;
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = ( initial_z + 2.0f );
      
      vertices[ i++ ] = ( initial_x + 2.0f );
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = ( initial_z + 2.0f );
      
      vertices[ i++ ] = ( initial_x + 2.0f );
      vertices[ i++ ] = 0.0f;
      vertices[ i++ ] = initial_z;
    }
  }
  
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

real32* get_overside_floor_normals( const uint32 count, real32* vertices ) {
  
  real32* result = ( real32* )malloc( sizeof( real32 ) * count );
  
  memcpy( result, vertices, ( sizeof( real32 ) * count ) );
  
  // update y coords
  real32* update_y = result;
  update_y++; // get first y
  for( uint32 i = 0; i < ( count / 3 ); i++ ) {
    *update_y += 1.0f;
    update_y += 3;
  }
  
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
  
  const uint32 value_count = ( tile_count_x * tile_count_z * 12 * 2 );
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 colours[ value_count ] = {};
  
  uint32 i = 0;
  for( uint32 x = 0; x < ( tile_count_x * 4 ); x += 4 ) {
    for( uint32 z = 0; z < ( tile_count_z * 4 ); z += 4 ) {
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
      colours[ i++ ] = 0.87f;
      colours[ i++ ] = 0.733f;
      colours[ i++ ] = 0.129f;
      
    }
  }
  
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
  
  const uint32 value_count = ( tile_count_x * tile_count_z * 6 * 2 );
  uint16* result = ( uint16* )malloc( sizeof( uint16 ) * value_count );
  uint16 indices[ value_count ] = {}; // = { 
  //   0, 1, 2, 2, 3, 0
  // };
  uint32 indices_offset = 0;
  uint32 i = 0;
  for( uint32 x = 0; x < ( tile_count_x * 4 ); x += 4 ) {
    for( uint32 z = 0; z < ( tile_count_z * 4 ); z += 4 ) {
      
      indices[ i++ ] = indices_offset;
      indices[ i++ ] = ( indices_offset + 1 );
      indices[ i++ ] = ( indices_offset + 2 );
      indices[ i++ ] = ( indices_offset + 2 );
      indices[ i++ ] = ( indices_offset + 3 );
      indices[ i++ ] = indices_offset;
      
      indices_offset += 4;
      
      indices[ i++ ] = indices_offset;
      indices[ i++ ] = ( indices_offset + 1 );
      indices[ i++ ] = ( indices_offset + 2 );
      indices[ i++ ] = ( indices_offset + 2 );
      indices[ i++ ] = ( indices_offset + 3 );
      indices[ i++ ] = indices_offset;
      
      indices_offset += 4;
      
    }
  }
  
  
  memcpy( result, &indices[ 0 ], ( value_count * sizeof( indices[ 0 ] ) ) );
  *count = value_count;
  return result;
}



#endif //FLOOR_VERTEX_DATA_HPP
