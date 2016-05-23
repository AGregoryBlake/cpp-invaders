#include <algorithm>
#include <iostream>
#include <vector>
#include "bullet.h"
#include "invader.h"
#include "ship.h"

#define MAX_SHIP_BULLETS 3
#define MAX_INVADER_BULLETS 15
#define SHIP_SPEED 5
#define BULLET_SPEED 5

class World {
  public:
    Ship _ship;
    std::vector<Invader> _invaders;
    std::vector<Bullet> _shipBullets;
    std::vector<Bullet> _invaderBullets;
  
    World()
      : _ship(initializeShip()),
        _invaders(initializeInvaders()) {}

  private:
    World(Ship s, std::vector<Invader> i, std::vector<Bullet> sB, std::vector<Bullet> iB)
      : _ship(s),
        _invaders(i),
        _shipBullets(sB),
        _invaderBullets(iB) {}

  public:        
    void update(Direction d, bool firing) {
      this->_ship._direction = d;
      removeHitsAndOutOfBounds();
      invadersFire();
      if(firing) {
        shipFires();
      }
      worldMove();
    }
    
    bool gameOver() {
      return shipHit() || (this->_invaders.size() == 0);
    }
  
  private:
    static Ship initializeShip() {
      return Ship(point_t(250, 480), Direction::Left);
    }
  
    static std::vector<Invader> initializeInvaders() {
      std::vector<Invader> invaders;
      for(int x = 100; x <= 420; x += 40) {
        for(int y = 20; y <= 100; y += 30) {
          invaders.push_back(Invader(point_t(x,y)));
        }
      }
      return invaders;
    }
  
    void worldMove() {
      shipMove();
      bulletsMove(this->_shipBullets);
      bulletsMove(this->_invaderBullets);
    }
    
    void shipMove() {
      std::uint_fast64_t x = this->_ship._location.first;
      if(this->_ship._direction == Direction::Left) {
        x -= SHIP_SPEED;
      } else {
        x += SHIP_SPEED;
      }
      x = (500 + x) % 500;
      this->_ship._location.first = x;
    }
    
    void bulletsMove(std::vector<Bullet>& bullets) {
      for(auto &bullet : bullets) {
        std::uint_fast64_t y = bullet._location.second;
        if(bullet._direction == Direction::Up) {
          y -= BULLET_SPEED;
        } else {
          y += BULLET_SPEED;
        }
        bullet._location.second = y;
      }
    }
    
    void removeHitsAndOutOfBounds() {
      removeHits();
      removeOutOfBounds();
    }
    
    void invadersFire() {
      if(this->_invaderBullets.size() < MAX_INVADER_BULLETS) {
        int random = rand() % this->_invaders.size();
        this->_invaderBullets.push_back(Bullet(this->_invaders.at(random)._location, Direction::Down));
      }
    }
    
    void shipFires() {
      if(this->_shipBullets.size() < MAX_SHIP_BULLETS) {
        this->_shipBullets.push_back(Bullet(this->_ship._location, Direction::Up));
      }
    }
    
    void removeHits() {
      std::vector<Bullet> shipBullets = this->_shipBullets;
      std::vector<Invader> invaders = this->_invaders;
      
      auto new_end_bullets = std::remove_if(this->_shipBullets.begin(), this->_shipBullets.end(), [invaders](Bullet b) {
        for(auto &invader : invaders) {
          if(invader._location == b._location) {
            return true;
          }
        }
        return false;
      });
      this->_shipBullets.erase(new_end_bullets, this->_shipBullets.end());
      auto new_end_invaders = std::remove_if(this->_invaders.begin(), this->_invaders.end(), [shipBullets](Invader i) {
        for(auto &bullet : shipBullets) {
          if(bullet._location == i._location) {
            return true;
          }
        }
        return false;
      });
      this->_invaders.erase(new_end_invaders, this->_invaders.end());
    }
    
    void removeOutOfBounds() {
      auto new_end = std::remove_if(this->_invaderBullets.begin(), this->_invaderBullets.end(), [](Bullet b) {
        return b._location.second > 500;
      });
      this->_invaderBullets.erase(new_end, this->_invaderBullets.end());
      new_end = std::remove_if(this->_shipBullets.begin(), this->_shipBullets.end(), [](Bullet b) {
        return b._location.second > 500;
      });
      this->_shipBullets.erase(new_end, this->_shipBullets.end());
    }
    
    bool shipHit() {
      for(auto &bullet : this->_invaderBullets) {
        if(this->_ship._location == bullet._location) {
          return true;
        }
      }
      return false;
    }
};