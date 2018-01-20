#include "shader.hpp"
#include <fstream>
#include <iostream>
#include "game.hpp"

Shader::Shader( const std::string& filename ) {
  
  std::string vs = load( filename + ".vs" );
  std::string fs = load( filename + ".fs" );
  
  program_  = glCreateProgram();
  
  vs_ = create( load( filename + ".vs" ), GL_VERTEX_SHADER );
  fs_ = create( load( filename + ".fs" ), GL_FRAGMENT_SHADER );
  
  glAttachShader( program_, vs_ );
  glAttachShader( program_, fs_ );
  
  glLinkProgram( program_ );
  glUseProgram( program_ );
  
  u_time_loc = glGetUniformLocation( program_, "u_time" );
  u_time = 0.0f;
	
	// create vbo
	GLuint vbo;
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	float vertex_data[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertex_data ), vertex_data, GL_STATIC_DRAW );
 
	// setup vertex attribs
	GLuint va_position = 0;
	glEnableVertexAttribArray( va_position );
	glVertexAttribPointer( va_position, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
 
	glClearColor( 0.4, 0.6, 0.8, 1.0 );
	
}

void Shader::render() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glUniform1f( u_time_loc, u_time += 1.0f / 60.0f );
  glDrawArrays( GL_TRIANGLES, 0, 3 );
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}

void Shader::bind() {
  glUseProgram( program_ );
}

std::string Shader::load( const std::string& filename ) {
  
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

GLuint Shader::create( const std::string& text, unsigned int type ) {

  GLuint shader = glCreateShader( type );
  
  const GLchar* p[1];
  p[0] = text.c_str();
  GLint lengths[1];
  lengths[0] = text.length();
    
  glShaderSource( shader, 1, p, lengths );
  
  glCompileShader( shader );
  glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled_ );
  
  return shader;
}

void Shader::checkShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage ) {

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
