#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <glm/glm.hpp>
#include <vector>

struct AABB {
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;
  bool firstPass = true;
};

class Collision {
  
  private :
    bool areColliding_ = false;
    
  public :
    Collision( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 );
    ~Collision();
    
    bool areColliding() { return areColliding_; }
    
    
};
    
#endif //COLLISION_HPP
