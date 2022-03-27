#ifndef LOAD_LEVEL_HPP
#define LOAD_LEVEL_HPP

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
  
  positions[ i ].x = olp.initial_position.x;
  positions[ i ].y = olp.initial_position.y;
  positions[ i ].z = olp.initial_position.z;
  
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
  
  if( olp.mesh_source == LOAD_MESH_FROM_GLTF ) {
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
  } else if( olp.mesh_source == HM_TARGET ) {
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
  } else if( olp.mesh_source == HM_FLOOR1 || olp.mesh_source == HM_FLOOR2 ) {
    
    {
      real32* vertices;
      if( olp.mesh_source == HM_FLOOR1 ) {
        vertices = get_underside_floor_vertices( &counts_vertex_data[ i ] );
      } else if( olp.mesh_source == HM_FLOOR2 ) {
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
      if( olp.mesh_source == HM_FLOOR1 ) {
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
      if( olp.mesh_source == HM_FLOOR1 ) {
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
      if( olp.mesh_source == HM_FLOOR1 ) {
        indices = get_underside_floor_indices( &counts_index_data[ i ] );
      } else if( olp.mesh_source == HM_FLOOR2 ) {
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
  
  load_level_object( ship, ship_index, game_state );
  
  ObjectLoadParameters target;
  target.make_immediately_active  = true;
  target.shader_filename          = "shaderVertexColoursNoLight.glsl";
  target.shader_type              = SHADER_VERTEX_COLOURS_NO_LIGHT;
  target.initial_position.x       = ship.initial_position.x;
  target.initial_position.y       = ship.initial_position.y;
  target.initial_position.z       = ( ship.initial_position.z - 10.0f );
  target.mesh_source              = HM_TARGET;
  
  load_level_object( target, target_index, game_state );
  
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
  
  load_level_object( floor1, floor1_index, game_state );
  
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
  
  load_level_object( floor2, floor2_index, game_state );
  
  floor_start_z = floor2.initial_position.z;
  
}

#endif //LOAD_LEVEL_HPP