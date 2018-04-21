#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <string>

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
  
  GLuint shader = glCreateShader( type );
  
  if( shader == 0 )
    return 0;
  
  glShaderSource( shader, 1, &shaderSrc, NULL );
  
  glCompileShader( shader );
  
  checkShaderError( shader, GL_COMPILE_STATUS, false, "Error compiling shader!");
  
  return shader;
}

GLuint Shader::init() {
  
  GLuint  vs;
  GLuint  fs;
  
  std::string vsSrcStr = getFile( "./assets/triangle.vs" );
  std::string fsSrcStr = getFile( "./assets/triangle.fs" );
  
  const GLchar* vsSrc;
  const GLchar* fsSrc;
  vsSrc = vsSrcStr.c_str();
  fsSrc = fsSrcStr.c_str();
  
  // load the vertex / fragment shaders
  vs = loadShader( vsSrc, GL_VERTEX_SHADER );
  fs = loadShader( fsSrc, GL_FRAGMENT_SHADER );
  
  GLuint programID = glCreateProgram();
  
  if( programID == 0 ) {
    std::cout << "No programID" << std::endl;
    return 0;
  }
  
  glAttachShader( programID, vs );
  glAttachShader( programID, fs );
  
  glLinkProgram( programID );
  
  checkShaderError( programID, GL_LINK_STATUS, true, "Error linking shader program" );
  
  positionID_ = glGetAttribLocation( programID,  "aPosition" );
  colourID_   = glGetAttribLocation( programID,  "aColour" );
  mvpID_      = glGetUniformLocation( programID, "uMVP" );
  
  return programID;
}
