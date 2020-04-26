#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "gameObject.hpp"

class Enemy : public GameObject {
  
  private :
    
  public :
    Enemy();
    ~Enemy();
    
    bool init( std::string modelFilename );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    virtual void registerCollision();
    
};
    
#endif //ENEMY_HPP
