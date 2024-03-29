#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "types.hpp"
#include "input.hpp"
#include "object_data.hpp"

void caculate_camera( GameState* game_state, GameInput* game_input, f32 dt ) {
  
  f32 v[ 16 ]    = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };    // view
  
  populate_view_matrix( &v[ 0 ], game_state -> eye, game_state -> look_at );
  mat4_multiply( &game_state -> vp_mat[ 0 ], &v[ 0 ], &game_state -> p_mat[ 0 ] );
}

#endif //CAMERA_HPP
