#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec3  aNormal;
attribute vec2  aTexCoord;
varying   vec3  vNormal;
varying   vec2  vTexCoord;
uniform   mat4  uMVP;
uniform   mat4  uModelViewMatrix;
uniform   mat4  uModelMatrix;
uniform   vec3  uLightPosition;
varying   vec3  theColour;
varying   float brightness;

void main() {
  vTexCoord         = aTexCoord;
  vNormal           = aNormal;
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
  
  // vertex position in world space
  vec3 vertexPosition = vec3( uModelMatrix * position );
  
  // vec3  vertexColor = vec3( 1.0, 0.0, 0.0 );
  
  vec3  lightVector = normalize( uLightPosition - vertexPosition );
  
  brightness        = dot( lightVector, vNormal );
  
}

#shader fragment
#version 100

precision mediump float;

uniform sampler2D uTexture;
varying vec2      vTexCoord;
varying vec3      vNormal;
varying vec3      theColour;
varying float     brightness;


void main() {
  
  gl_FragColor = texture2D( uTexture, vTexCoord ) * vec4( 1.0, 1.0, 1.0, 1.0 );
  gl_FragColor = texture2D( uTexture, vTexCoord ) * brightness;
  
}
