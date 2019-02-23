#include "playState.hpp"
#include <iostream>
#include "shader.hpp"
#include "global.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera ) {
  
  std::shared_ptr<Shader> shader = std::make_shared<Shader>();
  
  viewProjectionMatrix_ = camera -> viewProjectionMatrix();
  
  GLuint programID = shader -> init();
  
  if( programID == 0 )
    return false;
  
  meshLoader_ = std::make_shared<MeshLoader>();
  meshLoader_ -> generateMeshes();
  
  renderer_ = std::make_shared<Renderer>( programID, camera );
  renderer_   -> generateBuffer( meshLoader_ -> totalVertexBufferSize(), meshLoader_ -> totalIndexBufferSize() );
  renderer_   -> addBufferData( meshLoader_ );
  camera_   = camera;
  
  Collision collision;
  collision_  = collision;
  audio_ = std::make_shared<Audio>();
  
  shipPosition_           = std::make_shared<glm::vec3>();
  GLfloat shipStartZ      = START_Z - 7.0f;
  GLfloat targetDistance  = 15.0f;
  
  int shapeType = TARGET;
  PhysicsObjectParams params;
  params.shapeType    = shapeType;
  params.objectType   = FRIENDLY;
  params.initPosition = { -3, START_Y, shipStartZ - targetDistance };
  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.inputHandler = inputHandler;
  params.shipPosition = shipPosition_;
  
  target_ = std::make_shared<Target>( params );
  addPhysicsObject( target_, true, false );
  params = {};
  
  shapeType = SHIP;
  params.shapeType    = shapeType;
  params.objectType   = FRIENDLY;
  params.initPosition = { 0, START_Y, shipStartZ };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.inputHandler = inputHandler;
  params.shipPosition = shipPosition_;
  
  ship_ = std::make_shared<Ship>( params, target_ );
  addPhysicsObject( ship_, true, false );
  params = {};
  
  shapeType           = ENEMY_POD;
  params.shapeType    = shapeType;
  params.objectType   = ENEMY;
  params.initPosition = { 40.0f, 5, START_Z - 20 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.canFire      = true;
  
  addPhysicsObject( std::make_shared<Enemy>( params ), true, false );
  params = {};
  
  shapeType = FLOOR1;
  params.shapeType    = shapeType;
  params.objectType   = SCENARY;
  params.initPosition = { 0, FLOOR_Y, -10 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  
  addPhysicsObject( std::make_shared<Floor>( params, params.shapeType ), true, false );
  params = {};
  
  shapeType = FLOOR2;
  params.shapeType    = shapeType;
  params.objectType   = SCENARY;
  params.initPosition = { 0, FLOOR_Y - 0.02, -10 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  addPhysicsObject( std::make_shared<Floor>( params, params.shapeType ), true, false );
  
  audio_ -> load( "assets/musicLevel01_organic_to_synthetic7.wav" );
  audio_ -> play();
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  // delete stuff that needs deleting
  for( unsigned i = liveObjects_.size(); i-- > 0; ) {
    if( liveObjects_[i] -> deleteObject() ) {
      liveObjects_[i]   -> clean();
      liveObjects_.erase( liveObjects_.begin() + i );
    }
  }
  
  // spawn anything that needs spawning
  for( unsigned i = levelObjects_.size(); i-- > 0; ) {
    levelObjects_[ i ] -> reduceSpawnCounter( dt );
    GLfloat timeUntilSpawn = levelObjects_[ i ] -> timeUntilSpawn();
    if( timeUntilSpawn <= 0.0f ) {
      addPhysicsObject( std::move( levelObjects_[ i ] ), true, false );
      levelObjects_.erase( levelObjects_.begin() + i );
    }
  }
  
  if( liveObjects_[ 1 ] -> fire() ) { // fire bullets
    
    PhysicsObjectParams params;
    params.shapeType      = BULLET;
    params.initPosition   = { shipPosition_ -> x, shipPosition_ -> y, shipPosition_ -> z };
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
    params.bufferData     = meshLoader_ -> bufferData( params.shapeType );
    params.mesh           = mesh;
    params.renderer       = renderer_;
    params.shipPosition   = shipPosition_;
    params.damageEnemy    = true;
    params.velMultiplier  = 100.0f;
    
    std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>( params, target_ -> position(), false, liveObjects_[ 1 ] -> objectID() );
    PlayState::addPhysicsObject( newProjectile, true, false );
  }
  
  if( liveObjects_.size() > 1 ) {
    for( unsigned int i = 2; i < liveObjects_.size(); i++ ) {
      if( !liveObjects_[ i ] -> canFire() )
        continue;
        
      if( liveObjects_[ i ] -> fire() ) {
        glm::vec3 enemyPosition = liveObjects_[ i ] -> position();
        
        PhysicsObjectParams params;
        params.shapeType        = BULLET;
        params.initPosition     = enemyPosition;
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
        params.bufferData       = meshLoader_ -> bufferData( params.shapeType );
        params.mesh             = mesh;
        params.renderer         = renderer_;
        params.shipPosition     = shipPosition_;
        params.damageShip       = true;
        params.velMultiplier    = 10.0f;
        unsigned int spawnerID  = liveObjects_[ i ] -> objectID();
        params.spawnerID        = spawnerID;
        
        glm::vec3 projectileTarget = { shipPosition_ -> x, shipPosition_ -> y, shipPosition_ -> z };
        
        std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>( params, projectileTarget, false, spawnerID );
        PlayState::addPhysicsObject( newProjectile, true, false );
      }
    }
  }
  
  // collisions - starting at 1 because target collision is irrelevent
  for( unsigned int i = 1; i < liveObjects_.size(); i++ ) {
    for( unsigned int j = i + 1; j < liveObjects_.size(); j++ ) {
      
      // for a bullet if they're touching the ship that spawned it
      if( liveObjects_[ i ] -> objectID() == liveObjects_[ j ] -> spawnerID() )
        continue;
        
      CollisionData collisionData = collision_.collisionData( liveObjects_[ i ], liveObjects_[ j ] );
      
      if( collisionData.isColliding() ) {
        
        CollisionProperties collisionProperties = liveObjects_[ i ] -> collisionProperties();
        liveObjects_[ j ] -> registerCollision( collisionData, collisionProperties );
        
        collisionProperties = liveObjects_[ j ] -> collisionProperties();
        liveObjects_[ i ] -> registerCollision( collisionData, collisionProperties );
      }
    }
  }
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ ) {
    liveObjects_[ i ] -> update( dt );
  }
  
  ship_ -> calculateRotation( dt );
  
  camera_ -> update( shipPosition_, dt );
  viewProjectionMatrix_ = camera_ -> viewProjectionMatrix();
  
}

void PlayState::render() {
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ )
    liveObjects_[ i ] -> render( viewProjectionMatrix_ );
}

void PlayState::addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading ) {
  
  physicsObject -> setObjectID( nextObjectID_++ );
  
  if( isLoading )
    levelObjects_.push_back( physicsObject );
  
  if( init )
    liveObjects_.push_back( physicsObject );
}

bool PlayState::onExit() {
  
  return true;
}