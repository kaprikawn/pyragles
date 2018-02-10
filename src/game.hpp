#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include "inputHandler.hpp"
#include "shader.hpp"

#define windowWidth 1280
#define windowHeight 720

class Game {
  private:
    SDL_Window*     window_;
    SDL_Renderer*   renderer_;
    
    bool          running_;
    static Game*  instance_;
    
    Shader*       shader;
    
    Game(){}
    
  public:
    ~Game(){}
    
    bool init( const char* title, int xpos, int ypos, int width, int height, int flags );
    void handleInputs();
    void update();
    void render();
    void clean();
    void quit();
    
    bool gameRunning() { return running_; }
    
    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window*   getWindow()   const { return window_; }
    
    
    static Game* Instance() {
      if( instance_ == 0 ) {
        instance_ = new Game();
      }
      return instance_;
    }
};

typedef Game TheGame;

#endif //GAME_HPP
