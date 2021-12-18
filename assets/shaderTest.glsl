#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec3  aNormal; // model space
attribute vec2  aTexCoord;
varying   vec2  vTexCoord;
uniform   mat4  uMVP;
uniform   mat4  uModelMatrix;
uniform   vec3  uLightPosition;
uniform   float uAmbientLight;
varying   float brightness;

void main() {
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
  
}

#shader fragment
#version 100

precision mediump float;

uniform sampler2D uTexture;
varying vec2      vTexCoord;
varying float     brightness;

void main() {
  
  vec4 pixelColour = vec4( 1.0, 0.0, 0.0, 1.0 ); //texture2D( uTexture, vTexCoord );
  
  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
  
}
