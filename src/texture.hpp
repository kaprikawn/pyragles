#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
  
  private :
    unsigned int    rendererID_;
    std::string     filepath_;
    unsigned char*  localBuffer_;
    
  public :
    Texture();
    ~Texture();
    
    void init( unsigned char* textureData, int width, int height );
    void bind( unsigned int slot = 0 ) const;
    void unbind() const;
    void initFromPNG( std::string filename );
    
};
    
#endif //TEXTURE_HPP