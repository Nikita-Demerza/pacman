#include "gameActivity.hpp"

GameActivity::GameActivity():
    maze(), pacman(&maze, {1, 1}, PACMAN_RIGHT),
    ghosts(), key(getch()), tick(0), isPaused(true) {
    reset();
}

void GameActivity::reset() {
    srand(time(0));
    initConsole();
    maze.initializeMaze();
    maze.printMaze();
    pacman = Pacman(&maze, {1, 1}, PACMAN_RIGHT);
    ghosts.clear();
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghosts.emplace_back(&maze, Cell{WIDTH-WIDTH/2, HEIGHT-HEIGHT/2}, (Direction)(rand()%4));
    }
}

void initConsole() {
    initscr();
    clear();
#ifdef WIN32
    resize_term(HEIGHT+2, WIDTH);
#endif
    refresh();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, WALL_COLOR, COLOR_BLACK);
    init_pair(2, SPACE_COLOR, COLOR_BLACK);
    init_pair(3, COIN_COLOR, COLOR_BLACK);
    init_pair(4, PACMAN_COLOR, COLOR_BLACK);
    init_pair(5, GHOST_CHASE_COLOR, COLOR_BLACK);
    init_pair(6, GHOST_SCATTER_COLOR, COLOR_BLACK);
    init_pair(7, GHOST_FRIGHTENED_COLOR, COLOR_BLACK);
    clear();
#ifdef WIN32
    cbreak();
#endif
#ifndef WIN32
    endwin();
#endif
}

void GameActivity::loop() {
    while (1) {
        if (control() == QUIT) {
            break;
        }
        updateGhostMode();
        move();
        drawPanel();
        if (pacman.getLives() == 0) {
            gameOver();
            if (!playAgain()) {
                break;
            } else {
                reset();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    initConsole();
}

int GameActivity::control() {
    getInput();
    switch (key) {
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
        case L'q': initConsole(); return -1;
        default: key = (wint_t)ERR;
    }
    return 0;
}

void GameActivity::getInput() {
#ifdef WIN32
    key = getch();
#else
    wget_wch(stdscr, &key);
#endif
}

void GameActivity::updateGhostMode() {
    if (tick%CYCLE_DURATION == DAY_DURATION) {
        for (auto& ghost : ghosts)
            ghost.setMode(Mode::SCATTER);
    } else if (tick%CYCLE_DURATION == 0) {
        for (auto& ghost : ghosts)
            ghost.setMode(Mode::CHASE);
    }
}

void GameActivity::move() {
    if (!isPaused) {
        tick++;
        for (auto& ghost : ghosts)
            ghost.move();
        }
    if (!isPaused || key != (wint_t)ERR)
        isPaused = !pacman.move();
}

void GameActivity::drawPanel() {
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

    mvaddstr(HEIGHT+2,0, "Move: wasd, цфыв or arrow keys. Exit - q.");
}

void GameActivity::gameOver() {
    initConsole();
    mvaddstr(HEIGHT/2, WIDTH/2, "Game Over");
    mvaddstr(HEIGHT/2+1, WIDTH/2, "Play again? (y/n)");
    refresh();
}

bool GameActivity::playAgain() {
    do {
        getInput();
    } while (key != L'y' && key != L'n');
    return key == L'y';
}
