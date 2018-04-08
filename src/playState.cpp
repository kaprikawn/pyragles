#include "playState.hpp"
#include <iostream>
#include <vector>
#include "game.hpp"
#include "shader.hpp"
#include "projectile.hpp"

const std::string PlayState::s_playID = "PLAY";

void PlayState::addGlObject( std::shared_ptr<GlObject> glObject, bool init, bool isLoading ) {
  
  if( isLoading ) {
    vertexBufferSize_ += glObject -> vertexBufferSize();
    indexBufferSize_  += glObject -> indexBufferSize();
    
    levelObjects_.push_back( glObject );
  }
  
  if( init )
    liveObjects_.push_back( glObject );
  
}

bool PlayState::onEnter() {
  
  std::unique_ptr<Shader> shader = std::make_unique<Shader>();
  programID_ = shader -> init();
  
  if( programID_ == 0 )
    return false;
    
  glUseProgram( programID_ );
  
  heroPosition_ = std::make_shared<glm::vec3>();
  target_ = std::make_shared<Target>( TARGET, programID_, heroPosition_ );
  hero_   = std::make_shared<Hero>( SHIP, programID_, target_, heroPosition_ );
  
  PlayState::addGlObject( hero_   , true, true );
  PlayState::addGlObject( target_ , true, true );
  
  PlayState::addGlObject( std::make_shared<Projectile>( BULLET, programID_, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ) ), false, true );
  
  glBuffer_ = std::make_unique<GlBuffer>( vertexBufferSize_, indexBufferSize_ );
  
  for( unsigned int i = 0; i < levelObjects_.size(); i++ ) {
    
    GLsizeiptr vertexOffset = glBuffer_ -> addVertexBufferData ( 
        levelObjects_[i] -> vertexBufferSize()
      , levelObjects_[i] -> vertexDataPointer()
    ); // add the vertex data to the buffer
    
    GLsizeiptr indexOffset = glBuffer_ -> addIndexBufferData (
        levelObjects_[i] -> indexBufferSize()
      , levelObjects_[i] -> indexDataPointer()
    );
    
    levelObjects_[i] -> setOffsetLocations( vertexOffset, indexOffset );
    
    int shapeType = levelObjects_[i] -> shapeType();
    
    vertexOffsets_[ shapeType ] = vertexOffset;
    indexOffsets_[ shapeType ]  = indexOffset;
  }
  
  levelStart_ = SDL_GetTicks();
  
  glClearColor( 0.0f, 0.0f, 0.4f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  glViewport( 0, 0, windowWidth, windowHeight );
  
  return true;
}

void PlayState::gameLogic( float dt ) {
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // delete stuff that needs deleting
  for( unsigned i = liveObjects_.size(); i-- > 0; ) {
    if( liveObjects_[i] -> deleteObject() ) {
      liveObjects_[i] -> clean();
      liveObjects_.erase( liveObjects_.begin() + i );
    }
  }
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ ) {
    liveObjects_[i] -> update( dt );
  }
  
  liveObjects_[0] -> calculateRotation( dt ); // calculate rotation of hero
  
  if( liveObjects_[0] -> fire() ) {
    std::shared_ptr<Projectile> newBullet =
      std::make_shared<Projectile>( BULLET, programID_, hero_ -> coordinates(), target_ -> coordinates() );
    
    newBullet -> setOffsetLocations( vertexOffsets_[ BULLET ], indexOffsets_[ BULLET ] );
    PlayState::addGlObject( newBullet, true, false );
  }
  
}

void PlayState::update( float dt ) {
   
   PlayState::gameLogic( dt );
   
}

void PlayState::render() {
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ ) {
    liveObjects_[i] -> render();
  }
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}

bool PlayState::onExit() {
  //delete hero_;
  //delete target_;
  return true;
}
