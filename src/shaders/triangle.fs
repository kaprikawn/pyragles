#version 100

precision mediump float;

varying vec4  v_colour;
void main() {
  gl_FragColor = vec4( v_colour );
}
