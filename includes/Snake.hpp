#pragma once

#include <dlfcn.h>
#include <algorithm>
#include <ctime>
#include "IAudio.hpp"
#include "IDisplay.hpp"

struct Config {
  int width;
  int height;
  bool twoPlayers = false;
};

struct Keys {
  std::string up = "W";
  std::string left = "A";
  std::string down = "S";
  std::string right = "D";
};

struct Player {
  float speed = 1.f;
  float distCrawled = 0.f;
  int prevCrawled = 0;
  SharedData data;
  std::vector<int> allDirs;
  glm::ivec2 newBodyPart;
  int newDirAngle;
  bool hasEaten = false;
  Keys keys;
  int score = 0;
};

struct Food {
  glm::ivec2 coords;
  int lifeTime = -1;
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
  size_t _newDylibIdx;

  void *_handle = nullptr;
  IDisplay *(*_displayCreator)(int w, int h);
  void (*_displayDestructor)(IDisplay *);
  IDisplay *_display = nullptr;

  void *_handleAudio = nullptr;
  IAudio *(*_audioCreator)();
  void (*_audioDestructor)(IAudio *);
  IAudio *_audio = nullptr;

  std::map<std::string, KeyState> _keyMap;
  Player _fstPlayer;
  Player _sndPlayer;
  float _interval;
  int const _snakeUnit;
  Food _apple;
  Food _meat;

  static std::vector<std::string> _dylibsPaths;

  Snake(void);
  Snake(Snake const &src);

  void _dlerrorWrapper(void);
  void _handleMoveInput(Player &player);
  void _moveSnake(Player &player, int toCrawl);
  void _loadAudio(void);
  void _loadDylib(void);
  void _unloadDylib(void);
  void _foodHandler(Player &player);
  void _placeFood(Food &food);
  void _dropBonusFood(void);
  bool _handlePlayer(Player &player, Player &opponent);
  bool _killPlayer(Player &player);

  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
