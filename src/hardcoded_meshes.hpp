#ifndef HARDCODED_MESHES_HPP
#define HARDCODED_MESHES_HPP

#include <cstring>
#include <cstdlib>
#include "types.hpp"

f32* get_target_vertices( u32* count ) {
  const u32 value_count = 36;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 vertices[ value_count ] = {
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

u16* get_target_indices( u32* count ) {
  const u32 value_count = 24;
  u16* result = ( u16* )malloc( sizeof( u16 ) * value_count );
  u16 indices[ value_count ] = { 
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

f32* get_target_colours( u32* count ) {
  const u32 value_count = 12;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 colours[ value_count ] = {
      0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
  };
  memcpy( result, &colours[ 0 ], ( value_count * sizeof( colours[ 0 ] ) ) );
  *count = value_count;
  return result;
}


const u32 tile_count_x  = 140;
const u32 tile_count_z  = 140;

f32* get_underside_floor_vertices( u32* count ) {
  const u32 value_count = 12;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 vertices[ value_count ] = {
        0.0f, 0.0f, 0.0f
    , 200.0f, 0.0f, 0.0f
    , 200.0f, 0.0f, 200.0f
    ,   0.0f, 0.0f, 200.0f
  };
  memcpy( result, &vertices[ 0 ], ( value_count * sizeof( vertices[ 0 ] ) ) );
  *count = value_count;
  return result;
}


f32* get_overside_floor_vertices( u32* count ) {
  
  const u32 value_count = ( ( ( tile_count_x * tile_count_z ) / 2 ) * 12 );
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 vertices[ value_count ] = {};
  
  u32 i = 0;
  for( s32 x = 0; x < tile_count_x; x++ ) {
    for( s32 z = 0; z < tile_count_z; z++ ) {
      
      if( ( x % 2 == 0 && z % 2 == 0 ) || ( x % 2 != 0 && z % 2 != 0 ) ) {
        
        s32 start_x = ( x * 2 );
        s32 start_z = ( z * 2 );
        
        f32 this_x = ( f32 )start_x;
        f32 this_z = ( f32 )start_z;
        
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

f32* get_underside_floor_normals( u32* count ) {
  const u32 value_count = 12;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 normals[ value_count ] = {
      -1.0f, 1.0f, -1.0f
    , -1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f,  1.0f
    ,  1.0f, 1.0f, -1.0f
  };
  memcpy( result, &normals[ 0 ], ( value_count * sizeof( normals[ 0 ] ) ) );
  *count = value_count;
  return result;
}

f32* get_overside_floor_normals( const u32 count, f32* vertices ) {
  
  f32* result = ( f32* )malloc( sizeof( f32 ) * count );
  
  memcpy( result, vertices, ( sizeof( f32 ) * count ) );
  
  // update y coords
  f32* update_y = result;
  update_y++; // get first y
  for( u32 i = 0; i < ( count / 3 ); i++ ) {
    *update_y += 1.0f;
    update_y += 3;
  }
  
  return result;
}

f32* get_underside_floor_colours( u32* count ) {
  const u32 value_count = 12;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  f32 colours[ value_count ] = {
      0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
    , 0.87f, 0.87f, 0.623f
  };
  memcpy( result, &colours[ 0 ], ( value_count * sizeof( colours[ 0 ] ) ) );
  *count = value_count;
  return result;
}

f32* get_overside_floor_colours( const u32 count ) {
  
  const u32 value_count = count;
  f32* result = ( f32* )malloc( sizeof( f32 ) * value_count );
  
  u32 j = 0;
  for( u32 i = 0; i < value_count; i++  ) {
    
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

u16* get_underside_floor_indices( u32* count ) {
  const u32 value_count = 6;
  u16* result = ( u16* )malloc( sizeof( u16 ) * value_count );
  u16 indices[ value_count ] = { 
    0, 1, 2, 2, 3, 0
  };
  memcpy( result, &indices[ 0 ], ( value_count * sizeof( indices[ 0 ] ) ) );
  *count = value_count;
  return result;
}

u16* get_overside_floor_indices( u32* count ) {
  
  const u32 value_count = ( tile_count_x * tile_count_z * 6 ) / 2;
  const u32 bytes = ( sizeof( u16 ) * value_count );
  u16* result = ( u16* )malloc( bytes );
  u16 indices[ value_count ] = {}; // = { 
  //   0, 1, 2, 2, 3, 0
  // };
  u32 indices_offset = 0;
  u32 i = 0;
  for( u32 x = 0; x < tile_count_x; x++ ) {
    for( u32 z = 0; z < tile_count_z; z++ ) {
      
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
