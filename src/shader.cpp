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
  programObject_ = glCreateProgram();
  
  if( programObject_ == 0 ) {
    return 0;
  }
  
  glAttachShader( programObject_, vs );
  glAttachShader( programObject_, fs );
  
  // link the program
  glLinkProgram( programObject_ );
  
  // check the link status
  checkShaderError( programObject_, GL_LINK_STATUS, true, "Error linking shader program");
  
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  
  vertices_ = {
      -0.5f,  0.5f, 0.0f // top left
    , -0.5f, -0.5f, 0.0f // bottom left
    ,  0.5f, -0.5f, 0.0f // bottom right
    
    ,  0.5f,  0.5f, 0.0f // top right
    , -0.5f,  0.5f, 0.5f // top left
    ,  0.5f, -0.5f, 0.0f // bottom right
  };
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, vertices_.size() * sizeof( GLfloat ), &vertices_[0], GL_STATIC_DRAW );
  
  
  indices_ = {
      0, 1, 2
    , 2, 0, 3
  };
    
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof( indices_[0] ), &indices_[0], GL_STATIC_DRAW );
  
  // use the program object
  glUseProgram( programObject_ );
  
  positionLoc_    = glGetAttribLocation( programObject_, "position" );
  
  mvpLoc_         = glGetUniformLocation( programObject_, "mvpMatrix" );
  
  // load the vertex data
  glVertexAttribPointer( positionLoc_, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0 );
  glEnableVertexAttribArray( positionLoc_ );
  
  // set the viewport
  glViewport( 0, 0, windowWidth, windowHeight );
  
  return 0;
}

void Shader::update( float dt ) {

}

void Shader::render() {
  
  // clear the colour buffer
  glClear( GL_COLOR_BUFFER_BIT );
  
  //glDrawArrays( GL_TRIANGLES, 0, 6 );
  glDrawElements( GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0 );
  //glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices_ );
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
  
  
}
