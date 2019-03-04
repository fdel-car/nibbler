#include "Snake.hpp"

Snake::Snake(void)
    : _dylibIdx(rand() % _dylibsPaths.size()), _twoPlayers(false) {
  _handle = dlopen(_dylibsPaths[_dylibIdx].c_str(), RTLD_LAZY);
  if (!_handle) _dlerrorWrapper();

  _displayCreator = (IDisplay * (*)(void)) dlsym(_handle, "createDisplay");
  if (!_displayCreator) _dlerrorWrapper();

  fstPlayer.bodyParts.push_back(glm::vec2(400, 200));
  fstPlayer.bodyParts.push_back(glm::vec2(400, 250));
  fstPlayer.bodyParts.push_back(glm::vec2(400, 300));
  fstPlayer.bodyParts.push_back(glm::vec2(400, 350));
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");
  fstPlayer.allDirs.push_back("UP");

  if (_twoPlayers) {
    sndPlayer.bodyParts.push_back(glm::vec2(130, 100));
    sndPlayer.bodyParts.push_back(glm::vec2(120, 100));
    sndPlayer.bodyParts.push_back(glm::vec2(110, 100));
    sndPlayer.bodyParts.push_back(glm::vec2(100, 100));
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
    sndPlayer.allDirs.push_back("UP");
  }

  _display = _displayCreator();
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
  // clock_t updateTime = 0;
  while (_display->windowIsOpen()) {
    _display->pollEvent(_keyMap);
    _display->renderScene(fstPlayer.bodyParts, sndPlayer.bodyParts);
    // if (clock() > updateTime) {
    // updateTime = clock() + (0.05 * CLOCKS_PER_SEC);
    _handleInput(fstPlayer);
    // if (_twoPlayers) _handleInput(&sndPlayer);
    // }
  }
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
  if (isKeyPressed("W") && player.dir != "DOWN") player.dir = "UP";
  if (isKeyPressed("A") && player.dir != "RIGHT") player.dir = "LEFT";
  if (isKeyPressed("S") && player.dir != "UP") player.dir = "DOWN";
  if (isKeyPressed("D") && player.dir != "LEFT") player.dir = "RIGHT";
  _moveSnake(player);
}

void Snake::_moveSnake(Player &player) {
  if (player.dir == "LEFT") player.bodyParts.front().x -= 1;
  if (player.dir == "RIGHT") player.bodyParts.front().x += 1;
  if (player.dir == "UP") player.bodyParts.front().y -= 1;
  if (player.dir == "DOWN") player.bodyParts.front().y += 1;
  player.allDirs.front() = player.dir;

  glm::vec2 prevPos = player.bodyParts.front();
  for (size_t idx = 1; idx < player.bodyParts.size(); idx++) {
    glm::vec2 &pos = player.bodyParts[idx];
    std::string &dir = player.allDirs[idx];

    if (dir == "UP") {
      if (prevPos.y < pos.y)
        pos.y -= 1;
      else {
        dir = player.allDirs[idx - 1];
        pos.x += dir == "LEFT" ? -1 : 1;
      }
    } else if (dir == "DOWN") {
      if (prevPos.y > pos.y)
        pos.y += 1;
      else {
        dir = player.allDirs[idx - 1];
        pos.x += dir == "LEFT" ? -1 : 1;
      }
    } else if (dir == "LEFT") {
      if (prevPos.x < pos.x)
        pos.x -= 1;
      else {
        dir = player.allDirs[idx - 1];
        pos.y += dir == "UP" ? -1 : 1;
      }
    } else if (dir == "RIGHT") {
      if (prevPos.x > pos.x)
        pos.x += 1;
      else {
        dir = player.allDirs[idx - 1];
        pos.y += dir == "UP" ? -1 : 1;
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
  vector.push_back("./dylibs/SFMLDisplay.so");

  return vector;
}

std::vector<std::string> Snake::_dylibsPaths = _initDylibsPaths();
