#include "Snake.hpp"

Snake::Snake(Config config)
    : _config(config),
      _dylibIdx(rand() % _dylibsPaths.size()),
      _interval(0.2f),
      _snakeUnit(30.f) {
  _handle = dlopen(_dylibsPaths[_dylibIdx].c_str(), RTLD_LAZY);
  if (!_handle) _dlerrorWrapper();

  _displayCreator =
      (IDisplay * (*)(int w, int h)) dlsym(_handle, "createDisplay");
  if (!_displayCreator) _dlerrorWrapper();

  fstPlayer.bodyParts.push_back(glm::vec2(20 * _snakeUnit, 20 * _snakeUnit));
  fstPlayer.bodyParts.push_back(glm::vec2(20 * _snakeUnit, 21 * _snakeUnit));
  fstPlayer.bodyParts.push_back(glm::vec2(20 * _snakeUnit, 22 * _snakeUnit));
  fstPlayer.bodyParts.push_back(glm::vec2(20 * _snakeUnit, 23 * _snakeUnit));
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");

  if (_config.twoPlayers) {
    sndPlayer.bodyParts.push_back(glm::vec2(40 * _snakeUnit, 40 * _snakeUnit));
    sndPlayer.bodyParts.push_back(glm::vec2(40 * _snakeUnit, 41 * _snakeUnit));
    sndPlayer.bodyParts.push_back(glm::vec2(40 * _snakeUnit, 42 * _snakeUnit));
    sndPlayer.bodyParts.push_back(glm::vec2(40 * _snakeUnit, 43 * _snakeUnit));
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
  }

  _display = _displayCreator(_config.width, _config.height);
}

Snake::~Snake(void) {
  if (_display && _handle) {
    _displayDestructor = (void (*)(IDisplay *))dlsym(_handle, "deleteDisplay");
    if (!_displayDestructor) _dlerrorWrapper();
    _displayDestructor(_display);
    dlclose(_handle);
  }
}

void Snake::runLoop(void) {
  if (!_display)
    throw std::runtime_error(
        "The graphical interface was not initialzed correctly.");
  prevTime = std::chrono::high_resolution_clock::now();
  size_t frameCount = 0;

  while (_display->windowIsOpen()) {
    if (_newDylibIdx != _dylibIdx) break;
    _display->pollEvent(_keyMap);
    _display->renderScene(fstPlayer.bodyParts, sndPlayer.bodyParts);

    currTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                    currTime - prevTime)
                    .count();

    timeElapsed += deltaTime;
    fstPlayer.speed = deltaTime * _snakeUnit * (1.f / _interval);
    if (timeElapsed >= _interval) {
      _handleInput(fstPlayer);
      if (_config.twoPlayers) _handleInput(sndPlayer);
      timeElapsed = 0.0;
      frameCount = 0;
    }
    _moveSnake(fstPlayer);
    prevTime = currTime;
    frameCount++;
  }
  // if (_newDylibIdx != _dylibIdx)
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

void Snake::_handleInput(Player &player) {
  if (isKeyPressed("W") && player.dir != "DOWN")
    player.dir = "UP";
  else if (isKeyPressed("A") && player.dir != "RIGHT")
    player.dir = "LEFT";
  else if (isKeyPressed("S") && player.dir != "UP")
    player.dir = "DOWN";
  else if (isKeyPressed("D") && player.dir != "LEFT")
    player.dir = "RIGHT";

  player.allDirs.insert(player.allDirs.begin(), player.dir);
  player.allDirs.pop_back();
  // if (isKeyPressed("1") && _dylibIdx != 1) _newDylibIdx = 1;
  // if (isKeyPressed("2") && _dylibIdx != 2) _newDylibIdx = 2;
  // if (isKeyPressed("3") && _dylibIdx != 3) _newDylibIdx = 3;
}

void Snake::_moveSnake(Player &player) {
  if (player.dir == "LEFT") player.bodyParts.front().x -= player.speed;
  if (player.dir == "RIGHT") player.bodyParts.front().x += player.speed;
  if (player.dir == "UP") player.bodyParts.front().y -= player.speed;
  if (player.dir == "DOWN") player.bodyParts.front().y += player.speed;

  glm::vec2 prevPos = player.bodyParts.front();
  for (size_t idx = 1; idx < player.bodyParts.size(); idx++) {
    glm::vec2 &pos = player.bodyParts[idx];
    std::string currDir = player.allDirs[idx];
    std::string prevDir = player.allDirs[idx - 1];

    if (currDir == "UP") {
      if (prevPos.y <= pos.y - player.speed)
        pos.y -= player.speed;
      else {
        pos.y = prevPos.y;
        pos.x = prevPos.x + (prevDir == "LEFT" ? _snakeUnit : -_snakeUnit);
      }
    } else if (currDir == "DOWN") {
      if (prevPos.y >= pos.y + player.speed)
        pos.y += player.speed;
      else {
        pos.y = prevPos.y;
        pos.x = prevPos.x + (prevDir == "LEFT" ? _snakeUnit : -_snakeUnit);
      }
    } else if (currDir == "LEFT") {
      if (prevPos.x <= pos.x - player.speed)
        pos.x -= player.speed;
      else {
        pos.x = prevPos.x;
        pos.y = prevPos.y + (prevDir == "UP" ? _snakeUnit : -_snakeUnit);
      }
    } else if (currDir == "RIGHT") {
      if (prevPos.x >= pos.x + player.speed)
        pos.x += player.speed;
      else {
        pos.x = prevPos.x;
        pos.y = prevPos.y + (prevDir == "UP" ? _snakeUnit : -_snakeUnit);
      }
    }
    prevPos = pos;
  }

  // // Debug
  // for (auto dir : player.allDirs) {
  //   std::cout << dir << std::endl;
  // }
  // std::cout << std::endl;
}

void Snake::_dlerrorWrapper(void) { throw std::runtime_error(dlerror()); }

std::vector<std::string> Snake::_initDylibsPaths(void) {
  std::vector<std::string> vector;

  vector.push_back("./dylibs/GLFWDisplay.so");
  // vector.push_back("./dylibs/SDLDisplay.so");
  vector.push_back("./dylibs/SFMLDisplay.so");

  return vector;
}

std::vector<std::string> Snake::_dylibsPaths = _initDylibsPaths();
