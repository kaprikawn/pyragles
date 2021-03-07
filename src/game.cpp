#include "game.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "json.hpp"
#include "input.hpp"

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

struct GameObject {
  glm::vec4 position        = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
  glm::mat4 model_matrix    = glm::mat4( 1.0f );
  glm::mat4 mvp             = glm::mat4( 1.0f );
  glm::mat4 rotation_matrix = glm::mat4( 1.0f );
  real32    rotation_x      = 0.0f;
  real32    rotation_y      = 0.0f;
  real32    rotation_z      = 0.0f;
  int32     position_id;
  int32     tex_coord0_id;
  uint32    mesh_count;
  uint32    tbo;
  MeshData* mesh_data;
  uint32*   indices;
  uint32    shader_program_id;
  int32     mvp_id;
};

uint32 vertex_buffer_current_offset = 0;
uint32 index_buffer_current_offset  = 0;

// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;
    
void load_game_object( GameObject* game_object, const char* model_filename, const char* shader_filename ) {
  
  ReadFileResult shader_file = read_entire_file( shader_filename );
  
  game_object -> shader_program_id = createShader( ( const char* )shader_file.contents, shader_file.contentsSize );
  free_memory( shader_file.contents, shader_file.contentsSize );
  
  ReadFileResult gltf_file = read_entire_file( model_filename );
  uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
  
  char* json_string = ( char* )malloc( json_string_length + 1 );
  
  pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "json is %s\n", json_string );
  game_object -> mesh_count = count_meshes( json_string, json_string_length );
  game_object -> mesh_data = ( MeshData* )malloc( sizeof( MeshData ) * game_object -> mesh_count );
  
  for( uint32 i = 0; i < game_object -> mesh_count; i++ ) {
    
    MeshData this_mesh = populate_mesh_data( i, json_string, &gltf_file );
    uint32 index_count = this_mesh.index_count;
    
    this_mesh.index_data = ( uint32* )malloc( sizeof( uint32 ) * index_count );
    
    // TODO : this is broken
    uint16* src   = this_mesh.index_data_raw;
    uint32* dest  = this_mesh.index_data;
    for( uint32 j = 0; j < index_count; j++ ) {
      uint16 this_index = *src;
      uint32 index_value = ( uint32 )this_index;
      *dest = index_value;
      src++;
      dest++;
    }
    this_mesh.index_data_in_bytes = sizeof( this_mesh.index_data[ 0 ] ) * this_mesh.index_count;
    
    // determine where to copy data into - if we're on the second mesh, move the pointer along by one etc.
    MeshData* mesh_dest = game_object -> mesh_data;
    for( uint32 j = 0; j < i; j++ ) {
      mesh_dest++;
    }
    
    this_mesh.gl_vertex_offset      = vertex_buffer_current_offset;
    vertex_buffer_current_offset   += this_mesh.vertex_data_in_bytes;
    this_mesh.gl_tex_coord0_offset  = vertex_buffer_current_offset;
    vertex_buffer_current_offset   += this_mesh.tex_coord0_data_in_bytes;
    this_mesh.gl_index_offset       = index_buffer_current_offset;
    index_buffer_current_offset    += this_mesh.index_data_in_bytes;
    
    // upload data to GL
    glBufferSubData( GL_ARRAY_BUFFER, this_mesh.gl_vertex_offset, this_mesh.vertex_data_in_bytes, this_mesh.vertex_data );
    glBufferSubData( GL_ARRAY_BUFFER, this_mesh.gl_tex_coord0_offset, this_mesh.tex_coord0_data_in_bytes, this_mesh.tex_coord0_data );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, this_mesh.gl_index_offset, this_mesh.index_data_in_bytes, this_mesh.index_data );
    
    // copy data into object
    memcpy( mesh_dest, &this_mesh, sizeof( MeshData ) );
    
  }
  
  int32 texture_width, texture_height, texture_bpp;
  
  uchar* texture_data = stbi_load_from_memory( ( const uchar* )game_object -> mesh_data[ 0 ].image_data, game_object -> mesh_data[ 0 ].image_data_in_bytes, &texture_width, &texture_height, &texture_bpp, 4 );
  
  glGenTextures( 1, &game_object -> tbo );
  glBindTexture( GL_TEXTURE_2D, game_object -> tbo );
  
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data );
  glBindTexture( GL_TEXTURE_2D, game_object -> tbo );
  
  game_object -> position_id   = glGetAttribLocation( game_object -> shader_program_id,  "aPosition" );
  game_object -> tex_coord0_id = glGetAttribLocation( game_object -> shader_program_id,  "aTexCoord" );
  
  game_object -> mvp_id = glGetUniformLocation( game_object -> shader_program_id, "uMVP" );
  
  glEnableVertexAttribArray( game_object -> position_id );
  glEnableVertexAttribArray( game_object -> tex_coord0_id );
  
  glVertexAttribPointer( game_object -> position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_object -> mesh_data[ 0 ].gl_vertex_offset );
  glVertexAttribPointer( game_object -> tex_coord0_id , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_object -> mesh_data[ 0 ].gl_tex_coord0_offset );
}

inline real32 lerp( real32 src, real32 dest, real32 alpha ) {
  return ( ( src * ( 1 - alpha ) ) + ( dest * alpha ) );
}

inline real32 lerp_dt( real32 src, real32 dest, real32 smoothing, real32 dt ) {
  return lerp( src, dest, 1 - pow( smoothing, dt ) );
}

uint32 init_game( game_memory* memory ) {
  
  SDLObjects sdlObjects;
  
  Buttons_pressed old_buttons;
  Buttons_pressed new_buttons;
  
  init_sdl( &sdlObjects );
  
  SDL_Window* window = sdlObjects.window;
  
  glm::mat4 projection_matrix;
  glm::mat4 view_matrix;
  
  real32  aspect = ( real32 ) sdlObjects.windowWidth / ( real32 ) sdlObjects.windowHeight;
  
  uint32  vbo;
  uint32  ibo;
  
  initialise_gamepads();
  
  const uint32 object_count = 2;
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
      
      GameObject ship;
      load_game_object( &game_objects[ 0 ], "modelShip.glb"    , "shaderBasic.glsl" );
      load_game_object( &game_objects[ 1 ], "modelEnemyPod.glb", "shaderBasic.glsl" );
      
      game_objects[ 0 ].position.x -= 2.0f;
      game_objects[ 1 ].position.x += 2.0f;
      
      projection_matrix = glm::perspective( glm::radians( 70.0f ), aspect, 0.1f, 100.0f );
      
      view_matrix = glm::lookAt(
          glm::vec3( 4, 3, 3 )
        , glm::vec3( 0, 0, 0 )
        , glm::vec3( 0, 1, 0 )
      );
      
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
    
    Game_input game_input = get_game_input_state( old_buttons, new_buttons, invert_y );
    
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "x is %f\n", game_input.joy_axis_x );
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "y is %f\n", game_input.joy_axis_y );
        
    if( game_input.quit )
      running = false;
    
    int32 mvpID;
    
    // update and render
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // if( game_input.a_held ) {
    //   game_objects[ 0 ].rotation_y += 0.5f;
    // } else if( game_input.d_held ) {
    //   game_objects[ 0 ].rotation_y -= 0.5f;
    // }
    
    // if( game_input.w_held ) {
    //   game_objects[ 0 ].rotation_x += 0.5f;
    // } else if( game_input.s_held ) {
    //   game_objects[ 0 ].rotation_x -= 0.5f;
    // }
    
    // game_objects[ 0 ].rotation_y -= game_input.joy_axis_x;
    // game_objects[ 0 ].rotation_x -= game_input.joy_axis_y;
    
    real32 max_rotation_x = 30.0f; // up/down pitch
    real32 current_x      = game_objects[ 0 ].rotation_x;
    real32 joy_y          = game_input.joy_axis_y;
    real32 destination_x;
    real32 lerp_x;
    real32 lerp_smoothing = 0.05f;
    
    if( current_x > max_rotation_x )
      current_x -= 360.0f;
    if( current_x < -max_rotation_x )
      current_x += 360.0f;
    
    
    if( joy_y < 0.0f ) { // up on the stick
      destination_x = -( max_rotation_x * joy_y );
      lerp_x        = lerp_dt( current_x, destination_x, lerp_smoothing, dt );
    } else if( joy_y > 0.0f ) { // down on the stick
      //max_rotation_x = -max_rotation_x;
      destination_x = ( -max_rotation_x * joy_y );
      lerp_x        = lerp_dt( current_x, destination_x, lerp_smoothing, dt );
    } else {
      destination_x = 0.0f;
      lerp_x        = lerp_dt( current_x, destination_x, lerp_smoothing, dt );
    }
    
    if( joy_y != 0.0f )
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy_y = %f, max = %f, dest = %f, lerp = %f, cur = %f\n", joy_y, max_rotation_x, destination_x, lerp_x, current_x );
    
    
    game_objects[ 0 ].rotation_x = lerp_x;
    
    
    
    game_objects[ 1 ].rotation_y -= 0.8f;
    
    for( uint32 i = 0; i < object_count; i++ ) {
      
      if( game_objects[ i ].rotation_x > 360.0f ) game_objects[ i ].rotation_x -= 360.0f;
      if( game_objects[ i ].rotation_y > 360.0f ) game_objects[ i ].rotation_y -= 360.0f;
      if( game_objects[ i ].rotation_z > 360.0f ) game_objects[ i ].rotation_z -= 360.0f;
      if( game_objects[ i ].rotation_x < 0.0f )   game_objects[ i ].rotation_x += 360.0f;
      if( game_objects[ i ].rotation_y < 0.0f )   game_objects[ i ].rotation_y += 360.0f;
      if( game_objects[ i ].rotation_z < 0.0f )   game_objects[ i ].rotation_z += 360.0f;
      
      game_objects[ i ].rotation_matrix = glm::rotate( glm::mat4( 1.0f )   , glm::radians( game_objects[ i ].rotation_x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
      game_objects[ i ].rotation_matrix = glm::rotate( game_objects[ i ].rotation_matrix, glm::radians( game_objects[ i ].rotation_y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      game_objects[ i ].rotation_matrix = glm::rotate( game_objects[ i ].rotation_matrix, glm::radians( game_objects[ i ].rotation_z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
      
      game_objects[ i ].model_matrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( game_objects[ i ].position ) );
      game_objects[ i ].model_matrix *= game_objects[ i ].rotation_matrix;
    
      game_objects[ i ].mvp = projection_matrix * view_matrix * game_objects[ i ].model_matrix;
      
      glUseProgram( game_objects[ i ].shader_program_id );
      glUniformMatrix4fv( game_objects[ i ].mvp_id, 1, GL_FALSE, &game_objects[ i ].mvp[0][0] );
      
      glVertexAttribPointer( game_objects[ i ].position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_objects[ i ].mesh_data[ 0 ].gl_vertex_offset );
      glVertexAttribPointer( game_objects[ i ].tex_coord0_id , 2, GL_FLOAT, GL_FALSE, 0, ( void* )game_objects[ i ].mesh_data[ 0 ].gl_tex_coord0_offset );
      
      glBindTexture( GL_TEXTURE_2D, game_objects[ i ].tbo );
      
      glDrawElements( GL_TRIANGLES, game_objects[ i ].mesh_data[ 0 ].index_count, GL_UNSIGNED_INT, ( void* )game_objects[ i ].mesh_data[ 0 ].gl_index_offset );
      
    }
    
    uint64 before_frame_flip_ticks = SDL_GetTicks();
    
    SDL_GL_SwapWindow( window );
    
    uint64 end_frame_ticks = SDL_GetTicks();
    
  } while( running );
  
  return 0;
}
