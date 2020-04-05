#include "shader.hpp"
#include <sstream>
#include <fstream>
#include <GLES2/gl2.h>
#include <iostream>
#include "glCallLog.hpp"
#include "shaderCache.hpp"

Shader::Shader() {
  
}

void Shader::init( const std::string& filename ) {
  
  std::string filepath = "./assets/" + filename;
  ShaderProgramSource source = parseShader( filepath );
  
  rendererID_ = createShader( source.vertexSource, source.fragmentSource );
}

ShaderProgramSource Shader::parseShader( const std::string& filepath ) {
  
  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  
  std::fstream stream( filepath );
  std::stringstream ss[ 3 ];
  ShaderType type = ShaderType::NONE;
  std::string line;
  while( getline( stream, line ) ) {
    if( line.find( "#shader" ) != std::string::npos ) {
      if( line.find( "vertex" ) != std::string::npos )
        type = ShaderType::VERTEX;
      else if( line.find( "fragment" ) != std::string::npos )
        type = ShaderType::FRAGMENT;
    } else {
      ss[ ( int )type ] << line << "\n";
    }
  }
  
  ShaderProgramSource mySource;
  mySource.vertexSource   = ss[0].str();
  mySource.fragmentSource = ss[1].str();
  
  return mySource;
}

unsigned int Shader::compileShader( unsigned int type, const std::string& source ) {
  unsigned int id = glCreateShader( type );
  const char* src = source.c_str();
  glShaderSource( id, 1, &src, nullptr );
  glCompileShader( id );
  
  int result;
  glGetShaderiv( id, GL_COMPILE_STATUS, &result );
  if( result == GL_FALSE ) {
    int length;
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
    char* message = ( char* )alloca( length * sizeof( char ) );
    glGetShaderInfoLog( id, length, &length, message );
    std::cout << "Failed to compile " << ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << " shader : " << std::endl << message << std::endl;
    glDeleteShader( id );
    return 0;
  }
  
  return id;
}

unsigned int Shader::createShader( const std::string& vertexShader, const std::string fragmentShader ) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader( GL_VERTEX_SHADER, vertexShader );
  unsigned int fs = compileShader( GL_FRAGMENT_SHADER, fragmentShader );
  
  glAttachShader( program, vs );
  glAttachShader( program, fs );
    
  glLinkProgram( program );
  glValidateProgram( program );
  
  glDeleteShader( vs );
  glDeleteShader( fs );
  
  return program;
}

void Shader::bind() const {
  glUseProgram( rendererID_ );
}

void Shader::unbind() const {
  glUseProgram( 0 );
}

void Shader::setUniform4f( const std::string& name, glm::vec4 values ) {
  GLCall( glUniform4f( getUniformLocation( name ), values.x, values.y, values.z, values.a ) );
}

void Shader::setUniform4fv( const std::string& name, const float* mat4 ) {
  GLCall( glUniformMatrix4fv( getUniformLocation( name ), 1, GL_FALSE, mat4 ) );
}

void Shader::setUniform1i( const std::string& name, int value ) {
  GLCall( glUniform1i( getUniformLocation( name ), value ) );
}

void Shader::setUniform3f( const std::string& name, float x, float y, float z ) {
  GLCall( glUniform3f( getUniformLocation( name ), x, y, z ) );
}

int Shader::getUniformLocation( const std::string& name ) {
  
  if( uniformLocationCache_.find( name ) != uniformLocationCache_.end() )
    return uniformLocationCache_[ name ];
    
  int location = glGetUniformLocation( rendererID_, name.c_str() );
  
  if( location == -1 )
    std::cout << "Warning : uniform " << name << " doesn't exist\n";
  
  uniformLocationCache_[ name ] = location;
    
  return location;
}

Shader::~Shader() {
  glDeleteProgram( rendererID_ );
}
