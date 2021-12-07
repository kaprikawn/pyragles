#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#include "types.hpp"

real32 gl_array_buffer_data         [ 100000 ];
uint32 gl_element_array_buffer_data [ 10000 ];
uint8  image_data                   [ 100000 ];

uint32 gl_array_buffer_data_last_position         = 0;
uint32 gl_element_array_buffer_data_last_position = 0;
uint32 image_data_last_position                   = 0;

uint32 log_array_buffer_data( real32* data, uint32 count ) {
  uint32 result = gl_array_buffer_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    real32 this_value = *data;
    gl_array_buffer_data[ gl_array_buffer_data_last_position++ ] = this_value;
    data++;
  }
  return result;
}

uint32 log_element_array_buffer_data( uint16* data, uint32 count ) {
  uint32 result = gl_element_array_buffer_data_last_position;
  for( uint32 i = 0; i < count; i++ ) {
    uint16 this_index = *data;
    uint32 index_value = ( uint32 )this_index;
    gl_element_array_buffer_data[ gl_element_array_buffer_data_last_position++ ] = index_value;
    data++;
  }
  return result;
}

uint32 log_image_data( uint8* data, uint32 byte_length ) {
  
  uint32 result = image_data_last_position;
  for( uint32 i = 0; i < byte_length; i++ ) {
    uint8 this_value = *data;
    image_data[ image_data_last_position++ ] = this_value;
    data++;
  }
  return result;
}

#endif //OBJECT_DATA_HPP