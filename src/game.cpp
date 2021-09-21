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
  MeshData* mesh_data;
  MeshData* collider_mesh_data;
  uint32*   indices;
  uint32    shader_program_id;
  int32     mvp_id;
  int32     model_id;
  int32     model_view_id;
  int32     light_position_id;
};

uint32 vertex_buffer_current_offset = 0;
uint32 index_buffer_current_offset  = 0;

// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;

    
void load_game_object( GameObject* game_object, const char* model_filename, const char* shader_filename, real32 scale = 1.0f ) {
  
  ReadFileResult shader_file = read_entire_file( shader_filename );
  
  game_object -> shader_program_id = createShader( ( const char* )shader_file.contents, shader_file.contentsSize );
  free_memory( shader_file.contents, shader_file.contentsSize );
  
  ReadFileResult gltf_file = read_entire_file( model_filename );
  uint32 json_string_length = json_size_in_bytes( &gltf_file ); // including padding
  
  char* json_string = ( char* )malloc( json_string_length + 1 );
  
  pull_out_json_string( &gltf_file, json_string, json_string_length ); // loads json_string with the json from the file
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "json is %s\n", json_string );
  game_object -> mesh_count = count_meshes( json_string, json_string_length );
  
  uint32 collider_count = 0; // TODO : replace by actually reading the json
  
  for( uint32 i = 0; i < game_object -> mesh_count; i++ ) {
    const uint32 char_buffer_size = 100;
    char mesh_name[ char_buffer_size ];
    for( uint32 j = 0; j < char_buffer_size; j++ ) {
      mesh_name[ j ] = '\0';
    }
    populate_mesh_name( i, json_string, json_string_length, mesh_name );
    if( strings_are_equal( mesh_name, "Collider" ) ) {
      collider_count++;
    }
  }
  
  game_object -> mesh_data = ( MeshData* )malloc( sizeof( MeshData ) * ( game_object -> mesh_count - collider_count ) );
  if( collider_count > 0 ) {
    game_object -> collider_mesh_data = ( MeshData* )malloc( sizeof( MeshData ) * collider_count );
  }
  
  for( uint32 i = 0; i < game_object -> mesh_count; i++ ) {
    
    MeshData this_mesh = populate_mesh_data( i, json_string, &gltf_file, scale );
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
    this_mesh.gl_normal_offset      = vertex_buffer_current_offset;
    vertex_buffer_current_offset   += this_mesh.normal_data_in_bytes;
    this_mesh.gl_tex_coord0_offset  = vertex_buffer_current_offset;
    vertex_buffer_current_offset   += this_mesh.tex_coord0_data_in_bytes;
    this_mesh.gl_tex_coord0_offset  = vertex_buffer_current_offset;
    vertex_buffer_current_offset   += this_mesh.tex_coord0_data_in_bytes;
    this_mesh.gl_index_offset       = index_buffer_current_offset;
    index_buffer_current_offset    += this_mesh.index_data_in_bytes;
    
    if( this_mesh.is_collider ) {
      mesh_dest = game_object -> collider_mesh_data;
    } else {
      // upload data to GL
      glBufferSubData( GL_ARRAY_BUFFER, this_mesh.gl_vertex_offset, this_mesh.vertex_data_in_bytes, this_mesh.vertex_data );
      glBufferSubData( GL_ARRAY_BUFFER, this_mesh.gl_normal_offset, this_mesh.normal_data_in_bytes, this_mesh.normal_data );
      glBufferSubData( GL_ARRAY_BUFFER, this_mesh.gl_tex_coord0_offset, this_mesh.tex_coord0_data_in_bytes, this_mesh.tex_coord0_data );
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, this_mesh.gl_index_offset, this_mesh.index_data_in_bytes, this_mesh.index_data );
    }
    
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
  
  game_object -> position_id        = glGetAttribLocation( game_object -> shader_program_id,  "aPosition" );
  game_object -> normal_id          = glGetAttribLocation( game_object -> shader_program_id,  "aNormal" );
  game_object -> tex_coord0_id      = glGetAttribLocation( game_object -> shader_program_id,  "aTexCoord" );
  game_object -> mvp_id             = glGetUniformLocation( game_object -> shader_program_id, "uMVP" );
  game_object -> model_view_id      = glGetUniformLocation( game_object -> shader_program_id, "uModelViewMatrix" );
  game_object -> light_position_id  = glGetUniformLocation( game_object -> shader_program_id, "uLightPosition" );
  
  glEnableVertexAttribArray( game_object -> position_id );
  glEnableVertexAttribArray( game_object -> normal_id );
  glEnableVertexAttribArray( game_object -> tex_coord0_id );
  
  glVertexAttribPointer( game_object -> position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_object -> mesh_data[ 0 ].gl_vertex_offset );
  glVertexAttribPointer( game_object -> normal_id     , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_object -> mesh_data[ 0 ].gl_normal_offset );
  glVertexAttribPointer( game_object -> tex_coord0_id , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_object -> mesh_data[ 0 ].gl_tex_coord0_offset );
}

inline real32 lerp( real32 src, real32 dest, real32 alpha ) {
  return ( ( src * ( 1 - alpha ) ) + ( dest * alpha ) );
}

inline real32 lerp_dt( real32 src, real32 dest, real32 smoothing, real32 dt ) {
  return lerp( src, dest, 1 - pow( smoothing, dt ) );
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

//                                      view         proj

void dump_mat4( real32* mat ) {
  
  for( uint32 i = 0; i < 15; i++ ) {
    real32 this_float = *mat;
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "float %d is %f\n", i, this_float );
    mat++;
  }
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "#############################################\n" );
}

inline void translate( real32* matrix, Position position ) {
  
  // real32 x = position.x;
  // real32 y = position.y;
  // real32 z = position.z;
  
  // real32* matrix_pos = matrix;
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
  
  // Mat4 view_matrix;
  // Mat4 projection_matrix;
  
  // real32 view_matrix[ 16 ]        = { 0.59f, -4.11f, 0.68f, 0.0f, 0.0f, 0.86f, 0.51f, 0.0f, -0.8f, -0.31f, 0.51f, 0.0f, 0.0f, 0.0f, -5.8f, 1.0f };
  // real32 projection_matrix[ 16 ]  = { 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.43f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -0.2f, 0.0f };
  
  real32 light_position[ 3 ] = { 0.0f, 5.0f, 0.0f };
  
  real32  aspect = ( real32 ) sdlObjects.windowWidth / ( real32 ) sdlObjects.windowHeight;
  
  real32 vp[ 16 ] = {}; // view projection matrix
  real32 v[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
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
      load_game_object( &game_objects[ 0 ], "modelPlane2.glb"   , "shaderLight.glsl", 1.0f );
      load_game_object( &game_objects[ 1 ], "modelEnemyPod.glb" , "shaderLight.glsl" );
      
      game_objects[ 0 ].position.x -= 2.0f;
      game_objects[ 1 ].position.x += 2.0f;
      game_objects[ 1 ].position.z -= 5.0f;
      game_objects[ 1 ].position.y += 5.0f;
      
      // projection matrix
      real32 p[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
      real32 fov = 70.0f;
      real32 near_plane = 0.1f;
      real32 far_plane = 100.0f;
      populate_perspective_matrix( &p[ 0 ], fov, aspect, near_plane, far_plane );
      
      Position eye = { 20.0f, 15.0f, 12.0f };
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
    
    game_objects[ 1 ].rotation_x += 0.02f;
    game_objects[ 1 ].rotation_y += 2.0f;
    // game_objects[ 1 ].rotation_z += 6.0f;
    
    for( uint32 i = 0; i < object_count; i++ ) {
      
      if( game_objects[ i ].rotation_x > 360.0f ) game_objects[ i ].rotation_x -= 360.0f;
      if( game_objects[ i ].rotation_y > 360.0f ) game_objects[ i ].rotation_y -= 360.0f;
      if( game_objects[ i ].rotation_z > 360.0f ) game_objects[ i ].rotation_z -= 360.0f;
      if( game_objects[ i ].rotation_x < 0.0f )   game_objects[ i ].rotation_x += 360.0f;
      if( game_objects[ i ].rotation_y < 0.0f )   game_objects[ i ].rotation_y += 360.0f;
      if( game_objects[ i ].rotation_z < 0.0f )   game_objects[ i ].rotation_z += 360.0f;
      
      real32 model_matrix[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_x[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_y[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_z[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      
      translate( &model_matrix[ 0 ], game_objects[ i ].position );
      rotate_x( &rot_mat_x[ 0 ], game_objects[ i ].rotation_x );
      rotate_y( &rot_mat_y[ 0 ], game_objects[ i ].rotation_y );
      rotate_z( &rot_mat_z[ 0 ], game_objects[ i ].rotation_z );
      
      real32 rot_mat_xy[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      real32 rot_mat_xyz[ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
      mat4_multiply( &rot_mat_xy[ 0 ], &rot_mat_y[ 0 ], &rot_mat_x[ 0 ] );
      mat4_multiply( &rot_mat_xyz[ 0 ], &rot_mat_z[ 0 ], &rot_mat_xy[ 0 ] );
      
      mat4_multiply( &model_matrix[ 0 ], &rot_mat_xyz[ 0 ], &model_matrix[ 0 ] );
      
      // real32 m[ 16 ] = game_objects[ i ].model_matrix;
      // real32 v[ 16 ] = view_matrix;
      // real32 p[ 16 ] = projection_matrix;
      // real32 vp[ 16 ] = p * v;
      // real32 mvp[ 16 ] = vp * m;
    
      real32 mvp[ 16 ]  = {}; // model view projection matrix
      real32 mv[ 16 ]   = {}; // model view matrix
      
      // multiply model matrix with the view-projection matrix to get the mvp matrix
      // mat4_multiply( &game_objects[ i ].mvp[ 0 ], &model_matrix[ 0 ], &view_projection_matrix[ 0 ] );
      mat4_multiply( &mvp[ 0 ], &model_matrix[ 0 ], &vp[ 0 ] );
      mat4_multiply( &mv[ 0 ] , &model_matrix[ 0 ], &v[ 0 ] );
      
      glUseProgram( game_objects[ i ].shader_program_id );
      glUniformMatrix4fv( game_objects[ i ].mvp_id        , 1, GL_FALSE, &mvp[0] );
      glUniformMatrix4fv( game_objects[ i ].model_view_id , 1, GL_FALSE, &mv[0] );
      glUniform3f( game_objects[ i ].light_position_id, light_position[ 0 ], light_position[ 1 ], light_position[ 2 ] );
      //glUniformMatrix4fv( game_objects[ i ].model_id, 1, GL_FALSE, &model_matrix[0] );
      // glUniformMatrix4fv( game_objects[ i ].mvp_id, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );
      
      glVertexAttribPointer( game_objects[ i ].position_id   , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_objects[ i ].mesh_data[ 0 ].gl_vertex_offset );
      glVertexAttribPointer( game_objects[ i ].normal_id     , 3, GL_FLOAT, GL_FALSE, 0, ( void* )game_objects[ i ].mesh_data[ 0 ].gl_normal_offset );
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
