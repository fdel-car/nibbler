#pragma once

#include <dlfcn.h>
#include <algorithm>
#include <ctime>
#include <unordered_map>
#include <vector>
#include "IDisplay.hpp"

#define WIDTH 1280
#define HEIGHT 720

struct Config {
  int width;
  int height;
  bool twoPlayers = false;
};

struct Player {
  std::string dir = "UP";
  float speed = 1.f;
  std::vector<glm::vec2> bodyParts;
  std::vector<std::string> allDirs;
};

class Snake {
 public:
  Snake(Config config);
  virtual ~Snake(void);

  std::chrono::high_resolution_clock::time_point currTime, prevTime;
  double deltaTime, timeElapsed = 0.0;

  void runLoop(void);
  bool isKeyPressed(std::string key) const;
  bool isKeyJustPressed(std::string key) const;

 private:
  Config _config;
  size_t _dylibIdx;
  size_t _newdylibIdx;
  void *_handle = nullptr;
  IDisplay *(*_displayCreator)(int w, int h);
  void (*_displayDestructor)(IDisplay *);
  IDisplay *_display = nullptr;
  std::map<std::string, KeyState> _keyMap;
  Player fstPlayer;
  Player sndPlayer;
  float _interval;
  float const _snakeUnit;

  static std::vector<std::string> _dylibsPaths;

  Snake(void);
  Snake(Snake const &src);

  void _dlerrorWrapper(void);
  void _handleInput(Player &player);
  void _moveSnake(Player &player);
  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
