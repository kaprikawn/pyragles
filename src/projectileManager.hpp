#ifndef PROJECTILEMANAGER_HPP
#define PROJECTILEMANAGER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "projectile.hpp"

class ProjectileManager {
  
  private :
  
    std::vector<std::unique_ptr<Projectile>>  projectiles_;
    std::vector<std::vector<glm::vec4>>       colliders_;
    std::vector<unsigned int>                 projectilePositions_;
    
  public :
    ProjectileManager();
    ~ProjectileManager();
    
    void update( float dt );
    void render( glm::mat4 viewProjectionMatrix );
    
    void spawnProjectile( ProjectileParams projectileParams );
    void updateColliers();
    void registerCollision( unsigned int colliderIndex );
    
    std::vector<std::vector<glm::vec4>> colliers() { return colliders_; }
};
    
#endif //PROJECTILEMANAGER_HPP
