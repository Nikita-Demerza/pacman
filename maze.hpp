#ifndef MAZE_HPP_
#define MAZE_HPP_

#include <stdlib.h>
#include <wchar.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <chrono>
#include <thread>
#ifdef WIN32
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif
#include "parameters.hpp"

using namespace std;

struct Cell {
    int x, y;
};

class Maze {
 public:
    Maze();
    void initializeMaze();
    void generateMaze();
    vector<Cell> findDeadEnds() const;
    void removeDeadEnds();
    void placeCoin();
    void generateCoins();
    void updateCell(const Cell& cell, wchar_t symbol);
    void printMaze() const;
    bool isValidMove(const Cell& newCell) const;
    wchar_t& operator()(int x, int y);
    wchar_t& operator[](const Cell& cell);

 private:
    std::vector<std::vector<wchar_t>> maze;

    void renderCell(const Cell& cell) const;
    bool isValidForSpace(const Cell& cell) const;
    void removeWallInDeadEnd(const Cell& deadEnd);
};;
bool isPacman(const wchar_t symbol);
bool isGhost(const wchar_t symbol);

#endif  // MAZE_HPP_
