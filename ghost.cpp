#include "ghost.hpp"

Ghost::Ghost(Cell startCell, Direction startDirection, std::vector<std::vector<wchar_t>>& maze):
    cell(startCell), direction(startDirection), isAboveCoin(maze[cell.y][cell.x] == COIN), mode(Mode::CHASE) {
        updateCell(maze, cell.x, cell.y, GHOST_CHASE);
    }

void Ghost::move(std::vector<std::vector<wchar_t>>& maze) {
    Cell newCell = cell;
    std::vector<Direction> possibleDirections = getPossibleDirections(maze);
    if (possibleDirections.empty()) {
        direction = (Direction)((direction+2)%4);
    } else {
        direction = possibleDirections[rand()%possibleDirections.size()];
    }
    newCell = moveEncoder(direction);
    if (mode == Mode::SCATTER && (isPacman(maze[cell.y][cell.x]) || isPacman(maze[newCell.y][newCell.x]))) {
        newCell = Cell{WIDTH-WIDTH/2, HEIGHT-HEIGHT/2};
    }
    if (isAboveCoin) {
        updateCell(maze, cell.x, cell.y, COIN);
    } else if (isGhost(maze[cell.y][cell.x])) {
        updateCell(maze, cell.x, cell.y, SPACE);
    }
    cell = newCell;
    isAboveCoin = maze[newCell.y][newCell.x] == COIN;
    if (mode == Mode::FRIGHTENED) {
        updateCell(maze, newCell.x, newCell.y, GHOST_FRIGHTENED);
    } else if (mode == Mode::SCATTER) {
        updateCell(maze, newCell.x, newCell.y, GHOST_SCATTER);
    } else {
        updateCell(maze, newCell.x, newCell.y, GHOST_CHASE);
    }
}

std::vector<Direction> Ghost::getPossibleDirections(const std::vector<std::vector<wchar_t>>& maze) {
    std::vector<Direction> possibleDirections;
    Cell newCell = cell;
    for (int i = 0; i < 4; i++) {
        newCell = moveEncoder((Direction)i);
        if (i != (direction+2)%4 && isValidMove(maze, newCell)) {
            possibleDirections.push_back((Direction)i);
        }
    }
    return possibleDirections;
}

Cell Ghost::moveEncoder(Direction direction) {
    switch (direction) {
        case UP: return Cell{cell.x, cell.y-1};
        case DOWN: return Cell{cell.x, cell.y+1};
        case LEFT: return Cell{cell.x-1, cell.y};
        default: return Cell{cell.x+1, cell.y};
    }
}

Mode Ghost::getMode() const {
    return mode;
}

void Ghost::setMode(Mode newMode) {
    mode = newMode;
}
