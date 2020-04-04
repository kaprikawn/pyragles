#include "texture.hpp"
#include <GLES2/gl2.h>
#include <iostream>
#include "glCallLog.hpp"
#include "../deps/stb_image.h"

Texture::Texture() {
  
}

void Texture::init( unsigned char* textureData, int width, int height ) {
  
  GLCall( glGenTextures( 1, &rendererID_ ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, rendererID_ ) );
  
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
  
  // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  
  GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

void Texture::initFromPNG( std::string filename ) {
  
  //stbi_set_flip_vertically_on_load( 1 );

  std::string filepath = "./assets/" + filename;
  
  int width, height, bpp;
  unsigned char* localBuffer = stbi_load( filepath.c_str(), &width, &height, &bpp, 4 );
  
  if( localBuffer == nullptr )
    std::cout << "failed to load png\n";
  
  GLCall( glGenTextures( 1, &rendererID_ ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, rendererID_ ) );
  
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
  
  // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  
  //glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  
  GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

void Texture::bind( unsigned int slot /* = 0 */ ) const {
  GLCall( glActiveTexture( GL_TEXTURE0 + slot ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, rendererID_ ) );
}

void Texture::unbind() const {
  GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

Texture::~Texture() {
  GLCall( glDeleteTextures( 1, &rendererID_ ) );
}
