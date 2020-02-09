#shader vertex
#version 100

attribute vec2  aPosition;
attribute vec3  aColour;
varying   vec3  vColour;

void main() {
  gl_Position = vec4( aPosition, 0.0, 1.0 );
  vColour = aColour;
}

#shader fragment
#version 100

precision mediump float;

varying   vec3  vColour;
void main() {
  gl_FragColor = vec4( vColour, 1.0 );
}
