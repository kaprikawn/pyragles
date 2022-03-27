
#shader vertex
#version 100

attribute vec3  aPosition;
uniform   vec3  uColour;
varying   vec3  vColour;
uniform   mat4  uMVP;

void main() {
  
  vColour           = uColour;
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
}

#shader fragment
#version 100

precision mediump float;

varying vec3      vColour;

void main() {
  
  vec4 pixelColour = vec4( vColour, 1.0 );
  
  gl_FragColor = pixelColour;
  
}
