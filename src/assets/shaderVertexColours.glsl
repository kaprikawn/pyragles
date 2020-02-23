#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec4  aColour;
varying   vec4  vColour;
uniform   mat4  uMVP;

void main() {
  gl_Position = uMVP * vec4( aPosition, 1.0 );
  vColour = aColour;
}

#shader fragment
#version 100

precision mediump float;

varying vec4      vColour;

void main() {
  gl_FragColor = vec4( 1, 0, 0, 1 );
}
