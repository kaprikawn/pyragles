#version 100

attribute vec3  aPosition;
attribute vec3  aColour;
varying   vec3  vColour;
uniform   mat4  uMVP;

void main() {
  gl_Position = uMVP * vec4( aPosition, 1.0 );
  vColour = aColour;
}
