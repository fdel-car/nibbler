#include "Snake.hpp"

static bool is_number(const std::string &s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {
                         return !std::isdigit(c);
                       }) == s.end();
}

static void usage() { std::cerr << "Bad formatage." << std::endl; }

int main(int ac, char **av) {
  srand(time(nullptr));
  try {
    // TODO: Map size parsing and stuff
    if (ac > 2 && is_number(av[1]) && is_number(av[2]) && strlen(av[1]) < 9 &&
        strlen(av[2]) < 9) {
      Config config;
      config.width = std::stoi(av[1]);
      config.height = std::stoi(av[2]);
      if (config.width >= 20 && config.width <= 40 && config.height >= 20 &&
          config.height <= 40) {
        for (int i = 3; i < ac; i++) {
          if (strcmp(av[i], "-m") == 0)
            config.twoPlayers = true;
          else
            usage();
        }
        Snake game(config);
        game.runLoop();
      }
    }
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
