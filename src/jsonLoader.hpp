#ifndef JSONLOADER_HPP
#define JSONLOADER_HPP

#include <vector>
#include "physicsObject.hpp"

class JsonLoader {
  
  private:
    
    std::vector<PhysicsObjectParams> params_;
    
  public:
    void loadLevel( int levelNumber );
};

#endif //JSONLOADER_HPP