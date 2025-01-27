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
#include "pacman.hpp"
#include "ghost.hpp"
// #include "maze.hpp"

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

void initConsole() {
#ifdef WIN32
    system("cls");
    setlocale(LC_ALL, "C");
    wcout.imbue(std::locale("C"));
#else
    system("clear");
    setlocale(LC_ALL, "");
    wcout.imbue(std::locale(""));
#endif
    initscr();
    resize_term(HEIGHT+1, WIDTH);
    clear();
    refresh();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
#ifdef WIN32
    cbreak();
#endif
#ifndef WIN32
    endwin();
#endif
}

void newGame(Maze& maze, Pacman& pacman, std::vector<Ghost>& ghosts) {
    maze.initializeMaze();
    maze.printMaze();
    pacman = Pacman(&maze, {1, 1}, PACMAN_RIGHT);
    ghosts.clear();
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghosts.emplace_back(&maze, Cell{WIDTH-WIDTH/2, HEIGHT-HEIGHT/2}, (Direction)(rand()%4));
    }
    getch();
}

int getInput() {
    wint_t control;
#ifdef WIN32
    control = getch();
#else
    wget_wch(stdscr, &control);
#endif
    return control;
}

int main() {
    system("chcp 65001");
    initConsole();
    srand(static_cast<unsigned>(time(0)));
    Maze maze = Maze();
    wint_t control;
    int tick = 0;
    const int MAX_TICK = DAY_DURATION + NIGHT_DURATION;
    bool move = false;
    Pacman pacman(&maze, {1, 1}, PACMAN_RIGHT);
    std::vector<Ghost> ghosts;
    newGame(maze, pacman, ghosts);
    while (1) {
        loop_start:
        control = getInput();
        switch (control) {
            case KEY_UP:
            case KEY_UP_:
            case L'ц':
            case KEY_W:
            case L'w': pacman.changeDirection(PACMAN_UP); break;
            case KEY_DOWN:
            case KEY_DOWN_:
            case L'ы':
            case KEY_S:
            case L's': pacman.changeDirection(PACMAN_DOWN); break;
            case KEY_LEFT:
            case KEY_LEFT_:
            case L'ф':
            case KEY_A:
            case L'a': pacman.changeDirection(PACMAN_LEFT); break;
            case KEY_RIGHT:
            case KEY_RIGHT_:
            case L'в':
            case KEY_D:
            case L'd': pacman.changeDirection(PACMAN_RIGHT); break;
            case L'й':
            case KEY_Q:
            case L'q': initConsole(); return 0;
            case KEY_RU1:
            case KEY_RU2: goto loop_start;
            default: control = (wint_t)ERR;
        }
        if (tick%MAX_TICK == DAY_DURATION) {
            for (auto& ghost : ghosts)
                ghost.setMode(Mode::SCATTER);
        } else if (tick%MAX_TICK == 0) {
            for (auto& ghost : ghosts)
                ghost.setMode(Mode::CHASE);
        }
        if (move) {
            tick++;
            for (auto& ghost : ghosts)
                ghost.move();
        }
        if (move || control != (wint_t)ERR)
            move = pacman.move();
        if (pacman.getLives() < 0) {
            initConsole();
            mvaddstr(HEIGHT/2, WIDTH/2, "Game Over");
            mvaddstr(HEIGHT/2+1, WIDTH/2, "Play again? (y/n)");
            refresh();
            do {
                control = getInput();
            } while (control != L'y' && control != L'n');
            if (control == L'n')
                break;
            newGame(maze, pacman, ghosts);
            tick = 0;
            continue;
        }
        mvaddstr(0, 0, "Score: ");
        mvaddstr(0, 7, std::to_string(pacman.getScore()).c_str());
        mvaddstr(0, 30, "Lives: ");
        mvaddstr(0, 37, std::to_string(pacman.getLives()).c_str());
        mvaddstr(0, 48, "Current time: ");
        mvaddstr(0, 62, "          ");
        mvaddstr(0, 62, std::to_string(tick%1500).c_str());
        if (tick >= DAY_DURATION)
            mvaddstr(0, 70, "Night");
        else
            mvaddstr(0, 70, "Day  ");
        mvaddstr(0, 80, "Total time: ");
        mvaddstr(0, 93, "          ");
        mvaddstr(0, 93, std::to_string(tick).c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
