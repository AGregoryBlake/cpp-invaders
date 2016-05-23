#include <algorithm>
#include <iostream>
#include <SDL2/SDL.h>
#include "world.h"

#define SCREEN_HEIGHT 500
#define SCREEN_WIDTH 500

#define SHIP_HEIGHT 15
#define SHIP_WIDTH 25
#define INVADER_SIDE 20
#define BULLET_SIDE 4

class App { // The App class handles the application runtime.
  public:
    int runApp() {
      if(start() == 1) {
        return 1;
      };
      enterLoop();
      shutdown();
      return 0;
    }

  private:  
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    World* _world;

  
    int start() {
      SDL_Init(SDL_INIT_VIDEO);
      _window = SDL_CreateWindow("An SDL2 Window",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_HEIGHT,
                                SCREEN_WIDTH,
                                SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
      if(_window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << '\n';
        return 1;
      }
      
      _renderer = SDL_CreateRenderer(_window, -1, 0);
      if(_renderer == NULL) {
        std::cout << "Could not create renderer: " << SDL_GetError() << '\n';
        return 1;
      }
      _world = new World();
      return 0;
    }
    
    int enterLoop() {
      bool running = true;
      bool firing = false;
      Direction d = _world->_ship._direction;
      
      while(running == true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
          if(event.key.keysym.sym == SDLK_q) {
            running = false;
          }
          if(event.key.keysym.sym == SDLK_SPACE) {
            firing = true;
          }
          if(event.key.keysym.sym == SDLK_LEFT) {
            d = Direction::Left;
          } else if (event.key.keysym.sym == SDLK_RIGHT) {
            d = Direction::Right;
          }
        }
        
        _world->update(d, firing);
        if(_world->gameOver()){
          running = false;
        }
        firing = false;
        
        drawWorld();
        SDL_Delay(16);
        
      }

      return 0;
    }
    
    int shutdown() {
      SDL_DestroyRenderer(_renderer);
      SDL_DestroyWindow(_window);
      
      SDL_Quit();
      std::cout << "#Winning!";
      
      return 0;
    }
    
    void drawWorld() {
      drawBackground();
      drawShip();
      drawInvaders();
      drawBullets(_world->_shipBullets, 255, 255, 255);
      drawBullets(_world->_invaderBullets, 255, 0, 0);
      SDL_RenderPresent(_renderer);
    }
    
    void drawBackground() {
      SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
      SDL_RenderClear(_renderer);
    }
    
    void drawShip() {
      point_t location = _world->_ship._location;
      SDL_Rect* r = getRect(location.first, location.second, SHIP_HEIGHT, SHIP_WIDTH);
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(_renderer, r);
      
      delete r;
    }
    
    void drawInvaders() {
      std::vector<Invader> invaders = _world->_invaders;
      std::for_each(invaders.begin(), invaders.end(), [this](Invader i) {
        SDL_Rect* r = getRect(i._location.first, i._location.second, INVADER_SIDE, INVADER_SIDE);
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(_renderer, r);
        
        delete r;
      });
    }
    
    void drawBullets(std::vector<Bullet> b, Uint8 red, Uint8 green, Uint8 blue) {
      std::for_each(b.begin(), b.end(), [this, red, green, blue](Bullet b) {
        SDL_Rect* r = getRect(b._location.first, b._location.second, BULLET_SIDE, BULLET_SIDE);
        SDL_SetRenderDrawColor(_renderer, red, green, blue, 255);
        SDL_RenderFillRect(_renderer, r);
        
        delete r;
      });
    }
    
    SDL_Rect* getRect(int x, int y, int h, int w) {
      SDL_Rect* r = new SDL_Rect();
      r->x = x;
      r->y = y;
      r->h = h;
      r->w = w;
      return r;
    }
};