#shader vertex
#version 100

attribute vec4  aPosition;
attribute vec3  aNormal;
attribute vec2  aTexCoord;
varying   vec2  vTexCoord;
uniform   mat4  uMVP;

void main() {
  gl_Position = uMVP * aPosition, 1.0;
  vTexCoord = aTexCoord;
}

#shader fragment
#version 100

precision mediump float;

varying vec2      vTexCoord;
uniform sampler2D uTexture;

void main() {
  gl_FragColor = texture2D( uTexture, vTexCoord ) * vec4( 1.0, 1.0, 1.0, 1.0 );
}
