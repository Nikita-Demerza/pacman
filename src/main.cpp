#include "gameActivity.hpp"

int main() {
#ifndef WIN32
    setlocale(LC_ALL, "");
#endif
    initConsole();
    GameActivity game;
    game.loop();
    return 0;
}
