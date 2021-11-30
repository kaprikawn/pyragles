
#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec3  aNormal; // model space
attribute vec3  aColour;
uniform   mat4  uMVP;
uniform   mat4  uModelMatrix;
uniform   vec3  uLightPosition;
uniform   float uAmbientLight;
varying   vec3  vColour;
varying   float brightness;

void main() {
  vColour               = aColour;
  vec4  position        = vec4( aPosition, 1.0 );
  gl_Position           = uMVP * position;
  // vertex position in world space
  vec3  vertexPosition  = vec3( uModelMatrix * position );
  vec3  lightVector     = normalize( uLightPosition - vertexPosition );
  vec3  normal          = normalize( vec3( uModelMatrix * vec4( aNormal, 0 ) ) ); // the '0' after aNormal makes sure we don't apply position transform
  float diffuse         = clamp( dot( lightVector, normal ), 0.0, 1.0 );
  brightness            = diffuse + uAmbientLight; // send brighness to fs
}

#shader fragment
#version 100

precision mediump float;

varying vec3      vColour;
varying float     brightness;

void main() {
  vec4 pixelColour  = vec4( vColour, 1.0 );
  pixelColour.rgb  *= brightness;
  gl_FragColor      = pixelColour;
}
