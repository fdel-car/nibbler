#pragma once

#include <dlfcn.h>
#include <vector>
#include <unordered_map>
#include <tuple>
#include "IDisplay.hpp"

#define EPSILON 0.01f

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
  std::vector<glm::vec2> _snakeBodyCoords;

  static std::vector<std::string> _dylibsPaths;

  Snake(Snake const &src);

  void _dlerrorWrapper(void);
  void _checkInputPressed(void);
  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
