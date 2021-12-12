#include "game.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "json.hpp"
#include "input.hpp"
#include "vector_maths.hpp"
#include "game.hpp"
#include "object_data.hpp"

struct GameState {
  real32  vp_mat[ 16 ]; // view perspective matrix
  GLuint  vbo;
  GLuint  ibo;
  uint32  array_buffer_target         = 0;
  uint32  element_array_buffer_target = 0;
  bool32  invert_y = false;
};

// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;

void initial_setup( GameState* game_state, SDLParams sdl_params ) {
  
  real32 aspect     = ( real32 ) sdl_params.windowWidth / ( real32 ) sdl_params.windowHeight;
  real32 v[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };    // view
  real32 p[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // projection
  real32 fov        = 70.0f;
  real32 near_plane = 0.1f;
  real32 far_plane  = 100.0f;
  populate_perspective_matrix( &p[ 0 ], fov, aspect, near_plane, far_plane );
  
  Position eye      = { 7.0f, 7.0f, 7.0f };
  Position centre   = { 1.0f, 1.0f, 1.0f };
  
  populate_view_matrix( &v[ 0 ], eye, centre );
  
  mat4_multiply( &game_state -> vp_mat[ 0 ], &v[ 0 ], &p[ 0 ] );
  
  GLuint  vbo;
  GLuint  ibo;
  
  // set up gl buffers
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, Megabytes( 50 ), 0, GL_STATIC_DRAW );
  
  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, Megabytes( 50 ), 0, GL_STATIC_DRAW );
  
  game_state -> vbo = vbo;
  game_state -> ibo = ibo;
}

void load_level_objects( GameState* game_state ) {
  
  for( uint32 i = 0; i < object_count; i++ ) {
    
    GameObject game_object;
    game_object.index = i;
    game_object.active = true;
    
    const char* shader_filename = "shaderLight.glsl";
    ReadFileResult shader_file  = read_entire_file( shader_filename );
    
    uint32 shader_program_id    = createShader( shader_file );
    shader_program_ids[ i ]     = shader_program_id;
    glUseProgram( shader_program_id );
    
    int32 position_attribute_location     = glGetAttribLocation ( shader_program_id, "aPosition" );
    int32 normal_attribute_location       = glGetAttribLocation ( shader_program_id, "aNormal" );
    int32 tex_coord0_attribute_location   = glGetAttribLocation ( shader_program_id, "aTexCoord" );
    int32 mvp_uniform_location            = glGetUniformLocation( shader_program_id, "uMVP" );
    int32 model_uniform_location          = glGetUniformLocation( shader_program_id, "uModelMatrix" );
    int32 light_position_uniform_location = glGetUniformLocation( shader_program_id, "uLightPosition" );
    int32 ambient_light_uniform_location  = glGetUniformLocation( shader_program_id, "uAmbientLight" );
    
    gl_id_positions[ i ]        = position_attribute_location;
    gl_id_normals[ i ]          = normal_attribute_location;
    gl_id_tex_coords0[ i ]      = tex_coord0_attribute_location;
    gl_id_mvp_mats[ i ]         = mvp_uniform_location;
    gl_id_light_positions[ i ]  = model_uniform_location;
    gl_id_ambient_lights[ i ]   = ambient_light_uniform_location;
    
    glEnableVertexAttribArray( position_attribute_location );
    glEnableVertexAttribArray( normal_attribute_location );
    glEnableVertexAttribArray( tex_coord0_attribute_location );
    
    ReadFileResult gltf_file;
    if( i == 0 ) {
      const char* gltf_filename = "modelShip.glb";
      gltf_file  = read_entire_file( gltf_filename );
    } else {
      const char* gltf_filename = "modelEnemyPod.glb";
      gltf_file  = read_entire_file( gltf_filename );
    }
    
    uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
    char* json_string         = ( char* )malloc( json_string_length + 1 );
    pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
    
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
    
    counts_vertex_data[ i ]   = count;
    offsets_vertex_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&offsets_vertex_data[ i ];
      void*   src   = ( void* )data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // normals data
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_NORMALS );
    count *= 3; // three values per vertex
    
    counts_normal_data[ i ]   = count;
    offsets_normal_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&offsets_normal_data[ i ];
      void*   src   = ( void* )data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // tex_coord0 data
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_TEX_COORD0 );
    count *= 2; // two values per uv coordinate
    
    counts_tex_coord0_data[ i ]   = count;
    offsets_tex_coord0_data[ i ]  = game_state -> array_buffer_target;
    {
      void*   dest  = ( void* )&offsets_tex_coord0_data[ i ];
      void*   src   = ( void* )data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> array_buffer_target += count;
    
    // indices
    buffer_view_info  = get_glft_buffer_view_info(  target_mesh_index, gltf_contents, json, GLTF_INDICES );
    data              = get_gltf_data_pointer(      target_mesh_index, gltf_contents, json, GLTF_INDICES );
    count             = get_gltf_data_count(        target_mesh_index, gltf_contents, json, GLTF_INDICES );
    
    counts_index_data[ i ]   = count;
    offsets_index_data[ i ]  = game_state -> element_array_buffer_target;
    {
      void*   dest  = ( void* )&offsets_index_data[ i ];
      void*   src   = ( void* )data;
      uint32  bytes = byte_length;
      memcpy( dest, src, bytes );
    }
    game_state -> element_array_buffer_target += count;
    
    free( json_string );
  }
}


int32 run_game() {
  
  SDLParams sdl_params;
  
  ButtonsPressed old_buttons;
  ButtonsPressed new_buttons;
  
  init_sdl( &sdl_params );
  
  GameState game_state;
  game_state.array_buffer_target          = 0;
  game_state.element_array_buffer_target  = 0;
  
  initial_setup( &game_state, sdl_params );
  
  SDL_Window* window = sdl_params.window;
  
  load_level_objects( &game_state );
  
  bool32 running = true;
  
  do {
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      handle_sdl_input_event( &event, &old_buttons, &new_buttons );
    }
    
    GameInput game_input = get_game_input_state( old_buttons, new_buttons, game_state.invert_y );
    
    if( game_input.quit )
      running = false;
    
    SDL_GL_SwapWindow( window );
    
  } while( running );
  
  return 0;
}
