#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#ifdef __linux__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#endif

#include "maths.hpp"
#include "types.hpp"
#include "sdl.hpp"

const u32 OBJECT_COUNT = 10;
f32 floor_start_z = 0.0f;

struct GameState {
  f32       vp_mat[ 16 ]; // view perspective matrix
  f32       p_mat [ 16 ]; // projection matrix
  Position  eye      = { 0.0f, 5.0f, 10.0f };
  Position  look_at  = { 0.0f, 5.0f, 9.0f };
  s32       vbo;
  s32       ibo;
  u32       array_buffer_target           = 0;
  u32       element_array_buffer_target   = 0;
  u32       target_gl_offsets_array_data  = 0;
  u32       target_gl_offsets_index_data  = 0;
  u32       target_texture_data_array_pos = 0;
  bool32    invert_y                      = true;
};

struct GameObject {
  bool32    active = false;
  u32       index;
  Position  position;
  Position  rotation;
};

GameObject game_objects[ OBJECT_COUNT ];

// gl data
f32 gl_array_buffer_data         [ 1000000 ];
u16 gl_element_array_buffer_data [ 100000 ];
u8* texture_data_array = ( u8* ) malloc( 1000000 );

f32 light_position[ 3 ]  = { 0.0f, 20.0f, 0.0f };
f32 ambient_light        = 0.3f;

enum ShaderTypes { SHADER_LIGHT, SHADER_VERTEX_COLOURS, SHADER_VERTEX_COLOURS_NO_LIGHT };
enum ObjectTypes { OBJECT_TYPE_NONE, OBJECT_TYPE_SCENARY, OBJECT_TYPE_ENEMY };

// object data
Position  positions                 [ OBJECT_COUNT ];
Position  rotations                 [ OBJECT_COUNT ];
Position  velocities                [ OBJECT_COUNT ];
u32       shader_program_ids        [ OBJECT_COUNT ];
s32       gl_id_positions           [ OBJECT_COUNT ];
s32       gl_id_normals             [ OBJECT_COUNT ];
s32       gl_id_tex_coords0         [ OBJECT_COUNT ];
s32       gl_id_colours             [ OBJECT_COUNT ];
s32       gl_id_mvp_mats            [ OBJECT_COUNT ];
s32       gl_id_model_mats          [ OBJECT_COUNT ];
s32       gl_id_light_positions     [ OBJECT_COUNT ];
s32       gl_id_ambient_lights      [ OBJECT_COUNT ];
u32       tbos                      [ OBJECT_COUNT ];
u32       texture_buffer_offsets    [ OBJECT_COUNT ];
u32       texture_buffer_lengths    [ OBJECT_COUNT ];
u32       offsets_vertex_data       [ OBJECT_COUNT ];
u32       offsets_normal_data       [ OBJECT_COUNT ];
u32       offsets_tex_coord0_data   [ OBJECT_COUNT ];
u32       offsets_colour_data       [ OBJECT_COUNT ];
u32       offsets_index_data        [ OBJECT_COUNT ];
u32       gl_offsets_vertex_data    [ OBJECT_COUNT ];
u32       gl_offsets_normal_data    [ OBJECT_COUNT ];
u32       gl_offsets_tex_coord0_data[ OBJECT_COUNT ];
u32       gl_offsets_colour_data    [ OBJECT_COUNT ];
u32       gl_offsets_index_data     [ OBJECT_COUNT ];
u32       counts_vertex_data        [ OBJECT_COUNT ];
u32       counts_normal_data        [ OBJECT_COUNT ];
u32       counts_tex_coord0_data    [ OBJECT_COUNT ];
u32       counts_colour_data        [ OBJECT_COUNT ];
u32       counts_index_data         [ OBJECT_COUNT ];
u32       shader_types              [ OBJECT_COUNT ];
u32       object_types              [ OBJECT_COUNT ];
bool32    object_active             [ OBJECT_COUNT ];
bool32    slot_available            [ OBJECT_COUNT ]; // whether we can (re)use this object index to load a new object into
Position  flat_colours              [ OBJECT_COUNT ];
u8*       image_data_locations      [ OBJECT_COUNT ];

// HM = hardcoded mesh
enum mesh_source { LOAD_MESH_FROM_GLTF, HM_TARGET, HM_FLOOR1, HM_FLOOR2 };

struct ObjectLoadParameters {
  bool32    make_immediately_active = false;
  char*     shader_filename;
  u32       shader_type             = SHADER_LIGHT;
  u32       object_type             = OBJECT_TYPE_NONE;
  char*     gltf_model_filename;
  bool32    is_floor                = false;
  Position  initial_position;
  u32       mesh_source             = 0;
};

s32 get_free_object_index() {
  s32 result = -1;
  for( u32 i = 0; i < OBJECT_COUNT; i++ ) {
    if( slot_available[ i ] ) {
      result = i;
      return result;
    }
  }
  return result;
}

void render_object( u32 object_index, f32* vp_matrix ) {
  u32 i = object_index;
  
  if( !object_active[ i ] ) { return; }
  
  f32 vp[ 16 ];
  memcpy( &vp[ 0 ], vp_matrix, ( sizeof( f32 ) * 16 ) );
  
  f32 model_matrix[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  // move object in world space
  translate( &model_matrix[ 0 ], positions[ i ] );
  // do rotations
  f32 rot_mat_x[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  f32 rot_mat_y[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  f32 rot_mat_z[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  rotate_x( &rot_mat_x[ 0 ], rotations[ i ].x );
  rotate_y( &rot_mat_y[ 0 ], rotations[ i ].y );
  rotate_z( &rot_mat_z[ 0 ], rotations[ i ].z );
  
  f32 rot_mat_xy[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  f32 rot_mat_xyz[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  mat4_multiply( &rot_mat_xy[ 0 ], &rot_mat_y[ 0 ], &rot_mat_x[ 0 ] );
  mat4_multiply( &rot_mat_xyz[ 0 ], &rot_mat_z[ 0 ], &rot_mat_xy[ 0 ] );
  
  mat4_multiply( &model_matrix[ 0 ], &rot_mat_xyz[ 0 ], &model_matrix[ 0 ] );
  
  // model view projection matrix
  f32 mvp[ 16 ];
  mat4_multiply( &mvp[ 0 ], &model_matrix[ 0 ], &vp[ 0 ] );
  
  glUseProgram( shader_program_ids[ i ] );
  
  { // mvp
    s32     location      = gl_id_mvp_mats[ i ];
    s32     count         = 1;
    bool32  transpose     = GL_FALSE;
    f32*    mvp_position  = &mvp[ 0 ];
    glUniformMatrix4fv( location, count, transpose, mvp_position );
  }
  
  { // model matrix
    s32     location      = gl_id_model_mats[ i ];
    s32     count         = 1;
    bool32  transpose     = GL_FALSE;
    f32*    mvp_position  = &model_matrix[ 0 ];
    glUniformMatrix4fv( location, count, transpose, mvp_position );
  }
  
  if( shader_types[ i ] != SHADER_VERTEX_COLOURS_NO_LIGHT ) { // light position
    s32 location  = gl_id_light_positions[ i ];
    f32 v0        = light_position[ 0 ];
    f32 v1        = light_position[ 1 ];
    f32 v2        = light_position[ 2 ];
    
    glUniform3f( location, v0, v1, v2 );
  }
  
  if( shader_types[ i ] != SHADER_VERTEX_COLOURS_NO_LIGHT ) { // ambient light
    s32  location    = gl_id_ambient_lights[ i ];
    f32  v0          = ambient_light;
    glUniform1f( location, v0 );
  }
  
  { // vertices
    s32     index       = gl_id_positions[ i ];
    s32     size        = 3;
    u32     type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    s32     stride      = 0;
    u32     pointer     = gl_offsets_vertex_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  if( shader_types[ i ] != SHADER_VERTEX_COLOURS_NO_LIGHT ) { // normals
    s32     index       = gl_id_normals[ i ];
    s32     size        = 3;
    u32     type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    s32     stride      = 0;
    u32     pointer     = gl_offsets_normal_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  if( shader_types[ i ] == SHADER_LIGHT ) { // tex_coord0 s
    s32     index       = gl_id_tex_coords0[ i ];
    s32     size        = 2;
    u32     type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    s32     stride      = 0;
    u32     pointer     = gl_offsets_tex_coord0_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
    glBindTexture( GL_TEXTURE_2D, tbos[ i ] );
  }
  
  if( shader_types[ i ] == SHADER_VERTEX_COLOURS ) { // tex_coord0 s
    s32     index       = gl_id_colours[ i ];
    s32     size        = 3;
    u32     type        = GL_FLOAT;
    bool32  normalized  = GL_FALSE;
    s32     stride      = 0;
    u32     pointer     = gl_offsets_colour_data[ i ]; // misleading, it's not a pointer, it's where in the buffer it is - offset by number of bytes
    
    glVertexAttribPointer( index, size, type, normalized, stride, ( const GLvoid* )pointer );
  }
  
  if( shader_types[ i ] == SHADER_VERTEX_COLOURS_NO_LIGHT ) {
    // upload flat colour
    s32  location    = gl_id_colours[ i ];
    f32  v0          = flat_colours[ i ].x;
    f32  v1          = flat_colours[ i ].y;
    f32  v2          = flat_colours[ i ].z;
    
    glUniform3f( location, v0, v1, v2 );
  }
  
  { // draw elements
    s32 mode    = GL_TRIANGLES;
    s32 count   = counts_index_data[ i ];
    u32 type    = GL_UNSIGNED_SHORT;
    u32 indices = gl_offsets_index_data[ i ];
    
    glDrawElements( mode, count, type, ( const GLvoid* )indices );
  }
}

#ifdef __linux__

#pragma GCC diagnostic pop

#endif

#endif //OBJECT_DATA_HPP
