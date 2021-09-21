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
varying   vec3  theColour;
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
  
  float diffuse = clamp( dot( lightVector, normal ), 0, 1 );
  brightness    = diffuse + uAmbientLight; // send brighness to fs
}

#shader fragment
#version 100

precision mediump float;

uniform sampler2D uTexture;
varying vec2      vTexCoord;
varying vec3      theColour;
varying float     brightness;


void main() {
  
  // gl_FragColor = texture2D( uTexture, vTexCoord ) * vec4( 1.0, 1.0, 1.0, 1.0 );
  // gl_FragColor = texture2D( uTexture, vTexCoord ) * brightness;
  //gl_FragColor = vec4( brightness, brightness, brightness, 1.0 );
  
  vec4 pixelColour = texture2D( uTexture, vTexCoord );
  
  float r = pixelColour.r * brightness;
  float g = pixelColour.g * brightness;
  float b = pixelColour.b * brightness;
  float a = pixelColour.a;
  
  gl_FragColor = vec4( r, g, b, a );
  
}
