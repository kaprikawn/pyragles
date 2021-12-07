#include "game.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "json.hpp"
#include "input.hpp"
#include "vector_maths.hpp"
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
  real32    mvp[ 16 ];
  Position  position;
  real32    rotation_x      = 0.0f;
  real32    rotation_y      = 0.0f;
  real32    rotation_z      = 0.0f;
  uint32    shader_program_id;
  int32     gl_id_position  = -9;
  // int32     gl_id_normal;
  // int32     gl_id_tex_coord0;
  int32     gl_id_mvp_mat   = -9;
  // int32     gl_id_model_mat;
  // int32     gl_id_model_view_mat;
  // int32     gl_id_light_position;
  // int32     gl_id_ambient_light;
  uint32    offset_vertex_data;
  uint32    offset_index_data;
  uint32    count_vertex_data;
  uint32    count_index_data;
};

// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;

real32 gl_array_buffer_data         [ 100000 ];
uint32 gl_element_array_buffer_data [ 10000 ];

uint32 gl_array_buffer_data_last_position         = 0;
uint32 gl_element_array_buffer_data_last_position = 0;

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
  
  const char* gltf_contents = ( char* )gltf_file.contents;
  uint32      target_mesh_index = 0; // @TODO: Hardcoding for the initial mesh for now
  void*       data;
  uint32      byte_length;
  uint32      count;
  
  GltfbufferViewInfo buffer_view_info;
  // vertices
  buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json, GLTF_VERTICES );
  data              = get_gltf_data_pointer( target_mesh_index, gltf_contents, json, GLTF_VERTICES );
  count             = get_gltf_data_count( target_mesh_index, gltf_contents, json, GLTF_VERTICES );
  byte_length       = buffer_view_info.byte_length;
  game_object -> offset_vertex_data = log_array_buffer_data( ( real32* )data, count );
  game_object -> count_vertex_data  = get_count_from_type( count, buffer_view_info.type );
  glBufferSubData( GL_ARRAY_BUFFER, game_object -> offset_vertex_data, byte_length, data );
  // indices
  buffer_view_info  = get_glft_buffer_view_info( target_mesh_index, gltf_contents, json, GLTF_INDICES );
  data              = get_gltf_data_pointer( target_mesh_index, gltf_contents, json, GLTF_INDICES );
  count             = get_gltf_data_count( target_mesh_index, gltf_contents, json, GLTF_INDICES );
  byte_length       = buffer_view_info.byte_length;
  game_object -> offset_index_data  = log_element_array_buffer_data( ( uint16* )data, count );
  game_object -> count_index_data   = get_count_from_type( count, buffer_view_info.type );
  glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, game_object -> offset_index_data, byte_length, data );
  
}

uint32 init_game( game_memory* memory ) {
  
  SDLObjects sdlObjects;
  
  Buttons_pressed old_buttons;
  Buttons_pressed new_buttons;
  
  init_sdl( &sdlObjects );
  
  SDL_Window* window = sdlObjects.window;
  
  real32  aspect = ( real32 ) sdlObjects.windowWidth / ( real32 ) sdlObjects.windowHeight;
  
  real32 vp[ 16 ] = {}; // view projection matrix
  real32 v[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
  uint32  vbo;
  uint32  ibo;
  
  initialise_gamepads();
  
  const uint32 object_count = 1;
  GameObject game_objects[ object_count ];
  
  GameObject game_object = {};
  game_object.position.z = -5.0f;
  
  bool32  running = true;
  
  real32* vertex_data_postion;
  uint32* index_data_position;
  int32   program_id;
  int32 pos_id = -9;
  int32 mvp_id = -9;
  
  do { // start main loop
    
    if( !memory -> isInitialized ) { // on first run
      
      // projection matrix
      real32 p[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
      real32 fov        = 70.0f;
      real32 near_plane = 0.1f;
      real32 far_plane  = 100.0f;
      populate_perspective_matrix( &p[ 0 ], fov, aspect, near_plane, far_plane );
      
      Position eye      = { 7.0f, 7.0f, 7.0f };
      Position centre   = { 1.0f, 1.0f, 1.0f };
      
      populate_view_matrix( &v[ 0 ], eye, centre );
      
      mat4_multiply( &vp[ 0 ], &v[ 0 ], &p[ 0 ] );
      
      real32 vertices[ 24 ] = {
          -1.0f,  1.0f,  1.0f // front top left
        , -1.0f, -1.0f,  1.0f // front bottom left
        ,  1.0f,  1.0f,  1.0f // front top right
        ,  1.0f, -1.0f,  1.0f // front bottom right
        ,  1.0f,  1.0f, -1.0f // back top right
        ,  1.0f, -1.0f, -1.0f // back bottom right
        , -1.0f,  1.0f, -1.0f // back top left
        , -1.0f, -1.0f, -1.0f // back bottom left
      };
      
      vertex_data_postion = &vertices[ 0 ];
      
      glGenBuffers( 1, &vbo );
      glBindBuffer( GL_ARRAY_BUFFER, vbo );
      glBufferData( GL_ARRAY_BUFFER, 24 * sizeof( &vertices[ 0 ] ), &vertices[ 0 ], GL_STATIC_DRAW );
      
      uint32 indices[ 36 ] = {
          0, 1, 2 // front A
        , 2, 1, 3 // front B
        , 4, 2, 3 // right A
        , 4, 3, 5 // right B
        , 4, 5, 6 // back A
        , 6, 5, 7 // back B
        , 7, 0, 6 // left A
        , 0, 7, 1 // left B
        , 5, 3, 7 // bottom A
        , 7, 3, 0 // bottom B
        , 2, 4, 0 // top A
        , 0, 4, 6 // top B
      };
      
      index_data_position = &indices[ 0 ];
      
      glGenBuffers( 1, &ibo );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof( &indices[ 0 ] ), &indices[ 0 ], GL_STATIC_DRAW );
      
      ReadFileResult shader_file = read_entire_file( "shaderDebug.glsl" );
      program_id = createShader( ( const char* )shader_file.contents, shader_file.contentsSize );
      glUseProgram( program_id );
      free_memory( shader_file.contents, shader_file.contentsSize );
      
      pos_id  = glGetAttribLocation( program_id, "aPosition" );
      mvp_id  = glGetUniformLocation( program_id, "uMVP" );
      
      if( pos_id >= 0 ) {
        glEnableVertexAttribArray( pos_id );
      } else {
        SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Error - Failed to get pos_id\n" );
      }
      
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
    
    // update and render
    real32 model_matrix[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    // translate( &model_matrix[ 0 ], game_object.position );
    real32 mvp[ 16 ]  = {}; // model view projection matrix
    mat4_multiply( &mvp[ 0 ], &model_matrix[ 0 ], &vp[ 0 ] );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( program_id );
    glUniformMatrix4fv( mvp_id, 1, GL_FALSE, &mvp[ 0 ] );
    
    glVertexAttribPointer( pos_id, 3, GL_FLOAT, GL_FALSE, 0, ( void* )0 );
    
    glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, ( void* )0 );
    
    uint64 before_frame_flip_ticks = SDL_GetTicks();
    
    SDL_GL_SwapWindow( window );
    
    uint64 end_frame_ticks = SDL_GetTicks();
    
  } while( running );
  
  return 0;
}
