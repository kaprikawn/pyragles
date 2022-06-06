#ifndef SDL_HPP
#define SDL_HPP

#include "types.hpp"

// fullscreen decl
bool32 launch_fullscreen = false;
#ifdef FULLSCREEN_ONLY
  launch_fullscreen = true;
#endif

#ifdef _WIN32

#include "SDL.h"
#include <GL/glew.h>

#elif __linux__

#include "SDL.h"
#include "SDL_opengles2.h"

#endif

// OpenGL Debug
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

#if defined DEBUG
  #define GLCall(x) GLClearError();\
                    x;\
                    GLLogCall(#x, __FILE__, __LINE__);
#else
  #define GLCall(x) x
#endif

struct SDLParams {
  SDL_Window*   window;
  SDL_GLContext gl_context;
  u32           window_width;
  u32           window_height;
};



void init_sdl( SDLParams* sdl_params ) {
  
#ifdef _WIN32
  GLenum err = glewInit(); // init glew
#endif
  
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
  
  if( launch_fullscreen ) {
    
    u32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
    // create window to native desktop size to query screen dimensions
    SDL_Window* nullWindow      = SDL_CreateWindow( "undef", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN );
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode( 0, &dm );
    sdl_params -> window_width  = dm.w;
    sdl_params -> window_height = dm.h;
    SDL_DestroyWindow( nullWindow );
    
    SDL_ShowCursor( SDL_DISABLE );
    
    
  }
  
  u32 sdl_flags         = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED;
  
  sdl_params -> window  = SDL_CreateWindow( "Alpha Flight", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl_params -> window_width, sdl_params -> window_height, sdl_flags );
  if( !sdl_params -> window ) {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "The window could not be created! SDL_Error : %s\n", SDL_GetError() );
  }
  
  if( launch_fullscreen ) {
    SDL_SetWindowFullscreen( sdl_params -> window, SDL_WINDOW_FULLSCREEN );
  }
  
  sdl_params -> gl_context = SDL_GL_CreateContext( sdl_params -> window );
  if( sdl_params -> gl_context ) {
    const GLubyte* version  = glGetString( GL_VERSION );
    const GLubyte* renderer = glGetString( GL_RENDERER );
    
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Version string: %s\n", version );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Renderer string: %s\n", renderer );
    
  } else {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Failed to create GL context : %s\n", SDL_GetError());
  }
  
  SDL_SetWindowGrab( sdl_params -> window, SDL_TRUE );
  SDL_ShowCursor( SDL_DISABLE );
  
  GLCall( SDL_GL_SetSwapInterval( 1 ) );
  GLCall( glEnable( GL_DEPTH_TEST ) );
  GLCall( glEnable( GL_BLEND ) );
  GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
  GLCall( glViewport( 0, 0, ( float )sdl_params -> window_width, ( float )sdl_params -> window_height ) );
  GLCall( glClearColor( 0.0f, 0.65f, 1.0f, 1.0f ) );
}

#endif //SDL_HPP

