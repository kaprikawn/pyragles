#ifndef HARDCODED_MESHES_HPP
#define HARDCODED_MESHES_HPP

#include <cstring>
#include <cstdlib>
#include "types.hpp"

real32* get_target_vertices( uint32* count ) {
  const uint32 value_count = 36;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {
      -0.9f,  0.9f, 0.0f
    , -0.9f,  0.8f, 0.0f
    ,  0.9f,  0.9f, 0.0f
    ,  0.9f,  0.8f, 0.0f
    , -0.8f,  0.8f, 0.0f
    , -0.9f, -0.8f, 0.0f
    , -0.8f, -0.8f, 0.0f
    , -0.9f, -0.9f, 0.0f
    ,  0.9f, -0.8f, 0.0f
    ,  0.9f, -0.9f, 0.0f
    ,  0.8f,  0.8f, 0.0f
    ,  0.8f, -0.8f, 0.0f
  };
  memcpy( result, &vertices[ 0 ], ( value_count * sizeof( vertices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

uint16* get_target_indices( uint32* count ) {
  const uint32 value_count = 24;
  uint16* result = ( uint16* )malloc( sizeof( uint16 ) * value_count );
  uint16 indices[ value_count ] = { 
      0,  1,  2
    , 2,  1,  3
    , 1,  4,  5
    , 4,  5,  6
    , 5,  8,  7
    , 7,  8,  9
    , 10, 11, 8
    , 10, 8,  3
  };
  memcpy( result, &indices[ 0 ], ( value_count * sizeof( indices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

real32* get_target_colours( uint32* count ) {
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


const uint32 tile_count_x  = 140;
const uint32 tile_count_z  = 140;

real32* get_underside_floor_vertices( uint32* count ) {
  const uint32 value_count = 12;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {
        0.0f, 0.0f, 0.0f
    , 200.0f, 0.0f, 0.0f
    , 200.0f, 0.0f, 200.0f
    ,   0.0f, 0.0f, 200.0f
  };
  memcpy( result, &vertices[ 0 ], ( value_count * sizeof( vertices[ 0 ] ) ) );
  *count = value_count;
  return result;
}


real32* get_overside_floor_vertices( uint32* count ) {
  
  const uint32 value_count = ( ( ( tile_count_x * tile_count_z ) / 2 ) * 12 );
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  real32 vertices[ value_count ] = {};
  
  uint32 i = 0;
  for( int32 x = 0; x < tile_count_x; x++ ) {
    for( int32 z = 0; z < tile_count_z; z++ ) {
      
      if( ( x % 2 == 0 && z % 2 == 0 ) || ( x % 2 != 0 && z % 2 != 0 ) ) {
        
        int32 start_x = ( x * 2 );
        int32 start_z = ( z * 2 );
        
        real32 this_x = ( real32 )start_x;
        real32 this_z = ( real32 )start_z;
        
        vertices[ i++ ] = this_x;
        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = this_z;
        
        vertices[ i++ ] = this_x;
        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = ( this_z + 2.0f );
        
        vertices[ i++ ] = ( this_x + 2.0f );
        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = ( this_z + 2.0f );
        
        vertices[ i++ ] = ( this_x + 2.0f );
        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = this_z;
      }
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

real32* get_overside_floor_colours( const uint32 count ) {
  
  const uint32 value_count = count;
  real32* result = ( real32* )malloc( sizeof( real32 ) * value_count );
  
  uint32 j = 0;
  for( uint32 i = 0; i < value_count; i++  ) {
    
    if( j == 0 ) {
      result[ i ] = 0.87f;
    } else if( j == 1 ) {
      result[ i ] = 0.733f;
    } else if( j == 2 ) {
      result[ i ] = 0.129f;
    }
    
    j++;
    if( j >= 3 ) { j = 0; }
  }
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
  
  const uint32 value_count = ( tile_count_x * tile_count_z * 6 ) / 2;
  const uint32 bytes = ( sizeof( uint16 ) * value_count );
  uint16* result = ( uint16* )malloc( bytes );
  uint16 indices[ value_count ] = {}; // = { 
  //   0, 1, 2, 2, 3, 0
  // };
  uint32 indices_offset = 0;
  uint32 i = 0;
  for( uint32 x = 0; x < tile_count_x; x++ ) {
    for( uint32 z = 0; z < tile_count_z; z++ ) {
      
      if( ( x % 2 == 0 && z % 2 == 0 ) || ( x % 2 != 0 && z % 2 != 0 ) ) {
        indices[ i++ ] = indices_offset;
        indices[ i++ ] = ( indices_offset + 1 );
        indices[ i++ ] = ( indices_offset + 2 );
        indices[ i++ ] = ( indices_offset + 2 );
        indices[ i++ ] = ( indices_offset + 3 );
        indices[ i++ ] = indices_offset;
        
        indices_offset += 4;
      }
    }
  }
  
  
  memcpy( result, &indices[ 0 ], bytes );
  *count = value_count;
  return result;
}



#endif //HARDCODED_MESHES_HPP
