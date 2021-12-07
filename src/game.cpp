#include "game.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "json.hpp"
#include "input.hpp"
#include "vector_maths.hpp"
#include "object_data.hpp"
/*
void GLClearError() {
  while( glGetError() != GL_NO_ERROR );
}

bool GLLogCall( const char* function, const char* file, int line ) {
  while( GLenum error = glGetError() ) {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "[OpenGL error] %s\n%s\n%s\n%s\n", error, function, file, line );
    return false;
  }
  return true;
}
*/

struct Mat4 {
  real32  m00 = 1.0f;
  real32  m01 = 0.0f;
  real32  m02 = 0.0f;
  real32  m03 = 0.0f;
  real32  m10 = 0.0f;
  real32  m11 = 1.0f;
  real32  m12 = 0.0f;
  real32  m13 = 0.0f;
  real32  m20 = 0.0f;
  real32  m21 = 0.0f;
  real32  m22 = 1.0f;
  real32  m23 = 0.0f;
  real32  m30 = 0.0f;
  real32  m31 = 0.0f;
  real32  m32 = 0.0f;
  real32  m33 = 1.0f;
};

struct GameObject {
  uint32    id;
  real32    mvp[ 16 ];
  Position  position;
  real32    rotation_x      = 0.0f;
  real32    rotation_y      = 0.0f;
  real32    rotation_z      = 0.0f;
  int32     position_id;
  int32     normal_id;
  int32     tex_coord0_id;
  uint32    mesh_count;
  uint32    tbo;
  uint32    vertex_data_offset;
  uint32    vertex_data_count;
  uint32    normal_data_offset;
  uint32    normal_data_count;
  uint32    index_data_offset;
  uint32    index_data_count;
  uint32    tex_coord0_data_offset;
  uint32    tex_coord0_bytes;
  uint32    colour_data_offset;
  uint32    colour_data_count;
  uint32    shader_program_id;
  int32     mvp_id;
  int32     model_id;
  int32     model_view_id;
  int32     light_position_id;
  int32     ambient_light_id;
};


// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;

enum GltfContentType { GLTF_VERTICES, GLTF_INDICES, GLTF_NORMALS, GLTF_TEX_COORD0 };

uint32 get_bin_start_offset( const char* gltf_contents ) {
  uint32 result;
  GltfHeader* gltf_header   = ( GltfHeader* )gltf_contents; // pulls in fixed length json stuff too
  uint32  gltf_header_size_in_bytes = 12; // magic + version + length
  uint32  json_header_in_bytes      = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  uint32  json_string_in_bytes      = gltf_header -> json_chunk_length;
  uint32  bin_header_in_bytes       = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  result = gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes + bin_header_in_bytes;
  return result;
}

real32* get_gltf_data_pointer_real32( uint32 target_mesh_index, const char* gltf_contents, char* json_string, int32 content_type ) {
  
  real32* result = NULL;
  
  uint32  json_char_count   = string_length( json_string );
  
  GltfHeader* gltf_header   = ( GltfHeader* )gltf_contents; // pulls in fixed length json stuff too
  
  uint32  gltf_header_size_in_bytes = 12; // magic + version + length
  uint32  json_header_in_bytes      = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  uint32  bin_header_in_bytes       = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  uint32  json_string_in_bytes      = gltf_header -> json_chunk_length;
  uint32* bin_chunk_length          = ( uint32* )( ( uint8* )gltf_contents + gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes );
  uint32  bin_size_in_bytes         = *bin_chunk_length;
  uint32  bin_start_offset          = get_bin_start_offset( gltf_contents );
  uint32  json_chunk_in_bytes       = bin_header_in_bytes + json_string_in_bytes;
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  } else {
    return result;
  }
  
  BufferViewData buffer_view_data = get_buffer_view_data( accessor_data.buffer_view, json_string, json_char_count );
  
  uint32  total_offset = bin_start_offset + buffer_view_data.byte_offset;
  
  result = ( real32* )( ( char* )gltf_contents + total_offset );
  
  return result;
}

uint16* get_gltf_data_pointer_uint16( uint32 target_mesh_index, const char* gltf_contents, char* json_string, int32 content_type ) {
  
  uint16* result = NULL;
  
  uint32  json_char_count   = string_length( json_string );
  
  GltfHeader* gltf_header   = ( GltfHeader* )gltf_contents; // pulls in fixed length json stuff too
  
  uint32  gltf_header_size_in_bytes = 12; // magic + version + length
  uint32  json_header_in_bytes      = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  uint32  bin_header_in_bytes       = 8; // 4 bytes for chunk length, 4 bytes for chunk type
  uint32  json_string_in_bytes      = gltf_header -> json_chunk_length;
  uint32* bin_chunk_length          = ( uint32* )( ( uint8* )gltf_contents + gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes );
  uint32  bin_size_in_bytes         = *bin_chunk_length;
  uint32  bin_start_offset          = gltf_header_size_in_bytes + json_header_in_bytes + json_string_in_bytes + bin_header_in_bytes;
  uint32  json_chunk_in_bytes       = bin_header_in_bytes + json_string_in_bytes;
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  } else {
    return result;
  }
  
  BufferViewData buffer_view_data = get_buffer_view_data( accessor_data.buffer_view, json_string, json_char_count );
  
  uint32  total_offset = bin_start_offset + buffer_view_data.byte_offset;
  
  result = ( uint16* )( ( char* )gltf_contents + total_offset );
  
  return result;
}

uint32 get_gltf_data_count( uint32 target_mesh_index, const char* gltf_contents, char* json_string, int32 content_type ) {
  
  uint32 result = 0;
  
  uint32  json_char_count   = string_length( json_string );
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  } else if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  } else {
    return result;
  }
  
  uint32 count_per_value = 0;
  if( strings_are_equal( accessor_data.type, "VEC3" ) ) {
    count_per_value = 3;
  } else if( strings_are_equal( accessor_data.type, "SCALAR" ) ) {
    count_per_value = 1;
  } else if( strings_are_equal( accessor_data.type, "VEC2" ) ) {
    count_per_value = 2;
  }
  result = count_per_value * accessor_data.count;
  
  return result;
}

struct GltfbufferViewInfo {
  uint32 buffer_view;
  uint32 component_type;
  uint32 gltf_count; // count as defined by the JSON
  uint32 buffer;
  uint32 byte_length;
  uint32 byte_offset;
  uint32 count; // count of values
  uint32 offset;
  int32  type;
};

struct GltfBufferViewData {
  uint32 buffer;
  uint32 byte_length;
  uint32 byte_offset;
};

struct JsonString {
  char*   json_string;
  uint32  json_char_count;
};

GltfbufferViewInfo get_glft_buffer_view_info( uint32 target_mesh_index, const char* gltf_contents, char* json_string, int32 content_type ) {
  GltfbufferViewInfo result = {};
  
  uint32  json_char_count   = string_length( json_string );
  
  MeshPositionIndices mesh_position_indices = get_mesh_position_indices( target_mesh_index, json_string, json_char_count );
  AccessorData        accessor_data;
  
  if( content_type == GLTF_VERTICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.vertices, json_string, json_char_count );
  } else if( content_type == GLTF_INDICES ) {
    accessor_data = get_accessor_data( mesh_position_indices.indices, json_string, json_char_count );
  } else if( content_type == GLTF_NORMALS ) {
    accessor_data = get_accessor_data( mesh_position_indices.normals, json_string, json_char_count );
  } else if( content_type == GLTF_TEX_COORD0 ) {
    accessor_data = get_accessor_data( mesh_position_indices.texcoord_0, json_string, json_char_count );
  } else {
    return result;
  }
  
  BufferViewData buffer_view_data = get_buffer_view_data( accessor_data.buffer_view, json_string, json_char_count );
  
  result.buffer_view    = accessor_data.buffer_view;
  result.component_type = accessor_data.component_type;
  result.count          = accessor_data.count;
  result.type           = accessor_data.accessor_type;
  result.buffer         = buffer_view_data.buffer;
  result.byte_length    = buffer_view_data.byte_length;
  result.byte_offset    = buffer_view_data.byte_offset;
  
  return result;
}

inline uint32 get_count_from_type_string( uint32 gltf_count, char* gltf_type ) {
  
  uint32 result = 0;
  uint32 count_per_gltf_count = 0;
  
  if( strings_are_equal( gltf_type, "VEC3" ) ) {
    count_per_gltf_count = 3;
  } else if( strings_are_equal( gltf_type, "SCALAR" ) ) {
    count_per_gltf_count = 1;
  } else if( strings_are_equal( gltf_type, "VEC2" ) ) {
    count_per_gltf_count = 2;
  }
  
  result = count_per_gltf_count * gltf_count;
  
  return result;
}

inline uint32 get_count_from_type( uint32 gltf_count, int32 type ) {
  uint32 result = 0;
  uint32 count_per_gltf_count = 0;
  
  if( type == ACCESSOR_VEC3 ) {
    count_per_gltf_count = 3;
  } else if( type == ACCESSOR_SCALAR ) {
    count_per_gltf_count = 1;
  } else if( type == ACCESSOR_VEC2 ) {
    count_per_gltf_count = 2;
  }
  
  result = count_per_gltf_count * gltf_count;
  
  return result;
}

void load_game_object_gltf( GameObject* game_object, const char* model_filename, const char* shader_filename, real32 scale = 1.0f ) {
  
  ReadFileResult shader_file = read_entire_file( shader_filename );
  
  game_object -> shader_program_id = createShader( ( const char* )shader_file.contents, shader_file.contentsSize );
  free_memory( shader_file.contents, shader_file.contentsSize );
  
  ReadFileResult gltf_file  = read_entire_file( model_filename );
  uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
  
  char* json_string         = ( char* )malloc( json_string_length + 1 );
  
  pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
  
  JsonString json;
  json.json_string = json_string;
  json.json_char_count = string_length( json_string );
  
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "json is %s\n", json_string );
  game_object -> mesh_count = count_meshes( json_string, json_string_length );
  
  // log data vertices, indices, normals and tex_coord0
  const char* gltf_contents = ( char* )gltf_file.contents;
  void*       data_void;
  real32*     data;
  uint32      count;
  uint32      offset;
  uint32      byte_length;
  uint32      vertex_offset, index_offset, normal_offset, tex_coord0_offset;
  uint32      vertex_byte_length, index_byte_length, normal_byte_length, tex_coord0_byte_length;
  uint32      target_mesh_index = 0; // @TODO: Hardcoding for the initial mesh for now
  
  GltfbufferViewInfo buffer_view_info;
  
  // vertices
  buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json_string, GLTF_VERTICES );
  data              = get_gltf_data_pointer_real32( target_mesh_index, gltf_contents, json_string, GLTF_VERTICES );
  count             = get_gltf_data_count( target_mesh_index, gltf_contents, json_string, GLTF_VERTICES );
  byte_length       = buffer_view_info.byte_length;
  game_object       -> vertex_data_offset = log_array_buffer_data( data, count );
  game_object       -> vertex_data_count = get_count_from_type( count, buffer_view_info.type );
  glBufferSubData( GL_ARRAY_BUFFER, game_object -> vertex_data_offset, byte_length, data );
  
  // indices
  buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json_string, GLTF_INDICES );
  uint16* data16    = get_gltf_data_pointer_uint16( target_mesh_index, gltf_contents, json_string, GLTF_INDICES );
  count             = get_gltf_data_count( target_mesh_index, gltf_contents, json_string, GLTF_INDICES );
  byte_length       = buffer_view_info.byte_length;
  game_object       -> index_data_offset = log_element_array_buffer_data( data16, count );
  game_object       -> index_data_count = get_count_from_type( count, buffer_view_info.type );
  glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, game_object -> index_data_offset, byte_length, data16 );
  
  // // normals
  // buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json_string, GLTF_NORMALS );
  // data              = get_gltf_data_pointer_real32( target_mesh_index, gltf_contents, json_string, GLTF_NORMALS );
  // count             = get_gltf_data_count( target_mesh_index, gltf_contents, json_string, GLTF_NORMALS );
  // byte_length       = buffer_view_info.byte_length;
  // game_object       -> normal_data_offset = log_array_buffer_data( data, count );
  // game_object       -> normal_data_count = get_count_from_type( count, buffer_view_info.type );
  // glBufferSubData( GL_ARRAY_BUFFER, game_object -> normal_data_offset, byte_length, data );
  
  // // tex_coord0
  // buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json_string, GLTF_TEX_COORD0 );
  // data              = get_gltf_data_pointer_real32( target_mesh_index, gltf_contents, json_string, GLTF_TEX_COORD0 );
  // count             = get_gltf_data_count( target_mesh_index, gltf_contents, json_string, GLTF_TEX_COORD0 );
  // byte_length       = buffer_view_info.byte_length;
  // game_object       -> normal_data_offset = log_array_buffer_data( data, count );
  // game_object       -> normal_data_count = get_count_from_type( count, buffer_view_info.type );
  // glBufferSubData( GL_ARRAY_BUFFER, game_object -> tex_coord0_data_offset, byte_length, data );
  
  // int32 texture_width, texture_height, texture_bpp;
  
  // uint8*  texture_location = NULL;
  // uint32  texture_byte_length;
  
  // int32 image_buffer_view_index = get_image_buffer_view_index( json_string, 0 );
  // if( image_buffer_view_index >= 0  ) {
  //   BufferViewData image_buffer_view_data = get_buffer_view_data( image_buffer_view_index, json.json_string, json.json_char_count );
  //   uint32 bin_start_offset               = get_bin_start_offset( gltf_contents );
  //   uint32 image_data_total_offset        = bin_start_offset + image_buffer_view_data.byte_offset;
  //   texture_location                      = ( uint8* )( ( char* )gltf_contents + image_data_total_offset );
  //   texture_byte_length                   = image_buffer_view_data.byte_length;
  // }
  
  // const uchar* texture_buffer = ( uchar* )texture_location;
  // uchar* texture_data = stbi_load_from_memory( texture_buffer, texture_byte_length, &texture_width, &texture_height, &texture_bpp, 4 );
  
  free( json_string );
  
  // glGenTextures( 1, &game_object -> tbo );
  // glBindTexture( GL_TEXTURE_2D, game_object -> tbo );
  
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  // // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  // glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data );
  // glBindTexture( GL_TEXTURE_2D, game_object -> tbo );
  
  int32 position_id = glGetAttribLocation( game_object -> shader_program_id,  "aPosition" );
  
  game_object -> position_id        = glGetAttribLocation( game_object -> shader_program_id,  "aPosition" );
  // game_object -> normal_id          = glGetAttribLocation( game_object -> shader_program_id,  "aNormal" );
  // game_object -> tex_coord0_id      = glGetAttribLocation( game_object -> shader_program_id,  "aTexCoord" );
  game_object -> mvp_id             = glGetUniformLocation( game_object -> shader_program_id, "uMVP" );
  // game_object -> model_view_id      = glGetUniformLocation( game_object -> shader_program_id, "uModelViewMatrix" );
  // game_object -> model_id           = glGetUniformLocation( game_object -> shader_program_id, "uModelMatrix" );
  // game_object -> light_position_id  = glGetUniformLocation( game_object -> shader_program_id, "uLightPosition" );
  // game_object -> ambient_light_id   = glGetUniformLocation( game_object -> shader_program_id, "uAmbientLight" );
  
  glEnableVertexAttribArray( game_object -> position_id );
  // glEnableVertexAttribArray( game_object -> normal_id );
  // glEnableVertexAttribArray( game_object -> tex_coord0_id );
  
  real32* vertex_data_position      = &gl_array_buffer_data[ game_object -> vertex_data_offset ];
  // real32* normal_data_position      = &gl_array_buffer_data[ game_object -> normal_data_offset ];
  // real32* tex_coord0_data_position  = &gl_array_buffer_data[ game_object -> tex_coord0_data_offset ];
  
  glVertexAttribPointer( game_object -> position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )vertex_data_position );
  // glVertexAttribPointer( game_object -> normal_id     , 3, GL_FLOAT, GL_FALSE, 0, ( void* )normal_data_position );
  // glVertexAttribPointer( game_object -> tex_coord0_id , 2, GL_FLOAT, GL_FALSE, 0, ( void* )tex_coord0_data_position );
}

void calculate_ship_rotation( GameInput* game_input, GameObject* ship, real32 dt ) {
  
  // ###### X - up/down pitch
  real32 max_rotation_x = 30.0f;
  real32 current_x      = ship -> rotation_x;
  real32 joy_y          = game_input -> joy_axis_y;
  real32 lerp_smoothing = 0.05f;
  real32 target_x;
  
  if( current_x > max_rotation_x )
    current_x -= 360.0f;
  if( current_x < -max_rotation_x )
    current_x += 360.0f;
  
  if( joy_y < 0.0f ) { // up on the stick
    target_x = -( max_rotation_x * joy_y );
  } else if( joy_y > 0.0f ) { // down on the stick
    target_x = ( -max_rotation_x * joy_y );
  } else {
    target_x = 0.0f;
  }
  
  //target_x = 0.0f;
  
  real32 new_rotation_x = lerp_dt( current_x, target_x, lerp_smoothing, dt );
  ship -> rotation_x    = new_rotation_x;
  
  // ###### Y - turn
  real32 max_rotation_y = 30.0f;
  real32 current_y      = ship -> rotation_y;
  real32 joy_x          = game_input -> joy_axis_x;
  real32 target_y;
  
  if( current_y > max_rotation_y )
    current_y -= 360.0f;
  if( current_y < -max_rotation_y )
    current_y += 360.0f;
  
  if( joy_x < 0.0f ) { // left on the stick
    target_y = -( max_rotation_y * joy_x );
  } else if( joy_x > 0.0f ) { // right on the stick
    target_y = ( -max_rotation_y * joy_x );
  } else {
    target_y = 0.0f;
  }
  
  real32 new_rotation_y = lerp_dt( current_y, target_y, lerp_smoothing, dt );
  ship -> rotation_y    = new_rotation_y;
  
  // ###### Z - tilt
  real32 max_rotation_z = 20.0f;
  real32 current_z      = ship -> rotation_z;
  real32 target_z;
  
  if( current_z > max_rotation_z )
    current_z -= 360.0f;
  if( current_z < -max_rotation_z )
    current_z += 360.0f;
  
  if( joy_x < 0.0f ) { // left on the stick
    target_z = -( max_rotation_z * joy_x );
  } else if( joy_x > 0.0f ) { // right on the stick
    target_z = ( -max_rotation_z * joy_x );
  } else {
    target_z = 0.0f;
  }
  
  real32 new_rotation_z = lerp_dt( current_z, target_z, lerp_smoothing, dt );
  ship -> rotation_z    = new_rotation_z;
  
}

void dump_mat4( real32* mat ) {
  
  for( uint32 i = 0; i < 15; i++ ) {
    real32 this_float = *mat;
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "float %d is %f\n", i, this_float );
    mat++;
  }
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "#############################################\n" );
}

inline void translate( real32* matrix, Position position ) {
  matrix += 12;
  *matrix = position.x;
  matrix++;
  *matrix = position.y;
  matrix++;
  *matrix = position.z;
}

uint32 init_game( game_memory* memory ) {
  
  SDLObjects sdlObjects;
  
  Buttons_pressed old_buttons;
  Buttons_pressed new_buttons;
  
  init_sdl( &sdlObjects );
  
  SDL_Window* window = sdlObjects.window;
  
  real32 light_position[ 3 ]  = { 0.0f, 5.0f, 0.0f };
  real32 ambient_light        = 0.3f;
  
  real32  aspect = ( real32 ) sdlObjects.windowWidth / ( real32 ) sdlObjects.windowHeight;
  
  real32 vp[ 16 ] = {}; // view projection matrix
  real32 v[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  uint32  vbo;
  uint32  ibo;
  
  initialise_gamepads();
  
  const uint32 object_count = 1;
  GameObject game_objects[ object_count ];
  
  bool32  running = true;
  
  do { // start main loop
    
    if( !memory -> isInitialized ) { // on first run
      
      glGenBuffers( 1, &vbo );
      glBindBuffer( GL_ARRAY_BUFFER, vbo );
      glBufferData( GL_ARRAY_BUFFER, Megabytes( 50 ), 0, GL_STATIC_DRAW );
      
      glGenBuffers( 1, &ibo );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, Megabytes( 50 ), 0, GL_STATIC_DRAW );
      
      load_game_object_gltf( &game_objects[ 0 ], "modelShip.glb"     , "shaderBasic.glsl" );
      // load_game_object_gltf( &game_objects[ 1 ], "modelEnemyPod.glb" , "shaderLight.glsl" );
      
      game_objects[ 0 ].position.x -= 2.0f;
      // game_objects[ 1 ].position.x += 2.0f;
      // game_objects[ 1 ].position.z -= 5.0f;
      // game_objects[ 1 ].position.y += 5.0f;
      
      // projection matrix
      real32 p[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
      real32 fov = 70.0f;
      real32 near_plane = 0.1f;
      real32 far_plane = 100.0f;
      populate_perspective_matrix( &p[ 0 ], fov, aspect, near_plane, far_plane );
      
      Position eye = { 7.0f, 7.0f, 7.0f };
      Position centre = { 1.0f, 1.0f, 1.0f };
      
      populate_view_matrix( &v[ 0 ], eye, centre );
      
      mat4_multiply( &vp[ 0 ], &v[ 0 ], &p[ 0 ] );
      
      memory -> isInitialized = true;
      
      current_time = 1; // so dt calc doesn't do weird things
    }
    
    previous_time = current_time;
    current_time = SDL_GetTicks();
    
    uint32 ms_since_last_frame  = current_time - previous_time;
    real32 dt = ( real32 )ms_since_last_frame / 1000.0f;
    
    if( dt > 0.15f ) dt = 0.15f; // prevent weird outliers e.g. first frame
    
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "%d ms == %f dt\n", ms_since_last_frame, dt );
    
    reset_game_inputs_pressed( &old_buttons, &new_buttons );
    
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      handle_sdl_input_event( &event, &old_buttons, &new_buttons );
    }
    
    bool32 invert_y = false;
    
    GameInput game_input = get_game_input_state( old_buttons, new_buttons, invert_y );
    
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "x is %f\n", game_input.joy_axis_x );
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "y is %f\n", game_input.joy_axis_y );
    
    if( game_input.quit )
      running = false;
    
    int32 mvpID;
    
    // update and render
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    calculate_ship_rotation( &game_input, &game_objects[ 0 ], dt );
    
    // game_objects[ 1 ].rotation_x += 0.02f;
    // game_objects[ 1 ].rotation_y += 2.0f;
    // game_objects[ 1 ].rotation_z += 6.0f;
    
    for( uint32 i = 0; i < object_count; i++ ) {
      
      GameObject* game_object = &game_objects[ i ];
      
      if( game_object -> rotation_x > 360.0f ) game_object -> rotation_x -= 360.0f;
      if( game_object -> rotation_y > 360.0f ) game_object -> rotation_y -= 360.0f;
      if( game_object -> rotation_z > 360.0f ) game_object -> rotation_z -= 360.0f;
      if( game_object -> rotation_x < 0.0f )   game_object -> rotation_x += 360.0f;
      if( game_object -> rotation_y < 0.0f )   game_object -> rotation_y += 360.0f;
      if( game_object -> rotation_z < 0.0f )   game_object -> rotation_z += 360.0f;
      
      real32 model_matrix[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_x[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_y[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_z[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      
      translate( &model_matrix[ 0 ], game_object -> position );
      rotate_x( &rot_mat_x[ 0 ], game_object -> rotation_x );
      rotate_y( &rot_mat_y[ 0 ], game_object -> rotation_y );
      rotate_z( &rot_mat_z[ 0 ], game_object -> rotation_z );
      
      real32 rot_mat_xy[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_xyz[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      mat4_multiply( &rot_mat_xy[ 0 ], &rot_mat_y[ 0 ], &rot_mat_x[ 0 ] );
      mat4_multiply( &rot_mat_xyz[ 0 ], &rot_mat_z[ 0 ], &rot_mat_xy[ 0 ] );
      
      mat4_multiply( &model_matrix[ 0 ], &rot_mat_xyz[ 0 ], &model_matrix[ 0 ] );
      
      real32 mvp[ 16 ]  = {}; // model view projection matrix
      
      // multiply model matrix with the view-projection matrix to get the mvp matrix
      // mat4_multiply( &game_objects[ i ].mvp[ 0 ], &model_matrix[ 0 ], &view_projection_matrix[ 0 ] );
      mat4_multiply( &mvp[ 0 ], &model_matrix[ 0 ], &vp[ 0 ] );
      
      glUseProgram( game_objects[ i ].shader_program_id );
      glUniformMatrix4fv( game_objects[ i ].mvp_id  , 1, GL_FALSE, &mvp[ 0 ] );
      // glUniformMatrix4fv( game_objects[ i ].model_id, 1, GL_FALSE, &model_matrix[ 0 ] );
      // glUniform3f( game_objects[ i ].light_position_id, light_position[ 0 ] , light_position[ 1 ] , light_position[ 2 ] );
      // glUniform1f( game_objects[ i ].ambient_light_id , ambient_light );
      
      real32* vertex_data_position      = &gl_array_buffer_data[ game_object -> vertex_data_offset ];
      // real32* normal_data_position      = &gl_array_buffer_data[ game_object -> normal_data_offset ];
      // real32* tex_coord0_data_position  = &gl_array_buffer_data[ game_object -> tex_coord0_data_offset ];
      
      glVertexAttribPointer( game_object -> position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )vertex_data_position );
      // glVertexAttribPointer( game_object -> normal_id     , 3, GL_FLOAT, GL_FALSE, 0, ( void* )normal_data_position );
      // glVertexAttribPointer( game_object -> tex_coord0_id , 2, GL_FLOAT, GL_FALSE, 0, ( void* )tex_coord0_data_position );
      
      // glBindTexture( GL_TEXTURE_2D, game_object -> tbo );
      
      uint32* index_data_location = &gl_element_array_buffer_data[ game_object -> index_data_offset ];
      
      glDrawElements( GL_TRIANGLES, game_object -> index_data_count, GL_UNSIGNED_INT, ( void* )index_data_location );
      
    }
    
    // light_position[ 1 ] += 0.05f;
    
    uint64 before_frame_flip_ticks = SDL_GetTicks();
    
    SDL_GL_SwapWindow( window );
    
    uint64 end_frame_ticks = SDL_GetTicks();
    
  } while( running );
  
  return 0;
}
