#pragma once

#include <dlfcn.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include "IDisplay.hpp"

#define WIDTH 1280
#define HEIGHT 720

#define LEFT 71
#define RIGHT 72
#define UP 73
#define DOWN 74

#define A 0
#define D 3
#define W 22
#define S 18

struct Player {
	int id = 1;
	int lastDirectionMove = UP;
	int move = -10;
	std::vector<glm::vec2> snakeBodyCoords;
};

class Snake {
 public:
  Snake(void);
  virtual ~Snake(void);

  void runLoop(void);

 private:
  size_t _dylibIdx;
  void *_handle = nullptr;
  IDisplay *(*_displayCreator)(void);
  void (*_displayDestructor)(IDisplay *);
  IDisplay *_display = nullptr;
  std::unordered_map<int, bool> _inputsPressed;
  Player _P1;
  Player _P2;
  bool _multi;

  static std::vector<std::string> _dylibsPaths;

  Snake(Snake const &src);

  void _dlerrorWrapper(void);
  void _checkInputPressed(Player *player);
  void _moveSnake(Player *player);
  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
