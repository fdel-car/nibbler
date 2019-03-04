#include "Snake.hpp"

int main(int ac, char **av) {
  srand(time(nullptr));
  try {
    // TODO: Map size parsing and stuff
    (void)ac;
    (void)av;
    Snake game;
    game.runLoop();
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
