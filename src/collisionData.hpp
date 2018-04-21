#ifndef COLLISIONDATA_HPP
#define COLLISIONDATA_HPP

#include <memory>

struct CollisionProperties {
  unsigned int objectID       = 0;
  unsigned int damageToShip   = 0;
  unsigned int damageToEnemy  = 0;
};

class CollisionData {
  
  private:
    
    bool isColliding_ = false;
    
    CollisionProperties a;
    CollisionProperties b;
    
  public:
    
    CollisionData(){}
    ~CollisionData(){}
    
    bool isColliding() { return isColliding_; }
    
    void setCollisionData() {
      isColliding_  = true;
    }
    
};

#endif //COLLISIONDATA_HPP