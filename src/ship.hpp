#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"

class Ship : public GameObject {
  
  private :
    
  public :
    Ship();
    ~Ship();
    
  virtual bool init( std::string modelName, std::shared_ptr<Camera> camera );
  virtual void update( float dt );
  virtual void render( glm::mat4 viewProjectionMatrix );
  
  virtual void loadVertexData( const void* data, unsigned int size );
  virtual void loadIndexData( const unsigned int* data, unsigned int count );
  virtual void loadShader( const std::string& filename );
  
};
    
#endif //SHIP_HPP
