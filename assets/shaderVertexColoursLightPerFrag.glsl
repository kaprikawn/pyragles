
#shader vertex
#version 100

attribute vec3  aPosition;
attribute vec3  aNormal; // model space
attribute vec3  aColour;
varying   vec3  vPosition;
varying   vec3  vNormal;
varying   vec3  vColour;
uniform   mat4  uMVP;

void main() {
  
  vColour           = aColour;
  vNormal           = aNormal;
  
  vec4  position    = vec4( aPosition, 1.0 );
  
  gl_Position       = uMVP * position;
  
}

#shader fragment
#version 100

precision mediump float;

varying vec3      vColour;
varying vec3      vPosition;
varying vec3      vNormal;
uniform vec3      uLightPosition;
uniform mat4      uModelMatrix;
uniform float     uAmbientLight;

void main() {
  
  vec4  position          = vec4( vPosition, 1.0 );
  vec3  fragmentPosition  = vec3( uModelMatrix * position );
  vec3  lightVector       = normalize( uLightPosition - fragmentPosition );
  vec3  normal            = normalize( vec3( uModelMatrix * vec4( vNormal, 0 ) ) ); // the '0' after aNormal makes sure we don't apply position transform
  float diffuse           = clamp( dot( lightVector, normal ), 0.0, 1.0 );
  float brightness        = diffuse + uAmbientLight; // send brighness to fs
  
  vec4 pixelColour = vec4( vColour, 1.0 );
  
  pixelColour.rgb *= brightness;
  
  gl_FragColor = pixelColour;
  
}
