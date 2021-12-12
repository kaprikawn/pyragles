#include "game.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "json.hpp"
#include "input.hpp"
#include "vector_maths.hpp"
#include "game.hpp"

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


int32 run_game() {
  
  SDLParams sdl_params;
  
  ButtonsPressed old_buttons;
  ButtonsPressed new_buttons;
  
  init_sdl( &sdl_params );
  
  GameState game_state;
  
  initial_setup( &game_state, sdl_params );
  
  SDL_Window* window = sdl_params.window;
  
  bool32 running = true;
  
  
  
  return 0;
}
