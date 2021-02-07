#shader vertex
#version 100

attribute vec4  aPosition;
attribute vec4  aColour;
varying   vec4  vColour;
uniform   mat4  uMVP;

void main() {
  gl_Position = uMVP * aPosition;
  vColour = aColour;
}

#shader fragment
#version 100

precision mediump float;

varying vec4      vColour;

void main() {
  gl_FragColor = vColour;
}
