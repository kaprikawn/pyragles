#include "glWindow.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include "game.hpp"

const GLfloat X_DELTA               = 0.1f;
const uint    NUM_VERTICES_PER_TRI  = 3;
const uint    NUM_FLOATS_PER_VERTEX = 6;
const uint    TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTEX * sizeof( GLfloat );
const uint  MAX_TRIS = 20;

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

GLuint load( const char* shaderSrc, GLenum type ) {
  
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

int GlWindow::init() {

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
  programID_ = glCreateProgram();
  
  if( programID_ == 0 ) {
    return 0;
  }
  
  glAttachShader( programID_, vs );
  glAttachShader( programID_, fs );
  
  // link the program
  glLinkProgram( programID_ );
  
  // check the link status
  checkShaderError( programID_, GL_LINK_STATUS, true, "Error linking shader program" );
  
  glClearColor( 0.0f, 0.0f, 0.4f, 1.0f );
  
  Vertex myTri[] = {
      glm::vec3( -0.5f,  1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) // vertex0
    , glm::vec3( -1.0f, -1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
    , glm::vec3(  0.0f, -1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
    
    , glm::vec3( -0.5f,  1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
    , glm::vec3(  0.0f, -1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
    , glm::vec3(  0.5f,  1.0f,  0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
  };
  
  memcpy( myTri_, myTri, sizeof( myTri ) );
  
  // set the viewport
  glViewport( 0, 0, windowWidth, windowHeight );
  
  // use the program object
  glUseProgram( programID_ );
  
  positionID_ = glGetAttribLocation( programID_, "aPosition" );
  
  // load the vertex data
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), myTri_ );
  glEnableVertexAttribArray( positionID_ );
  
  return 0;
}

void GlWindow::update( float dt ) {
  
}

void GlWindow::render() {
  
  glClear( GL_COLOR_BUFFER_BIT );
  
  glDrawArrays( GL_TRIANGLES, 0, 6 );
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}
