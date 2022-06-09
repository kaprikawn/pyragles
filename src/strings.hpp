#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <stdlib.h>
#include "types.hpp"

const char* assetsDir         = "../assets/";

// see https://en.cppreference.com/w/cpp/language/ascii
enum ascii_chars {ASCII_NUL,ASCII_SOH,ASCII_STX,ASCII_ETX,ASCII_EOT,ASCII_ENQ,ASCII_ACK,ASCII_BEL,ASCII_BS,ASCII_HT,ASCII_LF,ASCII_VT,ASCII_FF,ASCII_CR,ASCII_SO,ASCII_SI,ASCII_DLE,ASCII_DC1,ASCII_DC2,ASCII_DC3,ASCII_DC4,ASCII_NAK,ASCII_SYN,ASCII_ETB,ASCII_CAN,ASCII_EM,ASCII_SUB,ASCII_ESC,ASCII_FS,ASCII_GS,ASCII_RS,ASCII_US,ASCII_SPACE,ASCII_EXCLAMATION,ASCII_DOUBLE_QUOTE,ASCII_HASH,ASCII_DOLLAR,ASCII_PERCENTAGE,ASCII_AMPERSANDS,ASCII_APOSTROPHE,ASCII_OPEN_BRACKET,ASCII_CLOSE_BRACKET,ASCII_STAR,ASCII_UNK1,ASCII_COMMA,ASCII_HYPHEN,ASCII_DECIMAL_POINT,ASCII_FORWARD_SLASH,ASCII_0,ASCII_1,ASCII_2,ASCII_3,ASCII_4,ASCII_5,ASCII_6,ASCII_7,ASCII_8,ASCII_9,ASCII_COLON,ASCII_SEMI_COLON,ASCII_LESS_THAN,ASCII_EQUALS,ASCII_GREATER_THAN,ASCII_QUESTION_MARK,ASCII_AT_SYMBOL,ASCII_A,ASCII_B,ASCII_C,ASCII_D,ASCII_E,ASCII_F,ASCII_G,ASCII_H,ASCII_I,ASCII_J,ASCII_K,ASCII_L,ASCII_M,ASCII_N,ASCII_O,ASCII_P,ASCII_Q,ASCII_R,ASCII_S,ASCII_T,ASCII_U,ASCII_V,ASCII_W,ASCII_X,ASCII_Y,ASCII_Z,ASCII_OPEN_SQUARE_BRACKETS,ASCII_BACK_SLASH,ASCII_CLOSE_SQUARE_BRACKETS,ASCII_CARROT,ASCII_UNDERSCORE,ASCII_BACKTICK,ASCII_a,ASCII_b,ASCII_c,ASCII_d,ASCII_e,ASCII_f,ASCII_g,ASCII_h,ASCII_i,ASCII_j,ASCII_k,ASCII_l,ASCII_m,ASCII_n,ASCII_o,ASCII_p,ASCII_q,ASCII_r,ASCII_s,ASCII_t,ASCII_u,ASCII_v,ASCII_w,ASCII_x,ASCII_y,ASCII_z,ASCII_OPEN_CURLY_BRACKET,ASCII_PIPE,ASCII_CLOSE_CURLY_BRACKET,ASCII_TILDE,ASCII_DEL};


struct ReadFileResult
{
    u32   contents_size;
    void* contents;
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

inline void remove_crlf( ReadFileResult* read_file_result ) {
  
  uchar* this_char = ( uchar* ) read_file_result -> contents;
  
  for( u32 i = 0; i < read_file_result -> contents_size; i++ ) {
    if( *this_char == ASCII_CR )
      *this_char = '\0';
    
    this_char++;
  }
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
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

bool fileExists( const char* filepath ) {
  if( FILE *file = fopen( filepath, "r" ) ) {
    fclose( file );
    return true;
  }
  std::cout << filepath << " does not exist\n";
  return false;
}

inline u32 get_filesize( const char* filepath ) {
  u32 result = 0;
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
  
  u32 bytesToRead = result.contents_size;
  u8*  nextByteLocation = ( u8* )result.contents;
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

void free_memory( void* memory, u32 size ) {
  // TODO
}

#endif


#endif //STRINGS_HPP
