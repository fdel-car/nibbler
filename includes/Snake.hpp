#pragma once

#include <dlfcn.h>
#include <algorithm>
#include <ctime>
#include <unordered_map>
#include <vector>
#include "IDisplay.hpp"

#define WIDTH 1280
#define HEIGHT 720

struct Player {
  std::string dir = "UP";
  // TODO: Speed coefficient maybe?
  std::vector<glm::vec2> bodyParts;
  std::vector<std::string> allDirs;
};

class Snake {
 public:
  Snake(void);
  virtual ~Snake(void);

  void runLoop(void);
  bool isKeyPressed(std::string key) const;
  bool isKeyJustPressed(std::string key) const;

 private:
  size_t _dylibIdx;
  void *_handle = nullptr;
  IDisplay *(*_displayCreator)(void);
  void (*_displayDestructor)(IDisplay *);
  IDisplay *_display = nullptr;
  std::map<std::string, KeyState> _keyMap;
  Player fstPlayer;
  Player sndPlayer;
  bool _twoPlayers;

  static std::vector<std::string> _dylibsPaths;

  Snake(Snake const &src);

  void _dlerrorWrapper(void);
  void _handleInput(Player &player);
  void _moveSnake(Player &player);
  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
