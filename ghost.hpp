#include "maze.hpp"

enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};

enum Mode {
    CHASE = 0,
    SCATTER = 1,
    FRIGHTENED = 2,
};

class Ghost {
 public:
    Ghost(Cell startCell, Direction startDirection, std::vector<std::vector<wchar_t>>& maze);
    void move(std::vector<std::vector<wchar_t>>& maze);
    Cell getCell() const;
    Direction getDirection() const;
    Mode getMode() const;
    void setMode(Mode newMode);

 private:
    Cell cell;
    Direction direction;
    bool isAboveCoin;
    Mode mode;
    vector<Direction> getPossibleDirections(const std::vector<std::vector<wchar_t>>& maze);
    Cell moveEncoder(Direction direction);
};
