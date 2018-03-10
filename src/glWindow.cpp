#include "glWindow.hpp"
#include <fstream>
#include <iostream>
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
  checkShaderError( programID_, GL_LINK_STATUS, true, "Error linking shader program");
  
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  
  positionID_ = glGetAttribLocation( programID_, "aPosition" );
  colourID_   = glGetAttribLocation( programID_, "aColour" );
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, MAX_TRIS * TRIANGLE_BYTE_SIZE, NULL, GL_STATIC_DRAW );
  glEnableVertexAttribArray( positionID_ );
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 6, (GLvoid*)0 );
  
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 6, ( char* )( sizeof( GLfloat ) * 3 ) );
glEnableVertexAttribArray( colourID_ );
  
  glViewport( 0, 0, windowWidth, windowHeight ); // set the viewport
  
  return 0;
}

void GlWindow::sendAnotherTriToOpenGL() {
  
  if( numTris_ == MAX_TRIS ) {
    return;
  }
  
  const GLfloat THIS_TRI_X = -1.0f + numTris_ * X_DELTA;
  
  GLfloat thisTri[] = {
      THIS_TRI_X          ,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f
    , THIS_TRI_X + X_DELTA,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f
    , THIS_TRI_X          ,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f
  };
  
  //std::cout << "this tri x is " << THIS_TRI_X << std::endl;
  
  glBufferSubData( GL_ARRAY_BUFFER, numTris_ * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri );
  numTris_++;
}

void GlWindow::update( float dt ) {
  
  // use the program object
  glUseProgram( programID_ );
	
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  sendAnotherTriToOpenGL();
  
  glDrawArrays( GL_TRIANGLES, ( numTris_ - 1 ) * NUM_VERTICES_PER_TRI, NUM_VERTICES_PER_TRI );
  //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );
}

void GlWindow::render() {
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}
