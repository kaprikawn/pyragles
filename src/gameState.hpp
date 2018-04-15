#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include "glObject.hpp"
#include "structs.hpp"

class GameState {
  protected:
    GameState() {}
    
    std::vector<std::shared_ptr<GlObject>> liveObjects_;
    std::vector<std::shared_ptr<GlObject>> levelObjects_;
    
    GLsizeiptr vertexOffsets_[ MAX_SHAPE ];
    GLsizeiptr indexOffsets_[ MAX_SHAPE ];
    
  public:
    virtual ~GameState() {}
    
    virtual bool onEnter() = 0;
    virtual void update ( float dt ) = 0;
    virtual void render () = 0;
    virtual bool onExit () = 0;
    virtual std::string getStateID() const = 0;
};

#endif //GAMESTATE_HPP
