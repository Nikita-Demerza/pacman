#include "pacman.hpp"
#include "maze.hpp"

Pacman::Pacman(int startX, int startY, wchar_t startDirection, std::vector<std::vector<wchar_t>>& maze):
    x(startX), y(startY), direction(startDirection) {
        updateCell(maze, x, y, direction);
    }

int Pacman::move(std::vector<std::vector<wchar_t>>& maze) {
    if (restartIfKilledByGhost(maze)) return 0;
    int newX = x, newY = y;
    switch (direction) {
        case PACMAN_UP: newY--; break;
        case PACMAN_DOWN: newY++; break;
        case PACMAN_LEFT: newX--; break;
        case PACMAN_RIGHT: newX++; break;
    }

    updateScore(maze);
    if (newX < 0) {
        newX = WIDTH - 1;
    } else if (newX > WIDTH - 1) {
        newX = 0;
    }
    if (newY < 0) {
        newY = HEIGHT - 1;
    } else if (newY > HEIGHT - 1) {
        newY = 0;
    }

    if (isValidMove(maze, Cell{newX, newY})) {
        updateScore(maze);
        if (isPacman(maze[y][x])) {
            updateCell(maze, x, y, SPACE);
        }
        x = newX;
        y = newY;
        if (restartIfKilledByGhost(maze)) return 0;
        updateScore(maze);
        updateCell(maze, x, y, direction);
        return 1;
    } else {
        updateCell(maze, x, y, direction);
        return 0;
    }
}

void Pacman::updateScore(std::vector<std::vector<wchar_t>>& maze) {
    if (maze[y][x] == COIN) {
        score++;
        placeCoin(maze);
    }
    if (maze[y][x] == GHOST_SCATTER) {
        score+=100;
    }
}

int Pacman::restartIfKilledByGhost(std::vector<std::vector<wchar_t>>& maze) {
    if (maze[y][x] == GHOST_CHASE) {
        for (int i = 0; i < 4; i++) {
            updateCell(maze, x, y, PACMAN_RIGHT);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            updateCell(maze, x, y, PACMAN_DOWN);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            updateCell(maze, x, y, PACMAN_LEFT);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            updateCell(maze, x, y, PACMAN_UP);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
        }
        lives -= 1;
        updateCell(maze, x, y, SPACE);
        x = 1, y = 1;
        updateCell(maze, x, y, direction);
        return 1;
    }
    return 0;
}

void Pacman::changeDirection(wchar_t newDirection) {
    direction = newDirection;
}

int Pacman::getX() const {
    return x;
}

int Pacman::getY() const {
    return y;
}

int Pacman::getScore() const {
    return score;
}

int Pacman::getLives() const {
    return lives;
}

wchar_t Pacman::getDirection() const {
    return direction;
}
