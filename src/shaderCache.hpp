#ifndef SHADERCACHE_HPP
#define SHADERCACHE_HPP

#include <map>
#include <string>

class ShaderCacheClass {
  
  private :
  
    std::map<std::string, unsigned int> shaderMap_;
    std::map<std::string, unsigned int>::iterator it_;
    
    ShaderCacheClass  (){}
    ~ShaderCacheClass (){}
    static ShaderCacheClass* instance_;
    
  public :
    
    int getRendererID( std::string shaderFilename );
    void addRendererID( std::string shaderFilename, unsigned int rendererID );
    
    static ShaderCacheClass* Instance() {
      if( instance_ == 0 ) {
        instance_ = new ShaderCacheClass();
      }
      return instance_;
    }
};

typedef ShaderCacheClass ShaderCache;
    
#endif //SHADERCACHE_HPP
