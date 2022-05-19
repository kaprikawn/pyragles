#ifndef GAME_HPP
#define GAME_HPP

#include "types.hpp"
#include "sdl.hpp"
#include "vector_maths.hpp"
#include "gl_debug.hpp"
#include <stdlib.h>

#ifdef _WIN32

#include <GL/glew.h>
#include <shlwapi.h>

#elif __linux__

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#endif

#if DEBUG
#define Assert( expression ) if( !( expression ) ) {*(int * )0 = 0; }
#else
#define Assert( expression )
#endif

const char* assetsDir         = "../assets/";
bool32      launch_fullscreen = false;

struct ReadFileResult
{
    uint32  contents_size;
    void*   contents;
};

inline uint32 string_length( const char* string ) {
  // length NOT including \0
  uint32 result = 0;
  for( uint32 i = 0; string[ i ] != '\0'; i++ )
    result++;
  return result;
}

inline void null_char_buffer( char* target, uint32 length ) {
  for( uint32 i = 0; i < length; i++ ) {
    target[ i ] = '\0';
  }
}

inline char* init_char_star( uint32 length ) {
  char* result = ( char* )malloc( length );
  null_char_buffer( result, length );
  return result;
}

inline char* concat( const char* str1, const char* str2 ) {
  ///////////////////////////////////////////////////////
  // REMEMBER TO FREE RESULT AFTER YOU'RE DONE WITH IT //
  ///////////////////////////////////////////////////////
  
  uint32 len1         = string_length( str1 );
  uint32 len2         = string_length( str2 );
  uint32 totalLength  = len1 + len2 + 1;
  uint32 currentIndex = 0;
  
  char* result = init_char_star( totalLength );
  
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
  
  ReadFileResult result = {};
  
  char* filepath = concat( assetsDir, filename );
  
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Reading file %s\n", filepath );
  
  HANDLE fh = CreateFileA( filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
  if( fh != INVALID_HANDLE_VALUE ) {
    uint32 filesize = get_filesize ( filepath );
    if( filesize == 0 )
      return result;
    result.contents = VirtualAlloc( 0, filesize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
    DWORD BytesRead;
    if( ReadFile( fh, result.contents, filesize, &BytesRead, 0 ) && filesize == BytesRead ) {
      result.contents_size = filesize;
    }
  } else {
    CloseHandle( fh );
    free( filepath );
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
  
  result -> contents_size = get_filesize( filepath );
  
  result -> contents = VirtualAlloc( 0, result -> contents_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
  
  DWORD bytesRead;
  if( ReadFile( fh, result -> contents, result -> contents_size, &bytesRead, 0 ) && result -> contents_size == bytesRead ) {
    // file read successfully
  } else {
    VirtualFree( result -> contents, result -> contents_size, MEM_RELEASE );
    result -> contents = 0;
    result -> contents_size = 0;
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
  
  result.contents_size = fileStatus.st_size;
  
  result.contents = malloc( result.contents_size );
  
  if( !result.contents ) {
    close( fh );
    result.contents_size = 0;
    return result;
  }
  
  uint32 bytesToRead = result.contents_size;
  uint8*  nextByteLocation = ( uint8* )result.contents;
  while( bytesToRead ) {
    ssize_t bytesRead = read( fh, nextByteLocation, bytesToRead );
    if( bytesRead  == -1 ) {
      free( result.contents );
      result.contents = 0;
      result.contents_size = 0;
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

struct SDLParams {
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

struct GameState {
  real32    vp_mat[ 16 ]; // view perspective matrix
  real32    p_mat [ 16 ]; // projection matrix
  Position  eye      = { 0.0f, 5.0f, 10.0f };
  Position  look_at  = { 0.0f, 5.0f, 9.0f };
  int32     vbo;
  int32     ibo;
  uint32    array_buffer_target           = 0;
  uint32    element_array_buffer_target   = 0;
  uint32    target_gl_offsets_array_data  = 0;
  uint32    target_gl_offsets_index_data  = 0;
  uint32    target_texture_data_array_pos = 0;
  bool32    invert_y                      = true;
};

uint32 shader_program_id;

void init_sdl( SDLParams* sdl_params ) {
  
  int windowWidth   = 1280;
  int windowHeight  = 720;
  int windowX       = 50;
  int windowY       = 50;
  uint32 sdlFlags = SDL_WINDOW_OPENGL;
  
#ifdef FULLSCREEN_ONLY
  launch_fullscreen = true;
#endif
  
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
  
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
  
  sdl_params -> window = SDL_CreateWindow( "Alpha Flight", windowX, windowY, windowWidth, windowHeight, sdlFlags );
  if( !sdl_params -> window ) {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "The window could not be created! SDL_Error : %s\n", SDL_GetError() );
  }
  
  if( launch_fullscreen ) {
    SDL_SetWindowFullscreen( sdl_params -> window, SDL_WINDOW_FULLSCREEN );
  }
  
  sdl_params -> glContext = SDL_GL_CreateContext( sdl_params -> window );
  if( sdl_params -> glContext ) {
    const GLubyte* version  = glGetString( GL_VERSION );
    const GLubyte* renderer = glGetString( GL_RENDERER );
    
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Version string: %s\n", version );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Renderer string: %s\n", renderer );
    
  } else {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Failed to create GL context : %s\n", SDL_GetError());
  }
  
  // init glew
#ifdef _WIN32
  GLenum err = glewInit();
#endif
  
  GLCall( SDL_GL_SetSwapInterval( 1 ) );
  GLCall( glEnable( GL_DEPTH_TEST ) );
  GLCall( glEnable( GL_BLEND ) );
  GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
  GLCall( glViewport( 0, 0, ( float )windowWidth, ( float )windowHeight ) );
  GLCall( glClearColor( 0.0f, 0.65f, 1.0f, 1.0f ) );
  
  sdl_params -> windowWidth   = windowWidth;
  sdl_params -> windowHeight  = windowHeight;
  
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
  
  uint32 result;
  
  uint32 shader_program_id = glCreateShader( type );
  glShaderSource( shader_program_id, 1, &source, nullptr );
  glCompileShader( shader_program_id );
  
  int compile_result;
  glGetShaderiv( shader_program_id, GL_COMPILE_STATUS, &compile_result );
  if( compile_result == GL_FALSE ) {
    int length;
    glGetShaderiv( shader_program_id, GL_INFO_LOG_LENGTH, &length );
    char* message = ( char* )alloca( length * sizeof( char ) );
    glGetShaderInfoLog( shader_program_id, length, &length, message );
    
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Failed to complile %s shader : %s\n", ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ), message );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "### SOURCE ### \n %s \n", source );
    glDeleteShader( shader_program_id );
    return 0;
  }
  result = shader_program_id;
  return result;
}

enum ShaderCompilationTypes {
  SHADERTYPENONE = -1, VERTEXSHADER = 0, FRAGMENTSHADER = 1
};

uint32 createShader( ReadFileResult shader_file ) {
  
  uint32 result;
  
  const char* shader_source = ( const char* )shader_file.contents;
  uint32 filesize_max       = shader_file.contents_size;
  
  uint32 type = SHADERTYPENONE;
  
  char* vertex_shader_source    = ( char* )malloc( filesize_max );
  char* fragment_shader_source  = ( char* )malloc( filesize_max );
  char  current_line [ 4086 ];
  
  uint32 current_line_index     = 0;
  uint32 line_start            = 0;
  uint32 line_end;
  
  uint32 vertex_current_index   = 0;
  uint32 fragment_current_index = 0;
  
  for( uint32 i = 0; i < filesize_max; i++ ) {
    char my_char = ( char )shader_source[ i ];
    if( my_char == 10 || my_char == 13 ) {
      
      line_end = i;
      
      if( strings_are_equal( ( const char* )current_line, "#shader vertex" ) ) {
        type = VERTEXSHADER;
      } else if( strings_are_equal( ( const char* )current_line, "#shader fragment" ) ) {
        type = FRAGMENTSHADER;
      } else {
        
        for( uint32 j = line_start; j < line_end; j++ ) {
          
          if( type == VERTEXSHADER ) {
            vertex_shader_source[ vertex_current_index++ ] = ( char )shader_source[ j ];
          } else if( type == FRAGMENTSHADER ) {
            fragment_shader_source[ fragment_current_index++ ] = ( char )shader_source[ j ];
          }
          
        }
        
        if( type == VERTEXSHADER ) {
          vertex_shader_source[ vertex_current_index++ ] = 0x0a;
        } else if( type == FRAGMENTSHADER ) {
          fragment_shader_source[ fragment_current_index++ ] = 0x0a;
        }
      }
      
      for( uint32 j = 0; j < filesize_max; j++ )
        current_line[ j ] = '\0';
      current_line_index = 0;
      line_start = i + 1;
    } else {
      current_line[ current_line_index++ ] = my_char;
    }
  }
  
  vertex_shader_source[ vertex_current_index ] = '\0';
  fragment_shader_source[ fragment_current_index ] = '\0';
  
  uint32 shader_program_id  = glCreateProgram();
  uint32 vs                 = compileShader( GL_VERTEX_SHADER   , vertex_shader_source );
  uint32 fs                 = compileShader( GL_FRAGMENT_SHADER , fragment_shader_source );
  
  glAttachShader( shader_program_id, vs );
  glAttachShader( shader_program_id, fs );
    
  glLinkProgram( shader_program_id );
  glValidateProgram( shader_program_id );
  
  glDeleteShader( vs );
  glDeleteShader( fs );
  

  free( vertex_shader_source );
  free( fragment_shader_source );
  
  result = shader_program_id;
  
  return result;
}

#endif //GAME_HPP
