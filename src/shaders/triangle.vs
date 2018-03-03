#version 100

uniform   mat4  u_mvpMatrix;
attribute vec4  a_position;
attribute vec4  a_colour;
varying   vec4  v_colour;  
void main() {
  gl_Position = u_mvpMatrix * a_position;
  v_colour = a_colour;
}
