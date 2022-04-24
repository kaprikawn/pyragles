#ifndef LOAD_LEVEL_HPP
#define LOAD_LEVEL_HPP

#include <iostream>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
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
  
  for( uint32 i = 0; i < OBJECT_COUNT; i++ ) {
    slot_available[ i ] = true;
  }
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

void load_level_object( ObjectLoadParameters* olp, uint32 target_array_position, GameState* game_state ) {
  
  uint32 i = target_array_position;
  
  if( olp -> make_immediately_active ) {
    object_active[ i ] = true;
  }
  
  slot_available[ i ] = false;
  
  positions[ i ].x = olp -> initial_position.x;
  positions[ i ].y = olp -> initial_position.y;
  positions[ i ].z = olp -> initial_position.z;
  
  const char* shader_filename = ( const char* )olp -> shader_filename;
  ReadFileResult shader_file  = read_entire_file( shader_filename );
  
  shader_types[ i ] = olp -> shader_type;
  if( olp -> object_type > 0 )
    object_types[ i ] = olp -> object_type;
  
  uint32 shader_program_id    = createShader( shader_file );
  shader_program_ids[ i ]     = shader_program_id;
  glUseProgram( shader_program_id );
  
  int32 position_attribute_location     = glGetAttribLocation ( shader_program_id, "aPosition" );
  int32 normal_attribute_location       = glGetAttribLocation ( shader_program_id, "aNormal" );
  int32 tex_coord0_attribute_location   = glGetAttribLocation ( shader_program_id, "aTexCoord" );
  int32 colour_attribute_location       = glGetAttribLocation ( shader_program_id, "aColour" );
  int32 mvp_uniform_location            = glGetUniformLocation( shader_program_id, "uMVP" );
  int32 model_uniform_location          = glGetUniformLocation( shader_program_id, "uModelMatrix" );
  int32 light_position_uniform_location = glGetUniformLocation( shader_program_id, "uLightPosition" );
  int32 ambient_light_uniform_location  = glGetUniformLocation( shader_program_id, "uAmbientLight" );
  
  gl_id_positions[ i ]        = position_attribute_location;
  gl_id_normals[ i ]          = normal_attribute_location;
  gl_id_tex_coords0[ i ]      = tex_coord0_attribute_location;
  gl_id_colours[ i ]          = colour_attribute_location;
  gl_id_mvp_mats[ i ]         = mvp_uniform_location;
  gl_id_model_mats[ i ]       = model_uniform_location;
  gl_id_light_positions[ i ]  = light_position_uniform_location;
  gl_id_ambient_lights[ i ]   = ambient_light_uniform_location;
  
  if( shader_types[ i ] == SHADER_VERTEX_COLOURS_NO_LIGHT ) {
    int32 colour_uniform_location = glGetUniformLocation( shader_program_id, "uColour" );
    gl_id_colours[ i ]            = colour_uniform_location;
  } else if( shader_types[ i ] == SHADER_VERTEX_COLOURS ) {
    int32 colour_uniform_location = glGetUniformLocation( shader_program_id, "aColour" );
    gl_id_colours[ i ]            = colour_uniform_location;
  }
  
  GLCall( glEnableVertexAttribArray( position_attribute_location ) );
  
  if( normal_attribute_location >= 0 ) {
    GLCall( glEnableVertexAttribArray( normal_attribute_location ) );
  }
  if( tex_coord0_attribute_location >= 0 ) {
    GLCall( glEnableVertexAttribArray( tex_coord0_attribute_location ) );
  }
  
  if( olp -> mesh_source == LOAD_MESH_FROM_GLTF ) {
    const char* gltf_filename = ( const char* )olp -> gltf_model_filename;
    ReadFileResult gltf_file  = read_entire_file( gltf_filename );
    
    check_gltf_file( gltf_file.contents );
    uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
    char* json_string         = ( char* )malloc( json_string_length + 1 );
    pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
    
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "JSON : \n%s\n\n", json_string );
    
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
  } else if( olp -> mesh_source == HM_TARGET ) {
    {
      real32* vertices  = get_target_vertices( &counts_vertex_data[ i ] );
      offsets_vertex_data[ i ]  = game_state -> array_buffer_target;
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_vertex_data[ i ] ];
      void*   src   = ( void* )&vertices[ 0 ];
      uint32  bytes = ( sizeof( vertices[ 0 ] ) * counts_vertex_data[ i ] );
      memcpy( dest, src, bytes );
      free( vertices );
      game_state -> array_buffer_target += counts_vertex_data[ i ];
    }
    
    {
      uint16* indices   = get_target_indices( &counts_index_data[ i ] );
      offsets_index_data[ i ]  = game_state -> element_array_buffer_target;
      void*   dest  = ( void* )&gl_element_array_buffer_data[ offsets_index_data[ i ] ];
      void*   src   = ( void* )&indices[ 0 ];
      uint32  bytes = ( sizeof( indices[ 0 ] ) * counts_index_data[ i ] );
      memcpy( dest, src, bytes );
      free( indices );
      game_state -> element_array_buffer_target += counts_index_data[ i ];
    }
  } else if( olp -> mesh_source == HM_FLOOR1 || olp -> mesh_source == HM_FLOOR2 ) {
    
    {
      real32* vertices;
      if( olp -> mesh_source == HM_FLOOR1 ) {
        vertices = get_underside_floor_vertices( &counts_vertex_data[ i ] );
      } else if( olp -> mesh_source == HM_FLOOR2 ) {
        vertices = get_overside_floor_vertices( &counts_vertex_data[ i ] );
      }
      offsets_vertex_data[ i ]  = game_state -> array_buffer_target;
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_vertex_data[ i ] ];
      void*   src   = ( void* )&vertices[ 0 ];
      uint32  bytes = ( sizeof( vertices[ 0 ] ) * counts_vertex_data[ i ] );
      memcpy( dest, src, bytes );
      free( vertices );
      game_state -> array_buffer_target += counts_vertex_data[ i ];
      
      counts_normal_data[ i ] = counts_vertex_data[ i ];
      counts_colour_data[ i ] = counts_vertex_data[ i ];
    }
    
    { // normals
      real32* normals;
      if( olp -> mesh_source == HM_FLOOR1 ) {
        normals = get_underside_floor_normals( &counts_normal_data[ i ] );
      } else {
        normals = get_overside_floor_normals( counts_normal_data[ i ], &gl_array_buffer_data[ offsets_vertex_data[ i ] ] );
      }
      offsets_normal_data[ i ]  = game_state -> array_buffer_target;
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_normal_data[ i ] ];
      void*   src   = ( void* )&normals[ 0 ];
      uint32  bytes = ( sizeof( normals[ 0 ] ) * counts_normal_data[ i ] );
      memcpy( dest, src, bytes );
      free( normals );
      game_state -> array_buffer_target += counts_normal_data[ i ];
    }
    
    { // colours
      real32* colours;
      if( olp -> mesh_source == HM_FLOOR1 ) {
        colours = get_underside_floor_colours( &counts_colour_data[ i ] );
      } else {
        uint32 count = counts_vertex_data[ i ];
        colours = get_overside_floor_colours( counts_colour_data[ i ] );
      }
      offsets_colour_data[ i ]  = game_state -> array_buffer_target;
      void*   dest  = ( void* )&gl_array_buffer_data[ offsets_colour_data[ i ] ];
      void*   src   = ( void* )&colours[ 0 ];
      uint32  bytes = ( sizeof( colours[ 0 ] ) * counts_colour_data[ i ] );
      memcpy( dest, src, bytes );
      free( colours );
      game_state -> array_buffer_target += counts_colour_data[ i ];
    }
    
    {
      uint16* indices;
      if( olp -> mesh_source == HM_FLOOR1 ) {
        indices = get_underside_floor_indices( &counts_index_data[ i ] );
      } else if( olp -> mesh_source == HM_FLOOR2 ) {
        indices = get_overside_floor_indices( &counts_index_data[ i ] );
      }
      offsets_index_data[ i ]  = game_state -> element_array_buffer_target;
      void*   dest  = ( void* )&gl_element_array_buffer_data[ offsets_index_data[ i ] ];
      void*   src   = ( void* )&indices[ 0 ];
      uint32  bytes = ( sizeof( indices[ 0 ] ) * counts_index_data[ i ] );
      memcpy( dest, src, bytes );
      free( indices );
      game_state -> element_array_buffer_target += counts_index_data[ i ];
    }
  } else {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Mesh source not specified\n" );
  }
  
}

void upload_objects_data_to_gl( GameState* game_state ) {
  
  for( uint32 i = 0; i < OBJECT_COUNT; i++ ) {
    
    // vertex data
    {
      gl_offsets_vertex_data[ i ] = game_state -> target_gl_offsets_array_data;
      uint32      target          = GL_ARRAY_BUFFER;
      uint32      offset          = gl_offsets_vertex_data[ i ];
      uint32      size            = ( sizeof( real32 ) * counts_vertex_data[ i ] );
      uint32      array_position  = offsets_vertex_data[ i ];
      const void* data            = ( const void* )&gl_array_buffer_data[ array_position ];
      
      GLCall( glBufferSubData( target, offset, size, data ) );
      game_state -> target_gl_offsets_array_data += size;
    }
    
    // normal data
    {
      gl_offsets_normal_data[ i ] = game_state -> target_gl_offsets_array_data;
      uint32      target          = GL_ARRAY_BUFFER;
      uint32      offset          = gl_offsets_normal_data[ i ];
      uint32      size            = ( sizeof( real32 ) * counts_normal_data[ i ] );
      uint32      array_position  = offsets_normal_data[ i ];
      const void* data            = ( const void* )&gl_array_buffer_data[ array_position ];
      
      GLCall( glBufferSubData( target, offset, size, data ) );
      game_state -> target_gl_offsets_array_data += size;
    }
    
    // tex_coord0 data
    if( shader_types[ i ] == SHADER_LIGHT ) {
      gl_offsets_tex_coord0_data[ i ] = game_state -> target_gl_offsets_array_data;
      uint32      target          = GL_ARRAY_BUFFER;
      uint32      offset          = gl_offsets_tex_coord0_data[ i ];
      uint32      size            = ( sizeof( real32 ) * counts_tex_coord0_data[ i ] );
      uint32      array_position  = offsets_tex_coord0_data[ i ];
      const void* data            = ( const void* )&gl_array_buffer_data[ array_position ];
      
      GLCall( glBufferSubData( target, offset, size, data ) );
      game_state -> target_gl_offsets_array_data += size;
    }
    
    if( shader_types[ i ] == SHADER_VERTEX_COLOURS ) {
      gl_offsets_colour_data[ i ] = game_state -> target_gl_offsets_array_data;
      uint32      target          = GL_ARRAY_BUFFER;
      uint32      offset          = gl_offsets_colour_data[ i ];
      uint32      size            = ( sizeof( real32 ) * counts_colour_data[ i ] );
      uint32      array_position  = offsets_colour_data[ i ];
      const void* data            = ( const void* )&gl_array_buffer_data[ array_position ];
      
      GLCall( glBufferSubData( target, offset, size, data ) );
      game_state -> target_gl_offsets_array_data += size;
    }
    
    // index data
    {
      gl_offsets_index_data[ i ]  = game_state -> target_gl_offsets_index_data;
      uint32      target          = GL_ELEMENT_ARRAY_BUFFER;
      uint32      offset          = gl_offsets_index_data[ i ];
      uint32      size            = ( sizeof( uint16 ) * counts_index_data[ i ] );
      uint32      array_position  = offsets_index_data[ i ];
      const void* data            = ( const void* )&gl_element_array_buffer_data[ array_position ];
      
      GLCall( glBufferSubData( target, offset, size, data ) );
      game_state -> target_gl_offsets_index_data += size;
    }
  }
}

void load_ship_and_target( GameState* game_state ) {
  
  const uint32 ship_index   = 0;
  const uint32 target_index = 1;
  
  ObjectLoadParameters ship;
  ship.make_immediately_active  = true;
  ship.shader_filename          = "shaderLight.glsl";
  ship.gltf_model_filename      = "modelShip.glb";
  ship.initial_position.x       = -2.0f;
  ship.initial_position.y       = 2.0f;
  ship.initial_position.z       = 0.0f;
  
  load_level_object( &ship, ship_index, game_state );
  
  ObjectLoadParameters target;
  target.make_immediately_active  = true;
  target.shader_filename          = "shaderVertexColoursNoLight.glsl";
  target.shader_type              = SHADER_VERTEX_COLOURS_NO_LIGHT;
  target.initial_position.x       = ship.initial_position.x;
  target.initial_position.y       = ship.initial_position.y;
  target.initial_position.z       = ( ship.initial_position.z - 10.0f );
  target.mesh_source              = HM_TARGET;
  
  load_level_object( &target, target_index, game_state );
  
  flat_colours[ target_index ].x = 0.729f;
  flat_colours[ target_index ].y = 0.129f;
  flat_colours[ target_index ].z = 0.176f;
  
}

const uint32 floor1_index = 2;
const uint32 floor2_index = 3;

void load_floor( GameState* game_state ) {
  
  // underside
  ObjectLoadParameters floor1;
  floor1.make_immediately_active  = true;
  floor1.shader_filename          = "shaderVertexColoursNoLight.glsl";
  floor1.shader_type              = SHADER_VERTEX_COLOURS_NO_LIGHT;
  floor1.mesh_source              = HM_FLOOR1;
  floor1.initial_position.x       = -100.0f;
  floor1.initial_position.y       = -0.05f; // avoid z fighting
  floor1.initial_position.z       = -60.0f;
  flat_colours[ floor1_index ].x  = 0.87f;
  flat_colours[ floor1_index ].y  = 0.87f;
  flat_colours[ floor1_index ].z  = 0.623f;
  
  load_level_object( &floor1, floor1_index, game_state );
  
  // overside
  ObjectLoadParameters floor2;
  floor2.make_immediately_active  = true;
  floor2.shader_filename          = "shaderVertexColoursNoLight.glsl";
  floor2.shader_type              = SHADER_VERTEX_COLOURS_NO_LIGHT;
  floor2.mesh_source              = HM_FLOOR2;
  floor2.initial_position.x       = -100.0f;
  floor2.initial_position.y       = 0.0f;
  floor2.initial_position.z       = -60.0f;
  flat_colours[ floor2_index ].x  = 0.87f;
  flat_colours[ floor2_index ].y  = 0.733f;
  flat_colours[ floor2_index ].z  = 0.129f;
  
  load_level_object( &floor2, floor2_index, game_state );
  
  floor_start_z = floor2.initial_position.z;
  
}

struct key_value_pair {
  char*   key;
  char*   value;
  uint32  key_length;
  uint32  value_length;
};

enum yaml_content_type { YAML_NULL, YAML_KEY, YAML_VALUE };
enum yaml_section { YAML_SECTION_NULL, YAML_SECTION_LEVEL_DETAILS, YAML_SECTION_LEVEL_OBJECTS };
enum yaml_section_sub { YAML_SECTION_SUB_NULL, YAML_SECTION_SUB_INIT_POS };

const uint32  char_buffer_size = 100;

struct yaml_line_result {
  char*   key;
  char*   value;
  uint32  key_length;
  uint32  value_length;
  bool32  is_key_value_pair = false;
  bool32  start_new_object  = false;
  bool32  do_nothing        = false;
};

// struct yaml_line_type {
//   bool32 key_value_pair   = false;
//   bool32 array_starter    = false;
//   bool32 object_starter   = false;
//   bool32 anon_array_entry = false;
// };

yaml_line_result process_line( const char* yaml_line, uint32 length, uint32 current_yaml_section ) {
  
  yaml_line_result result;
  
  if( strings_are_equal( yaml_line, "---" ) ) {
    result.do_nothing = true;
    // return result;
  }
  
  bool32  passed_colon  = false;
  uint32  content_type  = YAML_NULL;
  bool32  has_key       = false;
  bool32  has_value     = false;
  uint32  key_length    = 0;
  uint32  value_length  = 0;
  
  char key  [ char_buffer_size ];
  char value[ char_buffer_size ];
  
  uint32  key_index   = 0;
  uint32  value_index = 0;
  
  null_char_buffer( &key[ 0 ]   , char_buffer_size );
  null_char_buffer( &value[ 0 ] , char_buffer_size );
  
  char* line = ( char* )malloc( length + 1 );
  for( uint32 i = 0; i < length + 1; i++ ) {
    line[ i ] = '\0';
  }
  memcpy( line, yaml_line, length );
  
  for( uint32 i = 0; i < length; i++ ) {
    char this_char = line[ i ];
    
    if( this_char == ASCII_COLON ) {
      passed_colon = true;
      content_type  = YAML_NULL;
    } else if( this_char == ASCII_HYPHEN && content_type == YAML_NULL && current_yaml_section == YAML_SECTION_LEVEL_OBJECTS && i == 0 ) {
      result.start_new_object = true;
    } else if( ( this_char >= ASCII_0 && this_char <= ASCII_z ) || this_char == ASCII_DECIMAL_POINT || this_char == ASCII_HYPHEN ) {
      if( passed_colon ) {
        content_type  = YAML_VALUE;
        has_value     = true;
      } else {
        content_type  = YAML_KEY;
        has_key       = true;
      }
    } else {
      content_type = ASCII_NUL;
    }
    
    if( content_type == YAML_KEY ) {
      key[ key_index++ ] = this_char;
      key_length++;
    } else if( content_type == YAML_VALUE ) {
      value[ value_index++ ] = this_char;
      value_length++;
    }
  }
  
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "line %s\n", line );
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "key = %s value = %s\n", key, value );
  
  free( line );
  
  result.key_length   = key_length;
  result.value_length = value_length;
  result.key          = init_char_star( key_length + 1 );
  result.value        = init_char_star( value_length + 1 );
  
  memcpy( result.key, &key[ 0 ], key_length );
  memcpy( result.value, &value[ 0 ], value_length );
  
  if( has_key && has_value ) {
    result.is_key_value_pair = true;
  }
  
  return result;
}

inline uint32 get_shader_type( const char* shader_filename ) {
  uint32 result = 0;
  if( strings_are_equal( shader_filename, "shaderLight.glsl" ) ) {
    return SHADER_LIGHT;
  } else if( strings_are_equal( shader_filename, "shaderVertexColours.glsl") ) {
    return SHADER_VERTEX_COLOURS;
  }
  return result;
}

void load_yaml( const char* filename, GameState* game_state ) {
  
  ReadFileResult        yaml_file = read_entire_file( filename );
  ObjectLoadParameters* olp       = NULL;
  ObjectLoadParameters* to_submit = NULL;
  
  char    this_char;
  uint32  line_start                = 0;
  uint32  line_end                  = 0;
  uint32  current_yaml_section      = 0;
  uint32  current_yaml_section_sub  = 0;
  bool32  submit_olp                = false;
  bool32  is_first_object           = true;
  
  const char* data = ( const char* )yaml_file.contents;
  
  for( uint32 i = 0; i < yaml_file.contents_size; i++ ) {
    
    this_char = data[ i ];
    if( this_char == ASCII_LF ) {
      line_end = i;
      
      uint32 length = ( line_end - line_start );
      char* line = init_char_star( length + 1 );
      memcpy( line, &data[ line_start ], length );
      
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "line %s\n", line );
      
      yaml_line_result yaml_line = process_line( line, length, current_yaml_section );
      
      if( yaml_line.start_new_object ) {
        if( is_first_object ) {
          is_first_object = false;
        } else {
          to_submit = olp;
        }
        olp = ( ObjectLoadParameters* )malloc( sizeof( ObjectLoadParameters ) );
      } else if( strings_are_equal( line, "nullTerminator: 1" ) ) {
        to_submit = olp;
        current_yaml_section_sub = YAML_SECTION_SUB_NULL;
      }
      
      if( strings_are_equal( line, "levelObjects:" ) ) {
          current_yaml_section = YAML_SECTION_LEVEL_OBJECTS;
      } else if( yaml_line.is_key_value_pair && current_yaml_section == YAML_SECTION_LEVEL_OBJECTS ) {
        SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "key is %s - value is %s\n", yaml_line.key, yaml_line.value );
        
        if( strings_are_equal( yaml_line.key, "glbFile" ) ) {
          olp -> gltf_model_filename = init_char_star( yaml_line.value_length + 1 );
          memcpy( olp -> gltf_model_filename, yaml_line.value, yaml_line.value_length );
        } else if( strings_are_equal( yaml_line.key, "shaderFile" ) ) {
          olp -> shader_filename = init_char_star( yaml_line.value_length + 1 );
          memcpy( olp -> shader_filename, yaml_line.value, yaml_line.value_length );
          olp -> shader_type = get_shader_type( ( const char* )olp -> shader_filename );
        } else if( strings_are_equal( yaml_line.key, "objectType" ) ) {
          olp -> object_type = atoi( yaml_line.value );
        } else if( strings_are_equal( yaml_line.key, "x" ) && current_yaml_section_sub == YAML_SECTION_SUB_INIT_POS ) {
          olp -> initial_position.x = atoi( yaml_line.value );
        } else if( strings_are_equal( yaml_line.key, "y" ) && current_yaml_section_sub == YAML_SECTION_SUB_INIT_POS ) {
          olp -> initial_position.y = atoi( yaml_line.value );
        } else if( strings_are_equal( yaml_line.key, "z" ) && current_yaml_section_sub == YAML_SECTION_SUB_INIT_POS ) {
          olp -> initial_position.z = atoi( yaml_line.value );
        }
      } else if( strings_are_equal( yaml_line.key, "initPosition" ) && current_yaml_section == YAML_SECTION_LEVEL_OBJECTS ) {
        current_yaml_section_sub = YAML_SECTION_SUB_INIT_POS;
      }
      
      if( to_submit != NULL ) {
        int32 target_array_position = get_free_object_index();
        if( target_array_position == -1 ) {
          SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "[ ERROR ] : Failed to allocate new object index\n" );
        } else {
          uint32 index = ( uint32 )target_array_position;
          SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Submitting object index %d\n", index );
          to_submit -> make_immediately_active = true;
          to_submit -> shader_type = SHADER_LIGHT;
          load_level_object( to_submit, index, game_state );
          free( to_submit );
          to_submit = NULL;
        }
      }
      
      line_start = i;
      line_start++;
      free( line );
      free( yaml_line.key );
      free( yaml_line.value );
    }
  }
  
  free_memory( yaml_file.contents, yaml_file.contents_size );
}

#endif //LOAD_LEVEL_HPP
