#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#include "vector_maths.hpp"
#include "types.hpp"

const uint32 object_count = 2;

struct GameObject {
  bool32    active = false;
  uint32    index;
  Position  position;
  Position  rotation;
};

GameObject game_objects[ object_count ];

// gl data
real32 gl_array_buffer_data         [ 100000 ];
uint16 gl_element_array_buffer_data [ 10000 ];

// object data
Position  positions                 [ object_count ];
uint32    shader_program_ids        [ object_count ];
int32     gl_id_positions           [ object_count ];
int32     gl_id_normals             [ object_count ];
int32     gl_id_tex_coords0         [ object_count ];
int32     gl_id_mvp_mats            [ object_count ];
int32     gl_id_light_positions     [ object_count ];
int32     gl_id_ambient_lights      [ object_count ];
uint32    offsets_vertex_data       [ object_count ];
uint32    offsets_normal_data       [ object_count ];
uint32    offsets_tex_coord0_data   [ object_count ];
uint32    offsets_index_data        [ object_count ];
uint32    gl_offsets_vertex_data    [ object_count ];
uint32    gl_offsets_normal_data    [ object_count ];
uint32    gl_offsets_tex_coord0_data[ object_count ];
uint32    gl_offsets_index_data     [ object_count ];
uint32    counts_vertex_data        [ object_count ];
uint32    counts_normal_data        [ object_count ];
uint32    counts_tex_coord0_data    [ object_count ];
uint32    counts_index_data         [ object_count ];

#endif //OBJECT_DATA_HPP
