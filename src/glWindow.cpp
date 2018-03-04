#include "glWindow.hpp"
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
  programObject_ = glCreateProgram();
  
  if( programObject_ == 0 ) {
    std::cout << "No projectObject found" << std::endl;
    return 0;
  }
  
  std::cout << "programObject is " << programObject_ << std::endl;
  
  glAttachShader( programObject_, vs );
  glAttachShader( programObject_, fs );
  
  // link the program
  glLinkProgram( programObject_ );
  
  // check the link status
  checkShaderError( programObject_, GL_LINK_STATUS, true, "Error linking shader program");
  
  glClearColor( 0.0f, 0.0f, 0.4f, 1.0f );
  
  vertices_ = {
	//    x      y      z     r     g     b     a
      -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f // front top left
    , -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f // front bottom left
    ,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f // front top right
    ,  1.0f, -1.0f,  1.0f, 0.0f, 0.9f, 0.9f, 1.0f // front bottom right
    
    ,  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // back top right
    ,  1.0f, -1.0f, -1.0f, 0.9f, 0.9f, 0.0f, 1.0f // back bottom right
    , -1.0f,  1.0f, -1.0f, 0.9f, 0.5f, 0.0f, 1.0f // back top left
    , -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // back bottom left
	};
	
	glGenBuffers( 2, vboIDs_ );
	
	glBindBuffer( GL_ARRAY_BUFFER, vboIDs_[0] );
  glBufferData( GL_ARRAY_BUFFER, vertices_.size() * sizeof( GLfloat ), &vertices_[0], GL_STATIC_DRAW );
  
  indices_ = {
      0, 1, 2 // front A
    , 2, 1, 3 // front B
    , 4, 2, 3 // right A
    , 4, 3, 5 // right B
    , 4, 5, 6 // back A
    , 6, 5, 7 // back B
    , 7, 0, 6 // left A
    , 0, 7, 1 // left B
    , 5, 3, 7 // bottom A
    , 7, 3, 0 // bottom B
    , 2, 4, 0 // top A
    , 0, 4, 6 // top B
  };
    
  //glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboIDs_[1] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof( indices_[0] ), &indices_[0], GL_STATIC_DRAW );
  
  positionID_ = glGetAttribLocation( programObject_, "a_position" );
  colourID_   = glGetAttribLocation( programObject_, "a_colour" );
  mvpID_      = glGetUniformLocation( programObject_, "u_mvpMatrix" );
  
  projection_ = glm::perspective( glm::radians( 45.0f ), windowWidth / windowHeight, 0.1f, 100.0f );
  
  view_ = glm::lookAt(
      glm::vec3( 4, 3, 3 )
    , glm::vec3( 0, 0, 0 )
    , glm::vec3( 0, 1, 0 )
  );
  
  model_ = glm::mat4( 1.0f );
  
  mvp_ = projection_ * view_ * model_;
  
  // set the viewport
  glViewport( 0, 0, windowWidth, windowHeight );
  
  glEnable( GL_DEPTH_TEST );
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc( GL_LESS );
  
  return 0;
}

void GlWindow::update( float dt ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  glUseProgram( programObject_ );
  
  // compute the MVP matrix...goes here
  
  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp_[0][0] );
	
	// load the vertex data
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, (GLvoid*)0 );
  glEnableVertexAttribArray( positionID_ );
  
  // load the colour data
  glVertexAttribPointer( colourID_, 4, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, ( char* )( sizeof( GLfloat ) * 3 ) );
  glEnableVertexAttribArray( colourID_ );
}

void GlWindow::render() {
  
  // clear the colour buffer
  glClear( GL_COLOR_BUFFER_BIT );
  
  glDrawElements( GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0 );
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
  
}
