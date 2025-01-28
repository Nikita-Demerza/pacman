#ifndef PACMAN_HPP_
#define PACMAN_HPP_

#include <wchar.h>
#include <vector>
#include "maze.hpp"

class Pacman {
 public:
    Pacman(Maze *maze, Cell startCell, wchar_t startDirection);
    int move();
    void changeDirection(wchar_t newDirection);
    int getX() const;
    int getY() const;
    int getScore() const;
    int getLives() const;
    wchar_t getDirection() const;

 private:
    Maze *maze;
    Cell cell;
    wchar_t direction;
    int score = 0;
    int lives = 3;
    void updateScore();
    int restartIfKilledByGhost();
};

#endif  // PACMAN_HPP_
