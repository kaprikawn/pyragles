#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

struct ShaderProgramSource {
  std::string vertexSource    = "";
  std::string fragmentSource  = "";
};

class Shader {
  private :
    
    std::string   filepath_;
    unsigned int  rendererID_;
    std::unordered_map<std::string, int> uniformLocationCache_;
    
    std::vector<std::string> uniformNames_;
  
  public :
    Shader();
    ~Shader();
    
    void init( const std::string& filename );
    void bind() const;
    void unbind() const;
    void setUniform4f( const std::string& name, glm::vec4 values );
    void setUniform4fv( const std::string& name, const float* mat4 );
    void setUniform1i( const std::string& name, int value );
    
    unsigned int rendererID(){ return rendererID_; }
    
  private:
    
    int getUniformLocation( const std::string& name );
    
    ShaderProgramSource parseShader( const std::string& filepath );
    unsigned int        createShader( const std::string& vertexShader, const std::string fragmentShader );
    unsigned int        compileShader( unsigned int type, const std::string& source );
    
};
    
#endif //SHADER_HPP