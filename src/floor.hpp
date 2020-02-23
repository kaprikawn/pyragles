#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "gameObject.hpp"

class Floor : public GameObject {
  
  private :
    
  public :
    Floor();
    ~Floor();
    
    //virtual bool init( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
  
};
    
#endif //FLOOR_HPP
