#include <wchar.h>
#include <vector>

class Pacman {
 public:
    Pacman(int startX, int startY, wchar_t startDirection, std::vector<std::vector<wchar_t>>& maze);
    int move(std::vector<std::vector<wchar_t>>& maze);
    void changeDirection(wchar_t newDirection);
    int getX() const;
    int getY() const;
    int getScore() const;
    int getLives() const;
    wchar_t getDirection() const;

 private:
    int x, y;
    wchar_t direction;
    int score = 0;
    int lives = 3;
    void updateScore(std::vector<std::vector<wchar_t>>& maze);
    int restartIfKilledByGhost(std::vector<std::vector<wchar_t>>& maze);
};

