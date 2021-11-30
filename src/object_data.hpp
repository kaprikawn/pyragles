#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#include "types.hpp"
/*
real32  vertex_data     [ 10000 ];
uint32  index_data      [ 10000 ];
real32  normal_data     [ 10000 ];
real32  colour_data     [ 10000 ];
real32  tex_coord0_data [ 10000 ];

int32 vertex_data_last_position     = 0;
int32 index_data_last_position      = 0;
int32 normal_data_last_position     = 0;
int32 colour_data_last_position     = 0;
int32 tex_coord0_data_last_position = 0;

int32 log_vertex_data( real32* data, int32 count ) {
  int32 result = vertex_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    real32 this_vertex = *data;
    vertex_data[ vertex_data_last_position++ ] = this_vertex;
    data++;
  }
  return result;
}

int32 log_index_data( uint16* data, int32 count ) {
  int32 result = index_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    uint16 this_index = *data;
    uint32 index_value = ( uint32 )this_index;
    index_data[ index_data_last_position++ ] = index_value;
    data++;
  }
  return result;
}

int32 log_normal_data( real32* data, int32 count ) {
  int32 result = normal_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    real32 this_normal = *data;
    normal_data[ normal_data_last_position++ ] = this_normal;
    data++;
  }
  return result;
}

int32 log_colour_data( real32* data, int32 count ) {
  int32 result = colour_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    real32 this_colour = *data;
    colour_data[ colour_data_last_position++ ] = this_colour;
    data++;
  }
  return result;
}

int32 log_tex_coord0_data( real32* data, int32 count ) {
  int32 result = tex_coord0_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    real32 this_tex_coord0 = *data;
    tex_coord0_data[ tex_coord0_data_last_position++ ] = this_tex_coord0;
    data++;
  }
  return result;
}

*/

#endif //OBJECT_DATA_HPP
