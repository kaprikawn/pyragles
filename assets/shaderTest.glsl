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
  
  vTexCoord         = aTexCoord;
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
  
  // vertex position in world space
  vec3 vertexPosition = vec3( uModelMatrix * position );
  
  // vec3  vertexColor = vec3( 1.0, 0.0, 0.0 );
  
  vec3  lightVector = normalize( uLightPosition - vertexPosition );
  
  vec3  normal = normalize( vec3( uModelMatrix * vec4( aNormal, 0 ) ) ); // the '0' after aNormal makes sure we don't apply position transform
  
  float diffuse = clamp( dot( lightVector, normal ), 0.0, 1.0 );
  brightness    = diffuse + uAmbientLight; // send brighness to fs
}

#shader fragment
#version 100

precision mediump float;

uniform sampler2D uTexture;
varying vec2      vTexCoord;
varying float     brightness;

void main() {
  
  vec4 pixelColour = texture2D( uTexture, vTexCoord );
  
  pixelColour.rgb *= brightness;
  
  gl_FragColor = pixelColour;
  
}
