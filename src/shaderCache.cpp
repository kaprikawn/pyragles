#include "shaderCache.hpp"
#include <iostream>

ShaderCacheClass* ShaderCacheClass::instance_ = 0;

int ShaderCacheClass::getRendererID( std::string shaderFilename ) {
  it_ = shaderMap_.find( shaderFilename );
  if( it_ != shaderMap_.end() ) {
    return shaderMap_.find( shaderFilename ) -> second;
  }
  return -1;
}

void ShaderCacheClass::addRendererID( std::string shaderFilename, unsigned int rendererID ) {
  shaderMap_[ shaderFilename ] = rendererID;
}
