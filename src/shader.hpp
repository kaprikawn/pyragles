#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <string>
#include <GLES2/gl2.h>

std::string getFile( const std::string& filename ) {
  
  std::ifstream file;
  file.open( ( filename ).c_str() );
  
  std::string output;
  std::string line;
  
  if( file.is_open() ) {
    while( file.good() ) {
      getline( file, line );
      output.append( line + "\n" );
    }
  } else {
    std::cerr << "Unable to load shader: " << filename << std::endl;
  }
  
  return output;
}

void checkShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage ) {
  GLint success = 0;
  GLchar error[ 1024 ] = { 0 };
  
  if( isProgram ) {
    glGetProgramiv( shader, flag, &success );
  } else {
    glGetShaderiv( shader, flag, &success );
  }
  
  if( success == GL_FALSE ) {
    if( isProgram ) {
      glGetProgramInfoLog( shader, sizeof( error ), NULL, error );
    } else {
      glGetShaderInfoLog( shader, sizeof( error ), NULL, error );
    }
    
    std::cerr << errorMessage << ": '" << error << "'" << std::endl;
  }
}

GLuint loadShader( const char* shaderSrc, GLenum type ) {
  
  GLuint  shader;
  
  // create the shader object
  shader = glCreateShader( type );
  
  if( shader == 0 ) {
    return 0;
  }
  
  // load the shader source
  glShaderSource( shader, 1, &shaderSrc, NULL );
  
  // compile the shader
  glCompileShader( shader );
  
  // check the compile status
  //glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
  checkShaderError( shader, GL_COMPILE_STATUS, false, "Error compiling shader!");
  
  return shader;
}

#endif //SHADER_HPP
