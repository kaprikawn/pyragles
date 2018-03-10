#version 100

attribute vec3  aPosition;
attribute vec3  aColour;
varying   vec3  vColour;

void main() {
  gl_Position = vec4( aPosition, 1.0 );
  vColour = aColour;
}
