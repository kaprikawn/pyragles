#ifndef SDL_HPP
#define SDL_HPP

#if defined DEBUG
  #define GLCall(x) GLClearError();\
                    x;\
                    GLLogCall(#x, __FILE__, __LINE__);
#else
  #define GLCall(x) x
#endif

#ifdef _WIN32

#include "SDL.h"
#include <GL/glew.h>

#elif __linux__

#include "SDL.h"
#include "SDL_opengles2.h"

#endif

inline static void GLClearError() { while ( glGetError() != GL_NO_ERROR ); }

static bool GLLogCall( const char* function, const char* file, const int line ) {
  while( GLenum error = glGetError() ) {
    
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "[OpenGL] Error : %d\n", error );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "[OpenGL]   in function : %s\n", function );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "[OpenGL]   at %s : %d\n", file, line );
    return false;
  }
  return true;
}

#endif //SDL_HPP
