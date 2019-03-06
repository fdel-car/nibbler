#include "Snake.hpp"

Snake::Snake(Config config)
    : _config(config),
      _dylibIdx(0),
      // _dylibIdx(rand() % _dylibsPaths.size()),
      _interval(0.2f),
      _snakeUnit(30) {
  _newDylibIdx = _dylibIdx;
  int xPos = _config.width / 3;
  int yPos = _config.height / 3;

  for (ushort i = 0; i < 4; i++) {
    _fstPlayer.bodyParts.push_back(
        glm::ivec2(xPos * _snakeUnit, (yPos + i) * _snakeUnit));
    _fstPlayer.allDirs.push_back("UP");
  }

  if (_config.twoPlayers) {
    for (ushort i = 0; i < 4; i++) {
      _sndPlayer.bodyParts.push_back(
          glm::ivec2(xPos * 2 * _snakeUnit, (yPos * 2 + i) * _snakeUnit));
      _sndPlayer.allDirs.push_back("UP");
    }
    _sndPlayer.keys.up = "UP";
    _sndPlayer.keys.left = "LEFT";
    _sndPlayer.keys.down = "DOWN";
    _sndPlayer.keys.right = "RIGHT";
  }

  _placeApple();
  _loadDylib();
}

void Snake::_loadDylib(void) {
  _dylibIdx = _newDylibIdx;
  _handle = dlopen(_dylibsPaths[_dylibIdx].c_str(), RTLD_LAZY);
  if (!_handle) _dlerrorWrapper();
  _displayCreator =
      (IDisplay * (*)(int w, int h)) dlsym(_handle, "createDisplay");
  if (!_displayCreator) _dlerrorWrapper();
  _displayDestructor = (void (*)(IDisplay *))dlsym(_handle, "deleteDisplay");
  if (!_displayDestructor) _dlerrorWrapper();
  _display =
      _displayCreator(_config.width * _snakeUnit, _config.height * _snakeUnit);
  runLoop();
}

void Snake::_unloadDylib(void) {
  if (_display && _handle) {
    _keyMap.clear();
    _displayDestructor(_display);
    dlclose(_handle);
  }
}

Snake::~Snake(void) { _unloadDylib(); }

void Snake::runLoop(void) {
  if (!_display)
    throw std::runtime_error(
        "The graphical interface was not initialzed correctly.");
  prevTime = std::chrono::high_resolution_clock::now();
  while (_display->windowIsOpen()) {
    if (_newDylibIdx != _dylibIdx) break;

    currTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                    currTime - prevTime)
                    .count();

    _display->pollEvent(_keyMap);
    _display->renderScene(_apple.coords, _fstPlayer.bodyParts,
                          _sndPlayer.bodyParts);

    // Switch lib if asked
    if (isKeyPressed("1") && _dylibIdx != 0) _newDylibIdx = 0;
    if (isKeyPressed("2") && _dylibIdx != 1) _newDylibIdx = 1;
    if (isKeyPressed("3") && _dylibIdx != 2) _newDylibIdx = 2;

    _handlePlayer(_fstPlayer);
    if (_config.twoPlayers) _handlePlayer(_sndPlayer);
    prevTime = currTime;
  }
  if (_newDylibIdx != _dylibIdx) {
    _unloadDylib();
    _loadDylib();
  }
}

void Snake::_handlePlayer(Player &player) {
  player.distCrawled += deltaTime * _snakeUnit * (1.f / _interval);
  int toCrawl = (int)player.distCrawled - player.prevCrawled;
  if ((int)player.distCrawled >= _snakeUnit) {
    player.distCrawled = fmod(player.distCrawled, (float)_snakeUnit);
    toCrawl -= (int)player.distCrawled;
    player.prevCrawled = 0;
    if (toCrawl > 0) _moveSnake(player, toCrawl);
    _foodHandler(player);
    _handleMoveInput(player);
  } else {
    if (toCrawl > 0) _moveSnake(player, toCrawl);
    player.prevCrawled += toCrawl;
  }
}

void Snake::_foodHandler(Player &player) {
  if (player.hasEaten) {
    player.bodyParts.push_back(player.newBodyPart);
    player.allDirs.push_back(player.newDir);
    player.hasEaten = false;
  }
  if (player.bodyParts.front() == _apple.coords) {
    player.newBodyPart = player.bodyParts.back();
    player.newDir = player.allDirs.back();
    _placeApple();
    player.hasEaten = true;
  }
}

void Snake::_placeApple(void) {
  size_t mapSize = _config.width * _config.height;
  std::vector<glm::ivec2> freePlaces;

  for (size_t i = 0; i < mapSize; i++) {
    int x = i % _config.width;
    int y = i / _config.height;
    bool collision = false;
    for (auto bodyPart : _fstPlayer.bodyParts)
      if (x == bodyPart.x / _snakeUnit && y == bodyPart.y / _snakeUnit)
        collision = true;
    if (collision) continue;
    for (auto bodyPart : _sndPlayer.bodyParts)
      if (x == bodyPart.x / _snakeUnit && y == bodyPart.y / _snakeUnit)
        collision = true;
    if (collision) continue;
    freePlaces.push_back(glm::ivec2(x * _snakeUnit, y * _snakeUnit));
  }

  if (!freePlaces.size())
    throw std::runtime_error("You won! I've never seen that before...");
  _apple.coords = freePlaces[rand() % freePlaces.size()];
}

bool Snake::isKeyPressed(std::string key) const {
  auto result = _keyMap.find(key);
  return result != _keyMap.end() && result->second.currFrame;
}

bool Snake::isKeyJustPressed(std::string key) const {
  auto result = _keyMap.find(key);
  return result != _keyMap.end() && result->second.currFrame &&
         !result->second.prevFrame;
}

void Snake::_handleMoveInput(Player &player) {
  if (isKeyPressed(player.keys.up) && player.dir != "DOWN")
    player.dir = "UP";
  else if (isKeyPressed(player.keys.left) && player.dir != "RIGHT")
    player.dir = "LEFT";
  else if (isKeyPressed(player.keys.down) && player.dir != "UP")
    player.dir = "DOWN";
  else if (isKeyPressed(player.keys.right) && player.dir != "LEFT")
    player.dir = "RIGHT";
  player.allDirs.insert(player.allDirs.begin(), player.dir);
  player.allDirs.pop_back();
}

void Snake::_moveSnake(Player &player, int toCrawl) {
  for (size_t idx = 0; idx < player.bodyParts.size(); idx++) {
    glm::ivec2 &pos = player.bodyParts[idx];
    std::string dir = player.allDirs[idx];

    if (dir == "UP")
      pos.y -= toCrawl;
    else if (dir == "DOWN")
      pos.y += toCrawl;
    else if (dir == "LEFT")
      pos.x -= toCrawl;
    else if (dir == "RIGHT")
      pos.x += toCrawl;
  }
}

void Snake::_dlerrorWrapper(void) { throw std::runtime_error(dlerror()); }

std::vector<std::string> Snake::_initDylibsPaths(void) {
  std::vector<std::string> vector;

  vector.push_back("./dylibs/SFMLDisplay.so");
  vector.push_back("./dylibs/GLFWDisplay.so");
  vector.push_back("./dylibs/SDLDisplay.so");

  return vector;
}

std::vector<std::string> Snake::_dylibsPaths = _initDylibsPaths();
