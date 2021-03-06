#ifndef GAME_HPP
#define GAME_HPP

#include "types.hpp"
#include <stdlib.h>

#ifdef _WIN32

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include <shlwapi.h>

#elif __linux__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#endif

/*
#include <signal.h>

#define ASSERT(x) if( !(x)) raise(SIGINT);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT( GLLogCall( #x, __FILE__, __LINE__ ) )

  
void GLClearError();

bool GLLogCall( const char* function, const char* file, int line );
*/


#if DEBUG
#define Assert( expression ) if( !( expression ) ) {*(int * )0 = 0; }
#else
#define Assert( expression )
#endif

const char* assetsDir         = "../assets/";
bool32      launch_fullscreen = false;

struct ReadFileResult
{
    uint32  contentsSize;
    void*   contents;
};

inline uint32 string_length( const char* string ) {
  // length NOT including \0
  uint32 result = 0;
  for( uint32 i = 0; string[ i ] != '\0'; i++ )
    result++;
  return result;
}

char* concat( const char* str1, const char* str2 ) {
  ///////////////////////////////////////////////////////
  // REMEMBER TO FREE RESULT AFTER YOU'RE DONE WITH IT //
  ///////////////////////////////////////////////////////
  
  uint32 len1         = string_length( str1 );
  uint32 len2         = string_length( str2 );
  uint32 totalLength  = len1 + len2 + 1;
  uint32 currentIndex = 0;
  
  char* result = ( char* )malloc( totalLength );
  
  for( uint32 i = 0; i < len1; i++ )
    result[ currentIndex++ ] = str1[ i ];
  for( uint32 i = 0; i < len2; i++ )
    result[ currentIndex++ ] = str2[ i ];
  result[ currentIndex ] = '\0';
  
  return result;
}

inline uint32 safeTruncateUInt64( uint64 value ) {
  uint32 result = ( uint32 )value;
  return result;
}

#ifdef _WIN32

inline uint32 get_filesize( const char* filepath ) {
  uint32 result = 0;
  LARGE_INTEGER filesize;
  HANDLE fh = CreateFileA( filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
  if( !( GetFileSizeEx( fh, &filesize ) ) ) {
    // TODO : logging
    CloseHandle( fh );
    return result;
  }
  CloseHandle( fh );
  result = safeTruncateUInt64( filesize.QuadPart );
  return result;
}


inline ReadFileResult read_entire_file( const char* filename ) {
  // TODO : memory leaks on failure
  ReadFileResult result = {};
  
  char* filepath = concat( assetsDir, filename );
  
  HANDLE fh = CreateFileA( filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
  if( fh != INVALID_HANDLE_VALUE ) {
    uint32 filesize = get_filesize ( filepath );
    if( filesize == 0 )
      return result;
    result.contents = VirtualAlloc( 0, filesize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
    DWORD BytesRead;
    if( ReadFile( fh, result.contents, filesize, &BytesRead, 0 ) && filesize == BytesRead ) {
      result.contentsSize = filesize;
    }
  }
  CloseHandle( fh );
  free( filepath );
  return result;
}

void read_entire_file_into_memory( const char* filename, ReadFileResult* result ) {
  
  char* filepath = concat( assetsDir, filename );
  
  HANDLE fh = CreateFileA( filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
  
  if( fh == INVALID_HANDLE_VALUE ) {
    // TODO : logging
    return;
  }
  
  result -> contentsSize = get_filesize( filepath );
  
  result -> contents = VirtualAlloc( 0, result -> contentsSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
  
  DWORD bytesRead;
  if( ReadFile( fh, result -> contents, result -> contentsSize, &bytesRead, 0 ) && result -> contentsSize == bytesRead ) {
    // file read successfully
  } else {
    VirtualFree( result -> contents, result -> contentsSize, MEM_RELEASE );
    result -> contents = 0;
    result -> contentsSize = 0;
  }
  
  CloseHandle( fh );
  free( filepath );
}

void free_memory( void* memory, uint32 size ) {
  VirtualFree( ( LPVOID )memory, size, MEM_RELEASE );
}

bool32 fileExists( const char* filepath ) {
  bool32 result = false;
  int retval;
  retval = PathFileExistsA( ( char* )filepath );
  if( retval == 1 )
    result = true;
  return result;
}

#elif __linux__

#include <sys/stat.h>

bool fileExists( const char* filepath ) {
  if( FILE *file = fopen( filepath, "r" ) ) {
    fclose( file );
    return true;
  }
  std::cout << filepath << " does not exist\n";
  return false;
}

inline uint32 get_filesize( const char* filepath ) {
  uint32 result = 0;
  // TODO
  return result;
}


ReadFileResult read_entire_file( const char* filename ) {
  
  char* filepath = concat( assetsDir, filename );
  
  ReadFileResult result = {};
  if( !fileExists( filepath ) ) {
    std::cout << filepath << " does not exist" << std::endl;
    return result;
  }
  
  int fh = open( filepath, O_RDONLY );
  if( fh == -1 )
    return result;
    
  struct stat fileStatus;
  if( fstat( fh, &fileStatus ) == -1 ) {
    close( fh );
    return result;
  }
  
  result.contentsSize = fileStatus.st_size;
  
  result.contents = malloc( result.contentsSize );
  
  if( !result.contents ) {
    close( fh );
    result.contentsSize = 0;
    return result;
  }
  
  uint32 bytesToRead = result.contentsSize;
  uint8*  nextByteLocation = ( uint8* )result.contents;
  while( bytesToRead ) {
    ssize_t bytesRead = read( fh, nextByteLocation, bytesToRead );
    if( bytesRead  == -1 ) {
      free( result.contents );
      result.contents = 0;
      result.contentsSize = 0;
      close( fh );
      return result;
    }
    bytesToRead -= bytesRead;
    nextByteLocation += bytesRead;
  }
  
  close( fh );
  
  return result;
}

void free_memory( void* memory, uint32 size ) {
  // TODO
}

#endif

struct SDLObjects {
  SDL_Window*   window;
  SDL_GLContext glContext;
  int windowWidth;
  int windowHeight;
};

enum game_memory_type {
  GAME_MEMORY_NONE, PERMANENT, TEMP
};

struct game_memory {
  bool32  isInitialized;
  uint64  permanentStorageSize;
  void*   permanentStorage;
  uint32  permanentNextMemoryOffset;
  uint64  tempStorageSize;
  void*   tempStorage;
  uint32  tempNextMemoryOffset;
};

uint32 shaderProgramID;

void init_sdl( SDLObjects* sdlObjects ) {
  
  int windowWidth   = 1280;
  int windowHeight  = 720;
  int windowX       = 50;
  int windowY       = 50;
  uint32 sdlFlags = SDL_WINDOW_OPENGL;
  
#ifdef FULLSCREEN_ONLY
  launch_fullscreen = true;
#endif
  
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );
  
  if( launch_fullscreen ) {
    uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
    
    // create window to native desktop size to query screen dimensions
    
    SDL_Window* nullWindow = SDL_CreateWindow( "undef", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN );
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode( 0, &dm );
    windowWidth   = dm.w;
    windowHeight  = dm.h;
    SDL_DestroyWindow( nullWindow );
    
    windowX       = SDL_WINDOWPOS_UNDEFINED;
    windowY       = SDL_WINDOWPOS_UNDEFINED;
    
    SDL_ShowCursor( SDL_DISABLE );
  }
  
  sdlObjects -> window = SDL_CreateWindow( "Alpha Flight", windowX, windowY, windowWidth, windowHeight, sdlFlags );
  if( !sdlObjects -> window ) {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "The window could not be created! SDL_Error : %s\n", SDL_GetError() );
  }
  
  if( launch_fullscreen ) {
    SDL_SetWindowFullscreen( sdlObjects -> window, SDL_WINDOW_FULLSCREEN );
  }
  
  sdlObjects -> glContext = SDL_GL_CreateContext( sdlObjects -> window );
  if( !( sdlObjects -> glContext ) ) {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to create GL context : %s\n", SDL_GetError());
  }
  
  // init glew
#ifdef _WIN32
  GLenum err = glewInit();
#endif
  
  SDL_GL_SetSwapInterval( 1 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glViewport( 0, 0, ( float )windowWidth, ( float )windowHeight );
  glClearColor( 0.0f, 0.65f, 1.0f, 1.0f );
  
  sdlObjects -> windowWidth   = windowWidth;
  sdlObjects -> windowHeight  = windowHeight;
  
};

bool32 strings_are_equal( const char* str1, const char* str2 ) {
  uint32 len1 = string_length( str1 );
  uint32 len2 = string_length( str2 );
  if( len1 != len2 )
    return false;
  for( uint32 i = 0; i < len1; i++ ) {
    char char1 = ( char )str1[ i ];
    char char2 = ( char )str2[ i ];
    if( char1 != char2 )
      return false;
  }
  return true;
}


uint32 compileShader( uint32 type, const char* source ) {
  
  uint32 id = glCreateShader( type );
  glShaderSource( id, 1, &source, nullptr );
  glCompileShader( id );
  
  int result;
  glGetShaderiv( id, GL_COMPILE_STATUS, &result );
  if( result == GL_FALSE ) {
    int length;
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
    char* message = ( char* )alloca( length * sizeof( char ) );
    glGetShaderInfoLog( id, length, &length, message );
    
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to complile %s shader : %s\n", ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ), message );
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "### SOURCE ### \n %s \n", source );
    glDeleteShader( id );
    return 0;
  }
  
  return id;
}

uint32 createShader( const char* shaderSource, uint32 filesizeMax ) {
  
  enum ShaderType {
    SHADERTYPENONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  
  ShaderType type = SHADERTYPENONE;
  
  
  char* vertexShaderSource    = ( char* )malloc( filesizeMax );
  char* fragmentShaderSource  = ( char* )malloc( filesizeMax );
  char currentLine [ 4086 ];
  
  uint32 currentLineIndex     = 0;
  uint32 lineStart            = 0;
  uint32 lineEnd;
  
  uint32 vertexCurrentIndex   = 0;
  uint32 fragmentCurrentIndex = 0;
  
  for( uint32 i = 0; i < filesizeMax; i++ ) {
    char myChar = ( char )shaderSource[ i ];
    if( myChar == 10 || myChar == 13 ) {
      
      lineEnd = i;
      
      if( strings_are_equal( ( const char* )currentLine, "#shader vertex" ) ) {
        type = VERTEX;
      } else if( strings_are_equal( ( const char* )currentLine, "#shader fragment" ) ) {
        type = FRAGMENT;
      } else {
        
        for( uint32 j = lineStart; j < lineEnd; j++ ) {
          
          if( type == VERTEX ) {
            vertexShaderSource[ vertexCurrentIndex++ ] = ( char )shaderSource[ j ];
          } else if( type == FRAGMENT ) {
            fragmentShaderSource[ fragmentCurrentIndex++ ] = ( char )shaderSource[ j ];
          }
          
        }
        
        if( type == VERTEX ) {
          vertexShaderSource[ vertexCurrentIndex++ ] = 0x0a;
        } else if( type == FRAGMENT ) {
          fragmentShaderSource[ fragmentCurrentIndex++ ] = 0x0a;
        }
      }
      
      for( uint32 j = 0; j < filesizeMax; j++ )
        currentLine[ j ] = '\0';
      currentLineIndex = 0;
      lineStart = i + 1;
    } else {
      currentLine[ currentLineIndex++ ] = myChar;
    }
  }
  
  vertexShaderSource[ vertexCurrentIndex ] = '\0';
  fragmentShaderSource[ fragmentCurrentIndex ] = '\0';
  
  uint32 shaderProgramID = glCreateProgram();
  uint32 vs = compileShader( GL_VERTEX_SHADER, vertexShaderSource );
  uint32 fs = compileShader( GL_FRAGMENT_SHADER, fragmentShaderSource );
  
  glAttachShader( shaderProgramID, vs );
  glAttachShader( shaderProgramID, fs );
    
  glLinkProgram( shaderProgramID );
  glValidateProgram( shaderProgramID );
  
  glDeleteShader( vs );
  glDeleteShader( fs );
  

  free( vertexShaderSource );
  free( fragmentShaderSource );
  
  return shaderProgramID;
}

#endif //GAME_HPP
