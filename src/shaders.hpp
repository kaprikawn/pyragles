#ifndef SHADERS_HPP
#define SHADERS_HPP

u32 compileShader( u32 type, const char* source ) {
  
  u32 result;
  
  u32 shader_program_id = glCreateShader( type );
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

u32 createShader( ReadFileResult shader_file ) {
  
  u32 result;
  
  const char* shader_source = ( const char* )shader_file.contents;
  u32 filesize_max       = shader_file.contents_size;
  
  u32 type = SHADERTYPENONE;
  
  char* vertex_shader_source    = ( char* )malloc( filesize_max );
  char* fragment_shader_source  = ( char* )malloc( filesize_max );
  char  current_line [ 4086 ];
  
  u32 current_line_index     = 0;
  u32 line_start            = 0;
  u32 line_end;
  
  u32 vertex_current_index   = 0;
  u32 fragment_current_index = 0;
  
  for( u32 i = 0; i < filesize_max; i++ ) {
    char my_char = ( char )shader_source[ i ];
    if( my_char == 10 || my_char == 13 ) {
      
      line_end = i;
      
      if( strings_are_equal( ( const char* )current_line, "#shader vertex" ) ) {
        type = VERTEXSHADER;
      } else if( strings_are_equal( ( const char* )current_line, "#shader fragment" ) ) {
        type = FRAGMENTSHADER;
      } else {
        
        for( u32 j = line_start; j < line_end; j++ ) {
          
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
      
      for( u32 j = 0; j < filesize_max; j++ )
        current_line[ j ] = '\0';
      current_line_index = 0;
      line_start = i + 1;
    } else {
      current_line[ current_line_index++ ] = my_char;
    }
  }
  
  vertex_shader_source[ vertex_current_index ] = '\0';
  fragment_shader_source[ fragment_current_index ] = '\0';
  
  u32 shader_program_id  = glCreateProgram();
  u32 vs                 = compileShader( GL_VERTEX_SHADER   , vertex_shader_source );
  u32 fs                 = compileShader( GL_FRAGMENT_SHADER , fragment_shader_source );
  
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

#endif //SHADERS_HPP
