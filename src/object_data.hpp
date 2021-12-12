#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#include "vector_maths.hpp"
#include "types.hpp"

const uint32 object_count = 2;

struct GameObject {
  uint32    id;
  Position  position;
  Position  rotation;
};

GameObject game_objects[ object_count ];

// gl data
real32 gl_array_buffer_data         [ 100000 ];
uint16 gl_element_array_buffer_data [ 10000 ];

// object data
Position  positions             [ object_count ];
uint32    shader_program_ids    [ object_count ];
int32     gl_id_positions       [ object_count ];
int32     gl_id_mvp_mats        [ object_count ];
uint32    offsets_vertex_data   [ object_count ];
uint32    offsets_index_data    [ object_count ];
uint32    gl_offsets_vertex_data[ object_count ];
uint32    gl_offsets_index_data [ object_count ];
uint32    counts_vertex_data    [ object_count ];
uint32    counts_index_data     [ object_count ];

uint32    target_gl_offsets_vertex_data = 0;
uint32    target_gl_offsets_index_data  = 0;


#endif //OBJECT_DATA_HPP
