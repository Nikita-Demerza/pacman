#include <stdlib.h>
#include <wchar.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <chrono>
#include <thread>
#include "parameters.hpp"
#ifdef WIN32
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

using namespace std;

struct Cell {
    int x, y;
};

void initializeMaze(std::vector<std::vector<wchar_t>>& maze);
bool isValidForSpace(const std::vector<std::vector<wchar_t>>& maze, int x, int y);
void generateMaze(std::vector<std::vector<wchar_t>>& maze);
vector<Cell> findDeadEnds(const vector<vector<wchar_t>>& maze);
void removeWallInDeadEnd(std::vector<std::vector<wchar_t>>& maze, const Cell& deadEnd);
void removeDeadEnds(std::vector<std::vector<wchar_t>>& maze);
void placeCoin(vector<vector<wchar_t>>& maze);
void generateCoins(std::vector<std::vector<wchar_t>>& maze);
void renderCell(const vector<vector<wchar_t>>& maze, int x, int y);
void updateCell(vector<vector<wchar_t>>& maze, int x, int y, wchar_t symbol);
void printMaze(const std::vector<std::vector<wchar_t>>& maze);
bool isPacman(const wchar_t symbol);
bool isGhost(const wchar_t symbol);
bool isValidMove(const std::vector<std::vector<wchar_t>>& maze, Cell newCell);
