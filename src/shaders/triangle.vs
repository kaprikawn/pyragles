uniform float u_time;
attribute vec2 va_position;
varying vec3 v_color;
void main() {
  v_color = vec3( 1.0 - 0.5 * ( va_position.x + va_position.y ), va_position );
  float c = cos( u_time ), s = sin( u_time );
  vec2 t = mat2( c, s, -s, c ) * ( va_position - vec2( 0.33 ) );
  gl_Position = vec4( t.x * 3.0 / 5.0, t.y, 0.0, 1.0 );
}
