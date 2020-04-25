#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "gameObject.hpp"

struct ProjectileParams {
  glm::vec4 startingPostion     = glm::vec4( 0.0f );
  glm::vec4 destinationPosition = glm::vec4( 0.0f );
  float speed                   = 1.0f;
};

class Projectile : public GameObject {
  
  private :
    
    bool active_ = false;
    glm::vec4 startingPosition_     = glm::vec4( 0.0f );
    glm::vec4 destinationPosition_  = glm::vec4( 0.0f );
    
  public :
    Projectile( ProjectileParams params );
    ~Projectile();
    
    bool init();
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    bool active() { return active_; }
    void activate( ProjectileParams params );
    void calculateRotation();
    
};
    
#endif //PROJECTILE_HPP
