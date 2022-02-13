#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#ifdef __linux__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#endif

#include "vector_maths.hpp"
#include "types.hpp"

const uint32 object_count = 4;

struct GameObject {
  bool32    active = false;
  uint32    index;
  Position  position;
  Position  rotation;
};

GameObject game_objects[ object_count ];

// gl data
real32 gl_array_buffer_data         [ 1000000 ];
uint16 gl_element_array_buffer_data [ 100000 ];
uint8* texture_data_array = ( uint8* )malloc( 1000000 );

real32 light_position[ 3 ]  = { 0.0f, 20.0f, 0.0f };
real32 ambient_light        = 0.3f;

enum ShaderTypes {
  SHADERS_NONE, SHADER_LIGHT, SHADER_VERTEX_COLOURS
};

// object data
Position  positions                 [ object_count ];
Position  rotations                 [ object_count ];
Position  velocities                [ object_count ];
uint32    shader_program_ids        [ object_count ];
int32     gl_id_positions           [ object_count ];
int32     gl_id_normals             [ object_count ];
int32     gl_id_tex_coords0         [ object_count ];
int32     gl_id_colours             [ object_count ];
int32     gl_id_mvp_mats            [ object_count ];
int32     gl_id_model_mats          [ object_count ];
int32     gl_id_light_positions     [ object_count ];
int32     gl_id_ambient_lights      [ object_count ];
uint32    tbos                      [ object_count ];
uint32    texture_buffer_offsets    [ object_count ];
uint32    texture_buffer_lengths    [ object_count ];
uint32    offsets_vertex_data       [ object_count ];
uint32    offsets_normal_data       [ object_count ];
uint32    offsets_tex_coord0_data   [ object_count ];
uint32    offsets_colour_data       [ object_count ];
uint32    offsets_index_data        [ object_count ];
uint32    gl_offsets_vertex_data    [ object_count ];
uint32    gl_offsets_normal_data    [ object_count ];
uint32    gl_offsets_tex_coord0_data[ object_count ];
uint32    gl_offsets_colour_data    [ object_count ];
uint32    gl_offsets_index_data     [ object_count ];
uint32    counts_vertex_data        [ object_count ];
uint32    counts_normal_data        [ object_count ];
uint32    counts_tex_coord0_data    [ object_count ];
uint32    counts_colour_data        [ object_count ];
uint32    counts_index_data         [ object_count ];
uint32    shader_types              [ object_count ];
bool32    object_active             [ object_count ];
uint8*    image_data_locations      [ object_count ];

void render_object( uint32 object_index, real32* vp_matrix ) {
  uint32 i = object_index;
  
  if( !object_active[ i ] ) { return; }
  
  real32 vp[ 16 ];
  memcpy( &vp[ 0 ], vp_matrix, ( sizeof( real32 ) * 16 ) );
  
  real32 model_matrix[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  // move object in world space
  translate( &model_matrix[ 0 ], positions[ i ] );
  // do rotations
  real32 rot_mat_x[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  real32 rot_mat_y[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  real32 rot_mat_z[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  rotate_x( &rot_mat_x[ 0 ], rotations[ i ].x );
  rotate_y( &rot_mat_y[ 0 ], rotations[ i ].y );
  rotate_z( &rot_mat_z[ 0 ], rotations[ i ].z );
  
  real32 rot_mat_xy[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  real32 rot_mat_xyz[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  mat4_multiply( &rot_mat_xy[ 0 ], &rot_mat_y[ 0 ], &rot_mat_x[ 0 ] );
  mat4_multiply( &rot_mat_xyz[ 0 ], &rot_mat_z[ 0 ], &rot_mat_xy[ 0 ] );
  
  mat4_multiply( &model_matrix[ 0 ], &rot_mat_xyz[ 0 ], &model_matrix[ 0 ] );
  
  // model view projection matrix
  real32 mvp[ 16 ];
  mat4_multiply( &mvp[ 0 ], &model_matrix[ 0 ], &vp[ 0 ] );
  
  glUseProgram( shader_program_ids[ i ] );
  
  { // mvp
    int32   location    = gl_id_mvp_mats[ i ];
    int32   count       = 1;
    bool32  transpose   = GL_FALSE;
    real32* mvp_position  = &mvp[ 0 ];
    glUniformMatrix4fv( location, count, transpose, mvp_position );
  }
  
  { // model matrix
    int32   location    = gl_id_model_mats[ i ];
    int32   count       = 1;
    bool32  transpose   = GL_FALSE;
    real32* mvp_position  = &model_matrix[ 0 ];
    glUniformMatrix4fv( location, count, transpose, mvp_position );
  }
  
  { // light position
    int32   location    = gl_id_light_positions[ i ];
    real32  v0          = light_position[ 0 ];
    real32  v1          = light_position[ 1 ];
    real32  v2          = light_position[ 2 ];
    
    glUniform3f( location, v0, v1, v2 );
  }
  
  { // ambient light
    int32   location    = gl_id_ambient_lights[ i ];
    real32  v0          = ambient_light;
    glUniform1f( location, v0 );
  }
  
  { // vertices
    int32   index       = gl_id_positions[ i ];
    int32   size        = 3;
    uint32  type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    int32   stride      = 0;
    uint32  pointer     = gl_offsets_vertex_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  { // normals
    int32   index       = gl_id_normals[ i ];
    int32   size        = 3;
    uint32  type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    int32   stride      = 0;
    uint32  pointer     = gl_offsets_normal_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  if( shader_types[ i ] == SHADER_LIGHT ) { // tex_coord0 s
    int32   index       = gl_id_tex_coords0[ i ];
    int32   size        = 2;
    uint32  type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    int32   stride      = 0;
    uint32  pointer     = gl_offsets_tex_coord0_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
    glBindTexture( GL_TEXTURE_2D, tbos[ i ] );
  }
  
  if( shader_types[ i ] == SHADER_VERTEX_COLOURS ) { // tex_coord0 s
    int32   index       = gl_id_colours[ i ];
    int32   size        = 3;
    uint32  type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    int32   stride      = 0;
    uint32  pointer     = gl_offsets_colour_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  { // draw elements
    int32   mode        = GL_TRIANGLES;
    int32   count       = counts_index_data[ i ];
    uint32  type        = GL_UNSIGNED_SHORT;
    uint32 indices      = gl_offsets_index_data[ i ];
    
    glDrawElements( mode, count, type, ( const GLvoid* )indices );
  }
}

#ifdef __linux__

#pragma GCC diagnostic pop

#endif

#endif //OBJECT_DATA_HPP
