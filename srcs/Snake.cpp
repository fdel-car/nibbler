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
    fstPlayer.bodyParts.push_back(
        glm::ivec2(xPos * _snakeUnit, (yPos + i) * _snakeUnit));
    fstPlayer.allDirs.push_back("UP");
  }

  if (_config.twoPlayers) {
    for (ushort i = 0; i < 4; i++) {
      sndPlayer.bodyParts.push_back(
          glm::ivec2(xPos * 2 * _snakeUnit, (yPos * 2 + i) * _snakeUnit));
      sndPlayer.allDirs.push_back("UP");
    }
  }
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
    _display->pollEvent(_keyMap);
    _display->renderScene(_apple.coord, fstPlayer.bodyParts,sndPlayer.bodyParts);

	// Switch lib if asked
    if (isKeyPressed("1") && _dylibIdx != 0) _newDylibIdx = 0;
    if (isKeyPressed("2") && _dylibIdx != 1) _newDylibIdx = 1;
    if (isKeyPressed("3") && _dylibIdx != 2) _newDylibIdx = 2;

    currTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                    currTime - prevTime)
                    .count();

    // Kind of handlePlayer instructions (should be separated)
    fstPlayer.distCrawled += deltaTime * _snakeUnit * (1.f / _interval);
    int toCrawl = (int)fstPlayer.distCrawled - fstPlayer.prevCrawled;
    if ((int)fstPlayer.distCrawled >= _snakeUnit) {
      fstPlayer.distCrawled = fmod(fstPlayer.distCrawled, (float)_snakeUnit);
      toCrawl -= (int)fstPlayer.distCrawled;
      fstPlayer.prevCrawled = 0;
      if (toCrawl > 0) _moveSnake(fstPlayer, toCrawl);
      _handleMoveInput(fstPlayer);
    } else {
      if (toCrawl > 0) _moveSnake(fstPlayer, toCrawl);
      fstPlayer.prevCrawled += toCrawl;
    }
	
	_prepareFood();
    prevTime = currTime;
  }
  if (_newDylibIdx != _dylibIdx) {
    _unloadDylib();
    _loadDylib();
  }
}

void Snake::_prepareFood(void) {
	if (_apple.coord.x != -1 && _apple.coord.y != -1)
		return ;
	_apple.coord.x = (rand() % _config.width) * _snakeUnit;
	_apple.coord.y = (rand() % _config.height) * _snakeUnit;
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
