#include "maze.hpp"

void initializeMaze(vector<vector<wchar_t>>& maze) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            maze[y][x] = WALL;
        }
    }
}

bool isValidForSpace(const vector<vector<wchar_t>>& maze, int x, int y) {
    if (x <= 0 || x >= WIDTH - 1 || y <= 0 || y >= HEIGHT - 1) return false;
    if (maze[y][x] == SPACE) return false;

    int count = 0;
    if (maze[y - 1][x] == SPACE) count++;
    if (maze[y + 1][x] == SPACE) count++;
    if (maze[y][x - 1] == SPACE) count++;
    if (maze[y][x + 1] == SPACE) count++;

    return count <= 1;
}

void generateMaze(vector<vector<wchar_t>>& maze) {
    stack<Cell> stack;
    stack.push({1, 1});
    maze[1][1] = SPACE;

    while (!stack.empty()) {
        Cell current = stack.top();
        vector<Cell> neighbors;

        if (isValidForSpace(maze, current.x, current.y - 2)) neighbors.push_back({current.x, current.y - 2});
        if (isValidForSpace(maze, current.x, current.y + 2)) neighbors.push_back({current.x, current.y + 2});
        if (isValidForSpace(maze, current.x - 2, current.y)) neighbors.push_back({current.x - 2, current.y});
        if (isValidForSpace(maze, current.x + 2, current.y)) neighbors.push_back({current.x + 2, current.y});

        if (!neighbors.empty()) {
            vector<Cell> weightedNeighbors;
            for (const auto& neighbor : neighbors) {
                weightedNeighbors.push_back(neighbor);
                if (neighbor.x == current.x || neighbor.y == current.y) {
                    for (size_t i = 0; i < neighbors.size(); i++)
                        weightedNeighbors.push_back(neighbor);
                }
            }
            Cell next = weightedNeighbors[rand() % weightedNeighbors.size()];
            maze[next.y][next.x] = SPACE;
            maze[(current.y + next.y) / 2][(current.x + next.x) / 2] = SPACE;
            stack.push(next);
        } else {
            stack.pop();
        }
    }
}

vector<Cell> findDeadEnds(const vector<vector<wchar_t>>& maze) {
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

void removeWallInDeadEnd(vector<vector<wchar_t>>& maze, const Cell& deadEnd) {
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

void removeDeadEnds(vector<vector<wchar_t>>& maze) {
    vector<Cell> deadEnds;
    do {
        deadEnds = findDeadEnds(maze);
        for (const auto& deadEnd : deadEnds) {
            removeWallInDeadEnd(maze, deadEnd);
        }
    } while (!deadEnds.empty());
}

void placeCoin(vector<vector<wchar_t>>& maze) {
    int x, y;
    do {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    } while (maze[y][x] != SPACE);
    maze[y][x] = COIN;
    updateCell(maze, x, y, COIN);
}

void generateCoins(vector<vector<wchar_t>>& maze) {
    for (int i = 0; i < NUM_COINS; i++) {
        placeCoin(maze);
    }
}

void renderCell(const vector<vector<wchar_t>>& maze, int x, int y) {
    #ifdef WIN32
    mvaddch(y + 1, x, maze[y][x]);
    #else
    mvaddwstr(y + 1, x, &maze[y][x]);
    #endif
}

void updateCell(vector<vector<wchar_t>>& maze, int x, int y, wchar_t symbol) {
    maze[y][x] = symbol;
    renderCell(maze, x, y);
}

void printMaze(const vector<vector<wchar_t>>& maze) {
    for (size_t i = 0; i < HEIGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            renderCell(maze, j, i);
        }
    }
    refresh();
}

bool isPacman(const wchar_t symbol) {
    return symbol == PACMAN_UP || symbol == PACMAN_DOWN || symbol == PACMAN_LEFT || symbol == PACMAN_RIGHT;
}

bool isGhost(const wchar_t symbol) {
    return symbol == GHOST_CHASE || symbol == GHOST_SCATTER || symbol == GHOST_FRIGHTENED;
}

bool isValidMove(const std::vector<std::vector<wchar_t>>& maze, Cell newCell) {
    if (newCell.x < 0 || newCell.x > WIDTH-1 ||
        newCell.y < 0 || newCell.y > HEIGHT-1 ||
        maze[newCell.y][newCell.x] == WALL)
        return false;
    return true;
}
