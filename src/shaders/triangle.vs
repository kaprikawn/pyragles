uniform mat4 mvpMatrix;
attribute vec4 position;
void main() {
  gl_Position = mvpMatrix * position;
}
