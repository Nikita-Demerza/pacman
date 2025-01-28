#include <vector>
#include "ghost.hpp"

Ghost::Ghost(Maze *maze, Cell startCell, Direction startDirection):
    maze(maze), cell(startCell), direction(startDirection),
    isAboveCoin((*maze)[startCell] == COIN), mode(Mode::CHASE) {
    maze->updateCell(cell, GHOST_CHASE);
}

void Ghost::move() {
    Cell newCell = cell;
    std::vector<Direction> possibleDirections = getPossibleDirections();
    if (possibleDirections.empty()) {
        direction = (Direction)((direction + 2) % 4);
    } else {
        direction = possibleDirections[rand() % possibleDirections.size()];
    }
    newCell = moveEncoder(direction);
    if (mode == Mode::SCATTER && (isPacman((*maze)[cell]) || isPacman((*maze)[newCell]))) {
        newCell = Cell{WIDTH - WIDTH / 2, HEIGHT - HEIGHT / 2};
    }
    if (isAboveCoin) {
        maze->updateCell(cell, COIN);
    } else if (isGhost((*maze)[cell])) {
        maze->updateCell(cell, SPACE);
    }
    cell = newCell;
    isAboveCoin = (*maze)[newCell] == COIN;
    if (mode == Mode::FRIGHTENED) {
        maze->updateCell(newCell, GHOST_FRIGHTENED);
    } else if (mode == Mode::SCATTER) {
        maze->updateCell(newCell, GHOST_SCATTER);
    } else {
        maze->updateCell(newCell, GHOST_CHASE);
    }
}

vector<Direction> Ghost::getPossibleDirections() {
    vector<Direction> possibleDirections;
    Cell newCell = cell;
    for (int i = 0; i < 4; i++) {
        newCell = moveEncoder((Direction)i);
        if (i != (direction + 2) % 4 && maze->isValidMove(newCell)) {
            possibleDirections.push_back((Direction)i);
        }
    }
    return possibleDirections;
}

Cell Ghost::moveEncoder(Direction direction) {
    switch (direction) {
        case UP: return Cell{cell.x, cell.y - 1};
        case DOWN: return Cell{cell.x, cell.y + 1};
        case LEFT: return Cell{cell.x - 1, cell.y};
        default: return Cell{cell.x + 1, cell.y};
    }
}

Mode Ghost::getMode() const {
    return mode;
}

void Ghost::setMode(Mode newMode) {
    mode = newMode;
}

Cell Ghost::getCell() const {
    return cell;
}

Direction Ghost::getDirection() const {
    return direction;
}
