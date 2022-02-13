#ifndef SHIP_HPP
#define SHIP_HPP

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
  
  uint32 i      = ship_index;
  real32 joy_y  = game_input -> joy_axis_y;
  real32 joy_x  = game_input -> joy_axis_x;
  
  if( joy_y != 0.0f )
    positions[ i ].y -= joy_y * 6.0f * dt;
  
  if( joy_x != 0.0f )
    positions[ i ].x += joy_x * 6.0f * dt;
  
  
}

#endif //SHIP_HPP
