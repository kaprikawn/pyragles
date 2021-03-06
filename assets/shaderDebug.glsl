#shader vertex
#version 100

attribute vec4  aPosition;
uniform   mat4  uMVP;

void main() {
  gl_Position = uMVP * aPosition;
}

#shader fragment
#version 100

precision mediump float;

void main() {
  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
