#include <vector>
#include <stack>
#include "maze.hpp"

Maze::Maze(): maze(vector<vector<wchar_t>>(HEIGHT, vector<wchar_t>(WIDTH, WALL))) {}


wchar_t& Maze::operator()(int x, int y) {
    return maze[y][x];
}

wchar_t& Maze::operator[](const Cell& cell) {
    return maze[cell.y][cell.x];
}

void Maze::initializeMaze() {
    maze = vector<vector<wchar_t>>(HEIGHT, vector<wchar_t>(WIDTH, WALL));
    generateMaze();
    removeDeadEnds();
    generateCoins();
}

void Maze::generateMaze() {
    stack<Cell> stack;
    stack.push({1, 1});
    maze[1][1] = SPACE;

    while (!stack.empty()) {
        Cell current = stack.top();
        vector<Cell> neighbors;

        if (isValidForSpace({current.x, current.y - 2})) neighbors.push_back({current.x, current.y - 2});
        if (isValidForSpace({current.x, current.y + 2})) neighbors.push_back({current.x, current.y + 2});
        if (isValidForSpace({current.x - 2, current.y})) neighbors.push_back({current.x - 2, current.y});
        if (isValidForSpace({current.x + 2, current.y})) neighbors.push_back({current.x + 2, current.y});

        if (!neighbors.empty()) {
            Cell next = neighbors[rand() % neighbors.size()];
            maze[next.y][next.x] = SPACE;
            maze[(current.y + next.y) / 2][(current.x + next.x) / 2] = SPACE;
            stack.push(next);
        } else {
            stack.pop();
        }
    }
}

void Maze::removeDeadEnds() {
    vector<Cell> deadEnds;
    do {
        deadEnds = findDeadEnds();
        for (const auto& deadEnd : deadEnds) {
            removeWallInDeadEnd(deadEnd);
        }
    } while (!deadEnds.empty());
}

vector<Cell> Maze::findDeadEnds() const {
    vector<Cell> deadEnds;
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            if (maze[y][x] == SPACE) {
                int walls = 0;
                if (maze[y - 1][x] == WALL) walls++;
                if (maze[y + 1][x] == WALL) walls++;
                if (maze[y][x - 1] == WALL) walls++;
                if (maze[y][x + 1] == WALL) walls++;
                if (walls == 3) {
                    deadEnds.push_back({x, y});
                }
            }
        }
    }
    return deadEnds;
}

void Maze::removeWallInDeadEnd(const Cell& deadEnd) {
    Cell wallToRemove = {deadEnd.x, deadEnd.y};
    if (maze[deadEnd.y - 1][deadEnd.x] == SPACE) wallToRemove = {deadEnd.x, deadEnd.y + 1};
    if (maze[deadEnd.y + 1][deadEnd.x] == SPACE) wallToRemove = {deadEnd.x, deadEnd.y - 1};
    if (maze[deadEnd.y][deadEnd.x - 1] == SPACE) wallToRemove = {deadEnd.x + 1, deadEnd.y};
    if (maze[deadEnd.y][deadEnd.x + 1] == SPACE) wallToRemove = {deadEnd.x - 1, deadEnd.y};
    maze[wallToRemove.y][wallToRemove.x] = SPACE;

    if (wallToRemove.x <= 0 || wallToRemove.x >= WIDTH - 1) {
        maze[wallToRemove.y][WIDTH-wallToRemove.x - 1] = SPACE;
    }
    if (wallToRemove.y <= 0 || wallToRemove.y >= HEIGHT - 1) {
        maze[HEIGHT - wallToRemove.y - 1][wallToRemove.x] = SPACE;
    }
}

void Maze::placeCoin() {
    int x, y;
    do {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    } while (maze[y][x] != SPACE);
    maze[y][x] = COIN;
    updateCell({x, y}, COIN);
}

void Maze::generateCoins() {
    for (int i = 0; i < NUM_COINS; i++) {
        placeCoin();
    }
}

void Maze::renderCell(const Cell& cell) const {
    #ifdef WIN32
    mvaddch(cell.y + 1, cell.x, maze[cell.y][cell.x]);
    #else
    mvaddwstr(cell.y + 1, cell.x, &maze[cell.y][cell.x]);
    #endif
}

void Maze::updateCell(const Cell& cell, wchar_t symbol) {
    maze[cell.y][cell.x] = symbol;
    renderCell(cell);
}

void Maze::printMaze() const {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            renderCell({j, i});
        }
    }
    refresh();
}

bool Maze::isValidMove(const Cell& newCell) const {
    if (newCell.x < 0 || newCell.x > WIDTH-1 ||
        newCell.y < 0 || newCell.y > HEIGHT-1 ||
        maze[newCell.y][newCell.x] == WALL)
        return false;
    return true;
}

bool Maze::isValidForSpace(const Cell& cell) const {
    if (cell.x <= 0 || cell.x >= WIDTH - 1 || cell.y <= 0 || cell.y >= HEIGHT - 1) return false;
    if (maze[cell.y][cell.x] == SPACE) return false;

    int count = 0;
    if (maze[cell.y - 1][cell.x] == SPACE) count++;
    if (maze[cell.y + 1][cell.x] == SPACE) count++;
    if (maze[cell.y][cell.x - 1] == SPACE) count++;
    if (maze[cell.y][cell.x + 1] == SPACE) count++;

    return count <= 1;
}

bool isPacman(const wchar_t symbol) {
    return symbol == PACMAN_UP || symbol == PACMAN_DOWN || symbol == PACMAN_LEFT || symbol == PACMAN_RIGHT;
}

bool isGhost(const wchar_t symbol) {
    return symbol == GHOST_CHASE || symbol == GHOST_SCATTER || symbol == GHOST_FRIGHTENED;
}
