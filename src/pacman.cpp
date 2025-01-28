#include "pacman.hpp"

Pacman::Pacman(Maze *maze, Cell startCell, wchar_t startDirection)
    : maze(maze), cell(startCell), direction(startDirection) {
    maze->updateCell(cell, direction);
}

int Pacman::move() {
    if (restartIfKilledByGhost()) return 0;
    Cell newCell = cell;
    switch (direction) {
        case PACMAN_UP: newCell.y--; break;
        case PACMAN_DOWN: newCell.y++; break;
        case PACMAN_LEFT: newCell.x--; break;
        case PACMAN_RIGHT: newCell.x++; break;
    }

    updateScore();
    if (newCell.x < 0) {
        newCell.x = WIDTH - 1;
    } else if (newCell.x > WIDTH - 1) {
        newCell.x = 0;
    }
    if (newCell.y < 0) {
        newCell.y = HEIGHT - 1;
    } else if (newCell.y > HEIGHT - 1) {
        newCell.y = 0;
    }

    if (maze->isValidMove(newCell)) {
        updateScore();
        if (isPacman((*maze)[cell])) {
            maze->updateCell(cell, SPACE);
        }
        cell = newCell;
        if (restartIfKilledByGhost()) return 0;
        updateScore();
        maze->updateCell(cell, direction);
        return 1;
    } else {
        maze->updateCell(cell, direction);
        return 0;
    }
}

void Pacman::updateScore() {
    if ((*maze)[cell] == COIN) {
        score++;
        maze->placeCoin();
    }
    if ((*maze)[cell] == GHOST_SCATTER) {
        score += 100;
    }
}

int Pacman::restartIfKilledByGhost() {
    if ((*maze)[cell] == GHOST_CHASE) {
        for (int i = 0; i < 4; i++) {
            maze->updateCell(cell, PACMAN_RIGHT);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            maze->updateCell(cell, PACMAN_DOWN);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            maze->updateCell(cell, PACMAN_LEFT);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
            maze->updateCell(cell, PACMAN_UP);
            this_thread::sleep_for(chrono::milliseconds(300));
            refresh();
        }
        lives -= 1;
        maze->updateCell(cell, SPACE);
        cell = {1, 1};
        maze->updateCell(cell, direction);
        return 1;
    }
    return 0;
}

void Pacman::changeDirection(wchar_t newDirection) {
    direction = newDirection;
}

int Pacman::getX() const {
    return cell.x;
}

int Pacman::getY() const {
    return cell.y;
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
