#ifndef PROJECTILEMANAGER_HPP
#define PROJECTILEMANAGER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "projectile.hpp"

class ProjectileManager {
  
  private :
  
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    
  public :
    ProjectileManager();
    ~ProjectileManager();
    
    void update( float dt );
    void render( glm::mat4 viewProjectionMatrix );
    
    void spawnProjectile( ProjectileParams projectileParams );
    
};
    
#endif //PROJECTILEMANAGER_HPP
