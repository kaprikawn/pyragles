#shader vertex
#version 100

attribute vec4  aPosition;
attribute vec3  aNormal;
attribute vec2  aTexCoord;
varying   vec2  vTexCoord;
varying   vec3  vNormal;
varying   float vDiffuse;
uniform   mat4  uMVP;
uniform   mat4  uModelViewMatrix;
float           diffuse;
float           distance;

void main() {
  vTexCoord   = aTexCoord;
  vNormal     = aNormal;
  
  vec3 modelViewVertex = vec3( uModelViewMatrix * aPosition );
  vec3 modelViewNormal = vec3( uModelViewMatrix * vec4( aNormal, 0.0 ) );
  
  vec3 u_LightPos = vec3( 0, 10, 0 );
  
  distance = length( u_LightPos - modelViewVertex );
  
  vec3 lightVector = normalize( u_LightPos - modelViewVertex );
  
  diffuse = max( dot( modelViewNormal, lightVector ), 0.1 );
  
  vDiffuse = diffuse * ( 1.0 / ( 1.0 + ( 0.25 * distance * distance ) ) );
  
  gl_Position = uMVP * aPosition, 1.0;
}

// https://www.learnopengles.com/android-lesson-two-ambient-and-diffuse-lighting/

#shader fragment
#version 100

precision mediump float;

varying vec2      vTexCoord;
varying vec3      vNormal;
varying float     vDiffuse;
uniform sampler2D uTexture;

void main() {
  
  gl_FragColor          = texture2D( uTexture, vTexCoord ) * vec4( 1.0, 1.0, 1.0, 1.0 );
  
}
