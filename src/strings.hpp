#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <stdlib.h>
#include "types.hpp"

const char* assetsDir         = "../assets/";

struct ReadFileResult
{
    u32  contents_size;
    void*   contents;
};

inline u32 string_length( const char* string ) {
  // length NOT including \0
  u32 result = 0;
  for( u32 i = 0; string[ i ] != '\0'; i++ )
    result++;
  return result;
}

inline void null_char_buffer( char* target, u32 length ) {
  for( u32 i = 0; i < length; i++ ) {
    target[ i ] = '\0';
  }
}

inline char* init_char_star( u32 length ) {
  char* result = ( char* )malloc( length );
  null_char_buffer( result, length );
  return result;
}

inline void copy_string_into_char_star( const char* src, char* dest, u32 length ) {
  for( u32 i = 0; i < length; i++ )
    dest[ i ] = src[ i ];
}

inline char* concat( const char* str1, const char* str2 ) {
  ///////////////////////////////////////////////////////
  // REMEMBER TO FREE RESULT AFTER YOU'RE DONE WITH IT //
  ///////////////////////////////////////////////////////
  
  u32 len1         = string_length( str1 );
  u32 len2         = string_length( str2 );
  u32 totalLength  = len1 + len2 + 1;
  u32 currentIndex = 0;
  
  char* result = init_char_star( totalLength );
  
  for( u32 i = 0; i < len1; i++ )
    result[ currentIndex++ ] = str1[ i ];
  for( u32 i = 0; i < len2; i++ )
    result[ currentIndex++ ] = str2[ i ];
  result[ currentIndex ] = '\0';
  
  return result;
}

inline u32 safeTruncateUInt64( u64 value ) {
  u32 result = ( u32 )value;
  return result;
}

bool32 strings_are_equal( const char* str1, const char* str2 ) {
  u32 len1 = string_length( str1 );
  u32 len2 = string_length( str2 );
  if( len1 != len2 )
    return false;
  for( u32 i = 0; i < len1; i++ ) {
    char char1 = ( char )str1[ i ];
    char char2 = ( char )str2[ i ];
    if( char1 != char2 )
      return false;
  }
  return true;
}

#ifdef _WIN32

#include <shlwapi.h>

inline u32 get_filesize( const char* filepath ) {
  u32 result = 0;
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
    u32 filesize = get_filesize ( filepath );
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

void free_memory( void* memory, u32 size ) {
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


#endif //STRINGS_HPP
