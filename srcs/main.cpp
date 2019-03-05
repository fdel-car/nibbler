#include "Snake.hpp"

static void printUsage() {
  std::cerr << "usage: ./nibbler [width] [height] [-m]" << std::endl;
}

static int printError(std::string err) {
  std::cerr << err << std::endl;
  printUsage();
  return EXIT_FAILURE;
}

static bool isNumber(const std::string &s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {
                         return !std::isdigit(c);
                       }) == s.end();
}

int main(int ac, char **av) {
  srand(time(nullptr));
  try {
    Config config;
    if (ac > 2) {
      if (isNumber(av[1]) && isNumber(av[2])) {
        if (strlen(av[1]) < 9 && strlen(av[2]) < 9) {
          Config config;
          config.width = std::stoi(av[1]);
          config.height = std::stoi(av[2]);
          if (config.width >= 20 && config.width <= 40 && config.height >= 20 &&
              config.height <= 40) {
            for (int i = 3; i < ac; i++) {
              if (strcmp(av[i], "-m") == 0)
                config.twoPlayers = true;
              else
                return printError("Invalid flags.");
            }
            Snake game(config);
          } else
            return printError(
                "Invalid width and height. They should be between 20 and 40.");
        } else
          return printError(
              "Width or height overflow. They should be between 20 and 40.");
      } else
        return printError(
            "Invalid width and height. They should be positives numbers "
            "between 20 and 40.");
    } else
      return printError("Declare a width and height for the screen size.");
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
