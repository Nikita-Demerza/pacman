#ifndef GHOST_HPP_
#define GHOST_HPP_

#include <vector>
#include "maze.hpp"

enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};

enum Mode {
    CHASE = 0,
    SCATTER = 1,
    FRIGHTENED = 2,
};

class Ghost {
 public:
    Ghost(Maze *maze, Cell startCell, Direction startDirection);
    void move();
    Cell getCell() const;
    Direction getDirection() const;
    Mode getMode() const;
    void setMode(Mode newMode);

 private:
    Maze *maze;
    Cell cell;
    Direction direction;
    bool isAboveCoin;
    Mode mode;
    vector<Direction> getPossibleDirections();
    Cell moveEncoder(Direction direction);
};

#endif  // GHOST_HPP_
