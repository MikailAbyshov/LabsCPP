#include "../includes/Game.h"
#include <iostream>

int main() {
    std::cout << "[DEBUG] Starting game..." << std::endl;
    Game game;
    std::cout << "[DEBUG] Game object created. Entering game loop..." << std::endl;
    game.run();
    std::cout << "[DEBUG] Game loop ended." << std::endl;
    return 0;
}