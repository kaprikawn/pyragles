
#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec3  aNormal; // model space
attribute vec3  aColour;
varying   vec3  vColour;
uniform   mat4  uMVP;
uniform   mat4  uModelMatrix;
uniform   vec3  uLightPosition;
uniform   float uAmbientLight;
varying   float brightness;

void main() {
  
  vColour           = aColour;
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
}

#shader fragment
#version 100

precision mediump float;

varying vec3      vColour;
varying float     brightness;

void main() {
  
  vec4 pixelColour = vec4( vColour, 1.0 );
  
  gl_FragColor = pixelColour;
  
}
