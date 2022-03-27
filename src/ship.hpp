#ifndef SHIP_HPP
#define SHIP_HPP

#include "vector_maths.hpp"
#include "input.hpp"

void calculate_ship_rotation( GameInput* game_input, real32 dt, uint32 ship_index = 0 ) {
  
  uint32 i = ship_index;
  
  // ###### X - up/down pitch
  real32 max_rotation_x = 30.0f;
  real32 current_x      = rotations[ i ].x;
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
  
  real32 new_rotation_x = lerp_dt( current_x, target_x, lerp_smoothing, dt );
  rotations[ i ].x      = new_rotation_x;
  
  // ###### Y - turn
  real32 max_rotation_y = 30.0f;
  real32 current_y      = rotations[ i ].y;
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
  rotations[ i ].y      = new_rotation_y;
  
  // ###### Z - tilt
  real32 max_rotation_z = 20.0f;
  real32 current_z      = rotations[ i ].z;
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
  rotations[ i ].z      = new_rotation_z;
  
}

void update_ship_position( GameInput* game_input, real32 dt, uint32 ship_index = 0 ) {
  
  uint32  i                 = ship_index;
  real32  joy_y             = game_input -> joy_axis_y;
  real32  joy_x             = game_input -> joy_axis_x;
  real32  multiplier        = 8.0f;
  real32  lerp_smoothing    = 0.05f;
  
  real32 target_velocity_y  = -joy_y * multiplier;
  real32 current_velocity_y = velocities[ i ].y;
  real32 new_velocity_y     = lerp_dt( current_velocity_y, target_velocity_y, lerp_smoothing, dt );
  velocities[ i ].y         = new_velocity_y;
  
  real32 target_velocity_x  = joy_x * multiplier;
  real32 current_velocity_x = velocities[ i ].x;
  real32 new_velocity_x     = lerp_dt( current_velocity_x, target_velocity_x, lerp_smoothing, dt );
  velocities[ i ].x         = new_velocity_x;
  
  positions[ i ].y = positions[ i ].y + ( velocities[ i ].y * dt );
  positions[ i ].x = positions[ i ].x + ( velocities[ i ].x * dt );
  
  int y = 7;
}

void update_target_position( uint32 target_index = 1, uint32 ship_index = 0 ) {
  
  uint32 i = target_index;
  uint32 s = ship_index;
  
  real32 dist_from_ship = 10.0f;
  
  { // x / left and right
    real32 ship_rotation  = rotations[ s ].y;
    real32 tan_of_angle   = tan_degrees( ship_rotation );
    real32 offset         = ( dist_from_ship * tan_of_angle );
    positions[ i ].x      = positions[ s ].x - offset;
  }
  
  { // y / up and down
    real32 ship_rotation  = rotations[ s ].x;
    real32 tan_of_angle   = tan_degrees( ship_rotation );
    real32 offset         = ( dist_from_ship * tan_of_angle );
    positions[ i ].y      = positions[ s ].y + offset;
  }
  
  positions[ i ].z = ( positions[ s ].z - dist_from_ship );
  
}

void update_ship_velocity( GameInput* game_input, real32 dt, uint32 ship_index = 0 ) {
  
  uint32  i                 = ship_index;
  real32  joy_y             = game_input -> joy_axis_y;
  real32  joy_x             = game_input -> joy_axis_x;
  real32  multiplier        = 8.0f;
  real32  lerp_smoothing    = 0.05f;
  
  real32 target_velocity_y  = -joy_y * multiplier;
  real32 current_velocity_y = velocities[ i ].y;
  real32 new_velocity_y     = lerp_dt( current_velocity_y, target_velocity_y, lerp_smoothing, dt );
  velocities[ i ].y         = new_velocity_y;
  
  real32 target_velocity_x  = joy_x * multiplier;
  real32 current_velocity_x = velocities[ i ].x;
  real32 new_velocity_x     = lerp_dt( current_velocity_x, target_velocity_x, lerp_smoothing, dt );
  velocities[ i ].x         = new_velocity_x;
  
  int y = 7;
}

#endif //SHIP_HPP
