#include "shader.hpp"
#include <fstream>
#include <iostream>
#include "game.hpp"

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

GLuint Shader::load( const char* shaderSrc, GLenum type ) {
  
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

int Shader::init() {
  
  GLuint  vs;
  GLuint  fs;
  
  std::string vsSrcStr = getFile( "./shaders/triangle.vs" );
  std::string fsSrcStr = getFile( "./shaders/triangle.fs" );
  
  const GLchar* vsSrc;
  const GLchar* fsSrc;
  vsSrc = vsSrcStr.c_str();
  fsSrc = fsSrcStr.c_str();
  
  // load the vertex / fragment shaders
  vs = load( vsSrc, GL_VERTEX_SHADER );
  fs = load( fsSrc, GL_FRAGMENT_SHADER );
  
  // create the program object
  program_ = glCreateProgram();
  
  if( program_ == 0 ) {
    return 0;
  }
  
  glAttachShader( program_, vs );
  glAttachShader( program_, fs );
  
  // bind vPosition to attribute 0
  glBindAttribLocation( program_, 0, "vPosition" );
  
  // link the program
  glLinkProgram( program_ );
  
  // check the link status
  checkShaderError( program_, GL_LINK_STATUS, true, "Error linking shader program");
  
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  
  return 0;
}

void Shader::render() {
  GLfloat vVertices[] = {   0.0f, 0.5f, 0.0f
                          , -0.5f, -0.5f, 0.0f
                          , 0.5f, -0.5f, 0.0f };
  
  // set the viewport
  glViewport( 0, 0, 1280, 720 );
  
  // clear the colour buffer
  glClear( GL_COLOR_BUFFER_BIT );
  
  // use the program object
  glUseProgram( program_ );
  
  // load the vertex data
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
  glEnableVertexAttribArray( 0 );
  
  glDrawArrays( GL_TRIANGLES, 0, 3 );
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
  
  
}
