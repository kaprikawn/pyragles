#include "game.hpp"
#include "json.hpp"
#include "input.hpp"
#include "vector_maths.hpp"
#include "game.hpp"
#include "object_data.hpp"
#include "hardcoded_meshes.hpp"
#include "ship.hpp"
#include "camera.hpp"
#include "load_level.hpp"
#include "floor.hpp"

// for delta time ( dt )
uint32 current_time, previous_time, before_frame_flip_time;

void initial_setup( GameState* game_state, SDLParams sdl_params ) {
  
  real32 aspect     = ( real32 ) sdl_params.windowWidth / ( real32 ) sdl_params.windowHeight;
  real32 v[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };    // view
  real32 p[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // projection
  real32 fov        = 70.0f;
  real32 near_plane = 0.01f;
  real32 far_plane  = 65.0f;
  populate_perspective_matrix( &p[ 0 ], fov, aspect, near_plane, far_plane );
  memcpy( &game_state -> p_mat[ 0 ], &p[ 0 ], sizeof( p[ 0 ] ) * 16 );
  
  populate_view_matrix( &v[ 0 ], game_state -> eye, game_state -> look_at );
  
  mat4_multiply( &game_state -> vp_mat[ 0 ], &v[ 0 ], &p[ 0 ] );
  
  GLuint  vbo;
  GLuint  ibo;
  
  // set up gl buffers
  GLCall( glGenBuffers( 1, &vbo ) );
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, vbo ) );
  GLCall( glBufferData( GL_ARRAY_BUFFER, Megabytes( 10 ), 0, GL_STATIC_DRAW ) );
  GLCall( glGenBuffers( 1, &ibo ) );
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo ) );
  GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, Megabytes( 10 ), 0, GL_STATIC_DRAW ) );
  
  game_state -> vbo = vbo;
  game_state -> ibo = ibo;
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
  initialise_gamepads();
  
  reset_data();
  load_ship_and_target( &game_state );
  load_floor( &game_state );
  
  upload_objects_data_to_gl( &game_state );
  
  SDL_Window* window          = sdl_params.window;
  
  bool32 running = true;
  
  do {
    
    previous_time = current_time;
    current_time = SDL_GetTicks();
    uint32 ms_since_last_frame  = current_time - previous_time;
    real32 dt = ( real32 )ms_since_last_frame / 1000.0f;
    if( dt > 0.15f ) dt = 0.15f; // prevent weird outliers e.g. first frame
    
    reset_game_inputs_pressed( &old_buttons, &new_buttons );
    
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      handle_sdl_input_event( &event, &old_buttons, &new_buttons );
    }
    
    GameInput game_input = get_game_input_state( old_buttons, new_buttons, game_state.invert_y );
    
    if( game_input.quit )
      running = false;
    
    calculate_ship_rotation( &game_input, dt );
    update_ship_position( &game_input, dt );
    update_target_position();
    caculate_camera( &game_state, &game_input, dt );
    scroll_floor( floor2_index, dt );
    
    GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
    
    for( uint32 i = 0; i < OBJECT_COUNT; i++ ) {
      render_object( i, &game_state.vp_mat[ 0 ] );
    }
    
    SDL_GL_SwapWindow( window );
    
  } while( running );
  
  return 0;
}
