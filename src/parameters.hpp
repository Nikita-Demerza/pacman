#include <wchar.h>
#ifdef WIN32
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

const int WIDTH = 101;
const int HEIGHT = 33;
const int NUM_COINS = 200;
const int NUM_GHOSTS = 10;
const int DAY_DURATION = 1000;
const int NIGHT_DURATION = 500;
const int CYCLE_DURATION = DAY_DURATION + NIGHT_DURATION;
const wchar_t WALL = L'█';
const wchar_t SPACE = L' ';
const wchar_t COIN = L'•';
const wchar_t PACMAN_UP = L'ʌ';
const wchar_t PACMAN_DOWN = L'v';
const wchar_t PACMAN_LEFT = L'<';
const wchar_t PACMAN_RIGHT = L'>';
const wchar_t GHOST_CHASE = L'☻';
const wchar_t GHOST_SCATTER = L'☺';
const wchar_t GHOST_FRIGHTENED = L'0';
const int WALL_COLOR = COLOR_RED;
const int SPACE_COLOR = COLOR_BLACK;
const int COIN_COLOR = COLOR_YELLOW;
const int PACMAN_COLOR = COLOR_YELLOW;
const int GHOST_CHASE_COLOR = COLOR_CYAN;
const int GHOST_SCATTER_COLOR = COLOR_BLUE;
const int GHOST_FRIGHTENED_COLOR = COLOR_WHITE;
