#ifndef MATHS_HPP
#define MATHS_HPP

#include "types.hpp"
#include <math.h>
#include <string.h>

const f32 DEGREES_TO_RADIANS_FACTOR = 0.01745329251994329576923690768489f;

struct Position {
  f32 x = 0.0f;
  f32 y = 0.0f;
  f32 z = 0.0f;
};

inline void mat4_multiply( f32* dest, f32* mat1, f32* mat2 ) {
  
  f32 result[ 16 ] = {
      mat1[ 0 ] * mat2[ 0 ] + mat1[ 1 ] * mat2[ 4 ] + mat1[ 2 ] * mat2[ 8 ]  + mat1[ 3 ] * mat2[ 12 ]
    , mat1[ 0 ] * mat2[ 1 ] + mat1[ 1 ] * mat2[ 5 ] + mat1[ 2 ] * mat2[ 9 ]  + mat1[ 3 ] * mat2[ 13 ]
    , mat1[ 0 ] * mat2[ 2 ] + mat1[ 1 ] * mat2[ 6 ] + mat1[ 2 ] * mat2[ 10 ] + mat1[ 3 ] * mat2[ 14 ]
    , mat1[ 0 ] * mat2[ 3 ] + mat1[ 1 ] * mat2[ 7 ] + mat1[ 2 ] * mat2[ 11 ] + mat1[ 3 ] * mat2[ 15 ]
    
    , mat1[ 4 ] * mat2[ 0 ] + mat1[ 5 ] * mat2[ 4 ] + mat1[ 6 ] * mat2[ 8 ]  + mat1[ 7 ] * mat2[ 12 ]
    , mat1[ 4 ] * mat2[ 1 ] + mat1[ 5 ] * mat2[ 5 ] + mat1[ 6 ] * mat2[ 9 ]  + mat1[ 7 ] * mat2[ 13 ]
    , mat1[ 4 ] * mat2[ 2 ] + mat1[ 5 ] * mat2[ 6 ] + mat1[ 6 ] * mat2[ 10 ] + mat1[ 7 ] * mat2[ 14 ]
    , mat1[ 4 ] * mat2[ 3 ] + mat1[ 5 ] * mat2[ 7 ] + mat1[ 6 ] * mat2[ 11 ] + mat1[ 7 ] * mat2[ 15 ]
    
    , mat1[ 8 ] * mat2[ 0 ] + mat1[ 9 ] * mat2[ 4 ] + mat1[ 10 ] * mat2[ 8 ]  + mat1[ 11 ] * mat2[ 12 ]
    , mat1[ 8 ] * mat2[ 1 ] + mat1[ 9 ] * mat2[ 5 ] + mat1[ 10 ] * mat2[ 9 ]  + mat1[ 11 ] * mat2[ 13 ]
    , mat1[ 8 ] * mat2[ 2 ] + mat1[ 9 ] * mat2[ 6 ] + mat1[ 10 ] * mat2[ 10 ] + mat1[ 11 ] * mat2[ 14 ]
    , mat1[ 8 ] * mat2[ 3 ] + mat1[ 9 ] * mat2[ 7 ] + mat1[ 10 ] * mat2[ 11 ] + mat1[ 11 ] * mat2[ 15 ]
    
    , mat1[ 12 ] * mat2[ 0 ] + mat1[ 13 ] * mat2[ 4 ] + mat1[ 14 ] * mat2[ 8 ]  + mat1[ 15 ] * mat2[ 12 ]
    , mat1[ 12 ] * mat2[ 1 ] + mat1[ 13 ] * mat2[ 5 ] + mat1[ 14 ] * mat2[ 9 ]  + mat1[ 15 ] * mat2[ 13 ]
    , mat1[ 12 ] * mat2[ 2 ] + mat1[ 13 ] * mat2[ 6 ] + mat1[ 14 ] * mat2[ 10 ] + mat1[ 15 ] * mat2[ 14 ]
    , mat1[ 12 ] * mat2[ 3 ] + mat1[ 13 ] * mat2[ 7 ] + mat1[ 14 ] * mat2[ 11 ] + mat1[ 15 ] * mat2[ 15 ]
  };
  
  memcpy( dest, &result, sizeof( result ) );
}

inline void rotate_x( f32* dest, f32 degrees ) {
  // https://math.stackexchange.com/questions/72014/given-an-angle-in-radians-how-could-i-calculate-a-4x4-rotation-matrix-about-the
  // https://en.wikipedia.org/wiki/Rotation_matrix ( In three dimensions )
  f32 radians = degrees * DEGREES_TO_RADIANS_FACTOR; // theta
  f32 c1r1, c2r1, c1r2, c2r2;
  c1r1  = cos( radians );
  c2r1  = sin( radians );
  c1r2  = -( sin( radians ) );
  c2r2  = cos( radians );
  dest += 5;
  *dest = c1r1;
  dest++;
  *dest = c2r1;
  dest += 3;
  *dest = c1r2;
  dest++;
  *dest = c2r2;
}

inline void rotate_y( f32* dest, f32 degrees ) {
  f32 radians = degrees * DEGREES_TO_RADIANS_FACTOR; // theta
  f32 c0r0, c2r0, c0r2, c2r2;
  c0r0  = cos( radians );
  c2r0  = -( sin( radians ) );
  c0r2  = sin( radians );
  c2r2  = cos( radians );
  *dest = c0r0;
  dest += 2;
  *dest = c2r0;
  dest += 6;
  *dest = c0r2;
  dest += 2;
  *dest = c2r2;
}

inline void rotate_z( f32* dest, f32 degrees ) {
  f32 radians = degrees * DEGREES_TO_RADIANS_FACTOR; // theta
  f32 c0r0, c1r0, c0r1, c1r1;
  c0r0  = cos( radians );
  c1r0  = sin( radians );
  c0r1  = -( sin( radians ) );
  c1r1  = cos( radians );
  *dest = c0r0;
  dest++;
  *dest = c1r0;
  dest += 3;
  *dest = c0r1;
  dest++;
  *dest = c1r1;
}

void populate_perspective_matrix( f32* dest, f32 fov_degrees, f32 aspect, f32 near_plane, f32 far_plane ) {
  
  f32 fov_radians = fov_degrees * DEGREES_TO_RADIANS_FACTOR;
  const f32 tan_half_fov = tan( fov_radians / 2.0f );
  
  *dest = 1.0f / ( aspect * tan_half_fov );
  dest += 5;
  *dest = 1.0f / tan_half_fov;
  dest += 5;
  *dest = -( far_plane + near_plane ) / ( far_plane - near_plane );
  dest++;
  *dest = -1.0f;
  dest += 3;
  *dest = -( 2.0f * far_plane * near_plane ) / ( far_plane - near_plane );
}

void normalise_vec3( f32* x, f32* y, f32* z ) {
  // https://www.google.com/search?q=normalize+3d+vector&rlz=1C1CHBD_en-GBGB936GB936&oq=normalize+3d+vector&aqs=chrome..69i57.5830j0j7&sourceid=chrome&ie=UTF-8#kpvalbx=_DqBpYLTWLoe0gQblyLGIDQ21
  
  f32 temp_x = *x;
  f32 temp_y = *y;
  f32 temp_z = *z;
  
  f32 length = sqrt( ( temp_x * temp_x ) + ( temp_y * temp_y ) + ( temp_z * temp_z ) );
  
  f32 set_x = temp_x / length;
  f32 set_y = temp_y / length;
  f32 set_z = temp_z / length;
  
  *x = set_x;
  *y = set_y;
  *z = set_z;
}

void populate_view_matrix( f32* dest, Position eye, Position centre ) {
  // https://www.3dgep.com/understanding-the-view-matrix/
  // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/cross.xhtml
  
  Position up = { 0.0f, 1.0f, 0.0f };
  
  Position f;
  Position s;
  Position u;
  
  f.x = centre.x - eye.x;
  f.y = centre.y - eye.y;
  f.z = centre.z - eye.z;
  
  normalise_vec3( &f.x, &f.y, &f.z );
  
  s.x = f.y * up.z - up.y * f.z;
  s.y = f.z * up.x - up.z * f.x;
  s.z = f.x * up.y - up.x * f.y;
  
  normalise_vec3( &s.x, &s.y, &s.z );
  
  u.x = s.y * f.z - f.y * s.z;
  u.y = s.z * f.x - f.z * s.x;
  u.z = s.x * f.y - f.x * s.y;
  
  *dest = s.x; dest++;
  *dest = u.x; dest++;
  *dest = -f.x; dest++;
  dest++;
  *dest = s.y; dest++;
  *dest = u.y; dest++;
  *dest = -f.y; dest++;
  dest++;
  *dest = s.z; dest++;
  *dest = u.z; dest++;
  *dest = -f.z; dest++;
  dest++;
  *dest = -( s.x * eye.x + s.y * eye.y + s.z * eye.z ); dest++;
  *dest = -( u.x * eye.x + u.y * eye.y + u.z * eye.z ); dest++;
  *dest = f.x * eye.x + f.y * eye.y + f.z * eye.z;
  
  int y = 7;
}

inline f32 lerp( f32 src, f32 dest, f32 alpha ) {
  return ( ( src * ( 1 - alpha ) ) + ( dest * alpha ) );
}

inline f32 lerp_dt( f32 src, f32 dest, f32 smoothing, f32 dt ) {
  return lerp( src, dest, 1 - pow( smoothing, dt ) );
}

inline void translate( f32* matrix, Position position ) {
  matrix += 12;
  *matrix = position.x;
  matrix++;
  *matrix = position.y;
  matrix++;
  *matrix = position.z;
}

inline f32 tan_degrees( f32 degrees ) {
  f32 result;
  f32 radians  = degrees * DEGREES_TO_RADIANS_FACTOR;
  result = tan( radians );
  return result;
}

inline f32 tan_radians( f32 radians ) {
  f32 result;
  result = tan( radians );
  return result;
}

#endif //MATHS_HPP
