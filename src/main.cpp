#include <GLES2/gl2.h>
#include <SDL2/SDL.h>

int main( int argc, char **argv ) {
  SDL_GLContext context;
  int rc;
  const GLubyte *version;
  SDL_Window *window;

  rc = SDL_Init( SDL_INIT_VIDEO );

  if( rc < 0 ) {
    return 1;
  }

  atexit( SDL_Quit );

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
  
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

  window = SDL_CreateWindow( "OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL );
  context = SDL_GL_CreateContext(window);
  version = glGetString(GL_VERSION);

  if( version == 0 ) {
    printf( "Unable to get OpenGL ES version string: %d\n", glGetError() );
    return 1;
  }

  printf( "Version string: %s\n", version );

  SDL_GL_DeleteContext( context );

  return 0;
}
