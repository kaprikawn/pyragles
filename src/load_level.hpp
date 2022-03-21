#ifndef LOAD_LEVEL_HPP
#define LOAD_LEVEL_HPP

#include <string.h>
#include "../vendor/stb_image.h"
#include "object_data.hpp"
#include "types.hpp"
#include "game.hpp"
#include "json.hpp"

void reset_data() {
  
  memset( positions                 , 0, ( OBJECT_COUNT * sizeof( positions[ 0 ] ) ) );
  memset( rotations                 , 0, ( OBJECT_COUNT * sizeof( rotations[ 0 ] ) ) );
  memset( velocities                , 0, ( OBJECT_COUNT * sizeof( velocities[ 0 ] ) ) );
  memset( shader_program_ids        , 0, ( OBJECT_COUNT * sizeof( shader_program_ids[ 0 ] ) ) );
  memset( gl_id_positions           , 0, ( OBJECT_COUNT * sizeof( gl_id_positions[ 0 ] ) ) );
  memset( gl_id_positions           , 0, ( OBJECT_COUNT * sizeof( gl_id_positions[ 0 ] ) ) );
  memset( gl_id_normals             , 0, ( OBJECT_COUNT * sizeof( gl_id_normals[ 0 ] ) ) );
  memset( gl_id_tex_coords0         , 0, ( OBJECT_COUNT * sizeof( gl_id_tex_coords0[ 0 ] ) ) );
  memset( gl_id_colours             , 0, ( OBJECT_COUNT * sizeof( gl_id_colours[ 0 ] ) ) );
  memset( gl_id_mvp_mats            , 0, ( OBJECT_COUNT * sizeof( gl_id_mvp_mats[ 0 ] ) ) );
  memset( gl_id_model_mats          , 0, ( OBJECT_COUNT * sizeof( gl_id_model_mats[ 0 ] ) ) );
  memset( gl_id_light_positions     , 0, ( OBJECT_COUNT * sizeof( gl_id_light_positions[ 0 ] ) ) );
  memset( gl_id_ambient_lights      , 0, ( OBJECT_COUNT * sizeof( gl_id_ambient_lights[ 0 ] ) ) );
  memset( tbos                      , 0, ( OBJECT_COUNT * sizeof( tbos[ 0 ] ) ) );
  memset( texture_buffer_offsets    , 0, ( OBJECT_COUNT * sizeof( texture_buffer_offsets[ 0 ] ) ) );
  memset( texture_buffer_lengths    , 0, ( OBJECT_COUNT * sizeof( texture_buffer_lengths[ 0 ] ) ) );
  memset( offsets_vertex_data       , 0, ( OBJECT_COUNT * sizeof( offsets_vertex_data[ 0 ] ) ) );
  memset( offsets_normal_data       , 0, ( OBJECT_COUNT * sizeof( offsets_normal_data[ 0 ] ) ) );
  memset( offsets_tex_coord0_data   , 0, ( OBJECT_COUNT * sizeof( offsets_tex_coord0_data[ 0 ] ) ) );
  memset( offsets_colour_data       , 0, ( OBJECT_COUNT * sizeof( offsets_colour_data[ 0 ] ) ) );
  memset( offsets_index_data        , 0, ( OBJECT_COUNT * sizeof( offsets_index_data[ 0 ] ) ) );
  memset( gl_offsets_vertex_data    , 0, ( OBJECT_COUNT * sizeof( gl_offsets_vertex_data[ 0 ] ) ) );
  memset( gl_offsets_normal_data    , 0, ( OBJECT_COUNT * sizeof( gl_offsets_normal_data[ 0 ] ) ) );
  memset( gl_offsets_tex_coord0_data, 0, ( OBJECT_COUNT * sizeof( gl_offsets_tex_coord0_data[ 0 ] ) ) );
  memset( gl_offsets_colour_data    , 0, ( OBJECT_COUNT * sizeof( gl_offsets_colour_data[ 0 ] ) ) );
  memset( gl_offsets_index_data     , 0, ( OBJECT_COUNT * sizeof( gl_offsets_index_data[ 0 ] ) ) );
  memset( counts_vertex_data        , 0, ( OBJECT_COUNT * sizeof( counts_vertex_data[ 0 ] ) ) );
  memset( counts_normal_data        , 0, ( OBJECT_COUNT * sizeof( counts_normal_data[ 0 ] ) ) );
  memset( counts_tex_coord0_data    , 0, ( OBJECT_COUNT * sizeof( counts_tex_coord0_data[ 0 ] ) ) );
  memset( counts_colour_data        , 0, ( OBJECT_COUNT * sizeof( counts_colour_data[ 0 ] ) ) );
  memset( counts_index_data         , 0, ( OBJECT_COUNT * sizeof( counts_index_data[ 0 ] ) ) );
  memset( shader_types              , 0, ( OBJECT_COUNT * sizeof( shader_types[ 0 ] ) ) );
  memset( object_active             , 0, ( OBJECT_COUNT * sizeof( object_active[ 0 ] ) ) );
  
}

void check_gltf_file( void* gltf_file_contents ) {
  GltfHeader* gltf_header = ( GltfHeader* )gltf_file_contents;
  
  uint32 magic = gltf_header -> magic;
  
  if( magic != 1179937895 ) {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Error - expected gltf2 file not found\n" );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "  Please make sure you have git-lfs installed and you have run git lfs install\n" );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "  Once this is done, delete the stub glb files in the 'assets' directory and refresh your repo\n\n" );
  }
}

void load_level_object( ObjectLoadParameters olp, uint32 array_position_index, GameState* game_state ) {
  
  uint32 i = array_position_index;
  
  if( olp.make_immediately_active ) {
    object_active[ i ] = true;
  }
  
  const char* shader_filename = ( const char* )olp.shader_filename;
  ReadFileResult shader_file  = read_entire_file( shader_filename );
  
  shader_types[ i ] = olp.shader_type;
  
  uint32 shader_program_id    = createShader( shader_file );
  shader_program_ids[ i ]     = shader_program_id;
  glUseProgram( shader_program_id );
  
  int32 position_attribute_location     = glGetAttribLocation ( shader_program_id, "aPosition" );
  int32 normal_attribute_location       = glGetAttribLocation ( shader_program_id, "aNormal" );
  int32 tex_coord0_attribute_location   = glGetAttribLocation ( shader_program_id, "aTexCoord" );
  // int32 colour_attribute_location       = glGetAttribLocation ( shader_program_id, "aColour" );
  int32 mvp_uniform_location            = glGetUniformLocation( shader_program_id, "uMVP" );
  int32 model_uniform_location          = glGetUniformLocation( shader_program_id, "uModelMatrix" );
  int32 light_position_uniform_location = glGetUniformLocation( shader_program_id, "uLightPosition" );
  int32 ambient_light_uniform_location  = glGetUniformLocation( shader_program_id, "uAmbientLight" );
  
  gl_id_positions[ i ]        = position_attribute_location;
  gl_id_normals[ i ]          = normal_attribute_location;
  gl_id_tex_coords0[ i ]      = tex_coord0_attribute_location;
  // gl_id_colours[ i ]          = colour_attribute_location;
  gl_id_mvp_mats[ i ]         = mvp_uniform_location;
  gl_id_model_mats[ i ]       = model_uniform_location;
  gl_id_light_positions[ i ]  = light_position_uniform_location;
  gl_id_ambient_lights[ i ]   = ambient_light_uniform_location;
  
  GLCall( glEnableVertexAttribArray( position_attribute_location ) );
  GLCall( glEnableVertexAttribArray( normal_attribute_location ) );
  GLCall( glEnableVertexAttribArray( tex_coord0_attribute_location ) );
  
  if( olp.from_gltf ) {
    const char* gltf_filename = ( const char* )olp.gltf_model_filename;
    ReadFileResult gltf_file  = read_entire_file( gltf_filename );
    
    check_gltf_file( gltf_file.contents );
    uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
    char* json_string         = ( char* )malloc( json_string_length + 1 );
    pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
    
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "JSON : \n%s\n\n", json_string );
    
    JsonString json;
    json.json_string      = json_string;
    json.json_char_count  = string_length( json_string );
    
    const char*         gltf_contents = ( char* )gltf_file.contents;
    uint32              target_mesh_index = 0; // @TODO: Hardcoding for the initial mesh for now
    void*               data;
    uint32              byte_length;
    uint32              count;
    GltfbufferViewInfo  buffer_view_info;
    
    // vertex data
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_VERTICES );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_VERTICES );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_VERTICES );
    count *= 3; // three values per vertex
    byte_length = ( count * sizeof( real32 ) );
    
    counts_vertex_data[ i ]   = count;
    offsets_vertex_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_vertex_data[ i ] ];
      void*   src   = data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // normals data
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    count *= 3; // three values per vertex
    byte_length = ( count * sizeof( real32 ) );
    
    counts_normal_data[ i ]   = count;
    offsets_normal_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_normal_data[ i ] ];
      void*   src   = data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // tex_coord0 data
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    count *= 2; // two values per uv coordinate
    byte_length = ( count * sizeof( real32 ) );
    
    counts_tex_coord0_data[ i ]   = count;
    offsets_tex_coord0_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_tex_coord0_data[ i ] ];
      void*   src   = data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // indices
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_INDICES );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_INDICES );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_INDICES );
    byte_length = ( count * sizeof( uint16 ) );
    
    counts_index_data[ i ]   = count;
    offsets_index_data[ i ]  = game_state -> element_array_buffer_target;
    {
      void*   dest  = ( void* )&gl_element_array_buffer_data[ offsets_index_data[ i ] ];
      void*   src   = data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> element_array_buffer_target += count;
    
    
    int32 image_buffer_view_index = get_image_buffer_view_index( json.json_string, json.json_char_count );
    if( image_buffer_view_index >= 0  ) {
      
      // don't need to store image data so just upload straight to gl
      data                                = get_gltf_data_pointer( target_mesh_index, gltf_contents, json, GLTF_IMAGE );
      BufferViewData  image_buffer_view   = get_buffer_view_data( image_buffer_view_index, json.json_string, json.json_char_count );
      uint32 image_data_bytes = image_buffer_view.byte_length;
      
      int32 texture_width, texture_height, texture_bpp;
      
      uchar* texture_data = stbi_load_from_memory( ( const uchar* )data, image_data_bytes, &texture_width, &texture_height, &texture_bpp, 4 );
      
      GLCall( glGenTextures( 1, &tbos[ i ] ) );
      GLCall( glBindTexture( GL_TEXTURE_2D, tbos[ i ] ) );
      GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
      GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
      GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
      GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
      // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
      GLCall( glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
      GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data ) );
      GLCall( glBindTexture( GL_TEXTURE_2D, tbos[ i ] ) );
    
    }
    
    free( json_string );
  }
  
}

void load_ship_and_target( GameState* game_state ) {
  
  ObjectLoadParameters object_load_parameters;
  object_load_parameters.make_immediately_active = true;
  object_load_parameters.shader_filename = "modelShip.glb";
  
  load_level_object( object_load_parameters, 0, game_state );
  
}

#endif //LOAD_LEVEL_HPP
