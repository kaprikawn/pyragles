#include "glCallLog.hpp"
#include <iostream>
#include <GLES2/gl2.h>

void GLClearError() {
  while( glGetError() != GL_NO_ERROR );
}

bool GLLogCall( const char* function, const char* file, int line ) {
  while( GLenum error = glGetError() ) {
    std::cout << "[OpenGL error] (" << error << ")\n" << function << "\n" << file << "\n" << line << std::endl;
    return false;
  }
  return true;
}
