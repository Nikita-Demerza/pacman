#include <iostream>
#ifdef WIN32
#include <ncursesw/ncurses.h>
#include <windows.h>
#else
#include <ncurses.h>
#include <locale>
#endif
#include <ctime>
#include <chrono>
#include <thread>
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"

const int KEY_UP_ = 60419;
const int KEY_DOWN_ = 60418;
const int KEY_RIGHT_ = 60421;
const int KEY_LEFT_ = 60420;

const int KEY_RU1 = 208;
const int KEY_RU2 = 209;

const int KEY_W = 134;
const int KEY_S = 139;
const int KEY_A = 132;
const int KEY_D = 178;
const int KEY_Q = 185;

const int QUIT = -1;

class GameActivity {
 public:
    GameActivity();
    void reset();
    void loop();
    int control();
    void updateGhostMode();
    void gameOver();
    bool playAgain();
    void move();
    void getInput();
    void drawPanel();

 private:
    Maze maze;
    Pacman pacman;
    std::vector<Ghost> ghosts;
    wint_t key;
    int tick;
    bool isPaused;
};

void initConsole();
