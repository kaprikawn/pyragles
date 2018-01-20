#ifndef SHADER_HPP
#define SHADER_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>

class Shader {
  private:
  
    static const unsigned int NUM_SHADERS   = 2;
    static const unsigned int NUM_UNIFORMS  = 3;
    //void operator=( const Shader& shader ) {}
    //Shader( const std::string& filename ) {}
    
    std::string load( const std::string& filename );
    void checkShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage );
    GLuint create( const std::string& text, unsigned int type );
    
    GLuint  program_;
    GLuint  shaders_[ NUM_SHADERS ];
    GLuint  uniforms_[ NUM_UNIFORMS ];
    
    GLuint  vs_;
    GLuint  fs_;
    GLint   isCompiled_;
    
    GLuint  u_time_loc;
    float   u_time;
    
  public:
    Shader( const std::string& filename );
    
    void bind();
    void render();
    
};

#endif //SHADER_HPP
