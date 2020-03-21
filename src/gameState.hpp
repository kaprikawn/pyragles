#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <memory>
#include <GLES2/gl2.h>
#include <vector>

class PhysicsObject;

class GameState {
  protected:
    GameState() {}
    
    std::vector<std::shared_ptr<PhysicsObject>> liveObjects_;
    std::vector<std::shared_ptr<PhysicsObject>> levelObjects_;
    
  public:
    virtual ~GameState() {}
    
    virtual bool onEnter( int levelNumber ) = 0;
    virtual void update ( GLfloat dt ) = 0;
    virtual void render () = 0;
    virtual bool onExit () = 0;
    virtual std::string getStateID() const = 0;
    
    virtual int nextLevel() = 0;
    
};

#endif //GAMESTATE_HPP