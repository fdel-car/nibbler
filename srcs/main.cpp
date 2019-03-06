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
  try {
	if (ac < 3)
		return printError("Declare a width and height for the screen size.");
	if (!isNumber(av[1]) || !isNumber(av[2]))
		return printError(
			"Invalid width and height. They should be positives numbers "
			"between 20 and 40.");
	if (strlen(av[1]) > 9 && strlen(av[2]) > 9)
		return printError(
			"Width or height are to big. They should be between 20 and 40.");
	Config config;
	config.width = std::stoi(av[1]);
	config.height = std::stoi(av[2]);
	if (config.width < 20 || config.width > 40 || config.height < 20 ||
		config.height > 40)
		return printError(
			"Invalid width and height. They should be between 20 and 40.");
	for (int i = 3; i < ac; i++) {
		if (strcmp(av[i], "-m") == 0)
			config.twoPlayers = true;
		else
			return printError("Invalid flags.");
	}
	Snake game(config);
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
