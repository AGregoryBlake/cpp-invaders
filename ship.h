#ifndef DIRECTION_H
#define DIRECTION_H
#include "direction.h"
#endif

#ifndef POINT_H
#define POINT_H
#include "point.h"
#endif

class Ship {
  public:
    point_t _location;
    Direction _direction;

    Ship(point_t t, Direction d) {
        _location = t;
        _direction = d;
    };
    
};