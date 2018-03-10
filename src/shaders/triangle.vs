#version 100

attribute vec4  aPosition;
attribute vec3  aColour;
varying   vec3  vColour;

void main() {
  gl_Position = aPosition;
  vColour = aColour;
}
