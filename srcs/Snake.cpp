#include "Snake.hpp"

Snake::Snake(Config config)
    : _config(config),
      _dylibIdx(0),
      // _dylibIdx(rand() % _dylibsPaths.size()),
      _interval(0.2f),
      _snakeUnit(__GAME_LENGTH_UNIT__) {
  _newDylibIdx = _dylibIdx;
  _diagLength =
      sqrt(_config.width * _config.width + _config.height * _config.height);

  int xPos = _config.width / 3;
  int yPos = _config.height / 3;

  for (int i = 3; i >= 0; i--) {
    _fstPlayer.data.bodyParts.push_back(
        glm::ivec2(xPos * _snakeUnit, (yPos + i) * _snakeUnit));
    _fstPlayer.allDirs.push_back(180);
  }

  if (_config.twoPlayers) {
    _sndPlayer.data.dirAngle = 0;
    for (int i = 0; i < 4; i++) {
      _sndPlayer.data.bodyParts.push_back(
          glm::ivec2(xPos * 2 * _snakeUnit, (yPos * 2 + i) * _snakeUnit));
      _sndPlayer.allDirs.push_back(0);
    }
    _sndPlayer.keys.up = "UP";
    _sndPlayer.keys.left = "LEFT";
    _sndPlayer.keys.down = "DOWN";
    _sndPlayer.keys.right = "RIGHT";
  }
  _meat.coords.x = _config.width * _snakeUnit;
  _placeFood(_apple, _meat);
  _loadAudio();
  _loadDylib();
}

void Snake::_loadAudio(void) {
  void *_handleAudio = dlopen("./dylibs/SFMLAudio.so", RTLD_LAZY);
  if (!_handleAudio) _dlerrorWrapper();
  _audioCreator = (IAudio * (*)()) dlsym(_handleAudio, "createAudio");
  if (!_audioCreator) _dlerrorWrapper();
  _audioDestructor = (void (*)(IAudio *))dlsym(_handleAudio, "deleteAudio");
  if (!_audioDestructor) _dlerrorWrapper();
  _audio = _audioCreator();
  _audio->playAudio();
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
  if (_display) _displayDestructor(_display);
  if (_handle) dlclose(_handle);
  _keyMap.clear();
}

Snake::~Snake(void) {
  _unloadDylib();
  if (_audio) _audioDestructor(_audio);
  if (_handleAudio) dlclose(_handleAudio);
}

void Snake::runLoop(void) {
  if (!_display)
    throw std::runtime_error(
        "The graphical interface was not initialzed correctly.");
  prevTime = std::chrono::high_resolution_clock::now();
  bool fstAlive = true;
  bool sndAlive = _config.twoPlayers;

  while (_display->windowIsOpen()) {
    if (_newDylibIdx != _dylibIdx) break;

    currTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(
                    currTime - prevTime)
                    .count();

    _display->pollEvent(_keyMap);
    _display->renderScene(_apple.coords, _meat.coords, _fstPlayer.data,
                          _sndPlayer.data);

    // Switch lib if asked
    if (isKeyPressed("1") && _dylibIdx != 0) _newDylibIdx = 0;
    if (isKeyPressed("2") && _dylibIdx != 1) _newDylibIdx = 1;
    if (isKeyPressed("3") && _dylibIdx != 2) _newDylibIdx = 2;

    if (fstAlive) fstAlive = _handlePlayer(_fstPlayer, _sndPlayer);
    if (sndAlive) sndAlive = _handlePlayer(_sndPlayer, _fstPlayer);
    prevTime = currTime;
  }
  if (_newDylibIdx != _dylibIdx) {
    _unloadDylib();
    _loadDylib();
  }
}

bool Snake::_handlePlayer(Player &player, Player &opponent) {
  if (!player.data.bodyParts.size() || !player.allDirs.size()) return false;

  float crawled = deltaTime * _snakeUnit * (1.f / _interval) * player.speed;
  if (player.boostTimer > 0) crawled *= 1.3f;
  player.distCrawled += crawled;
  int toCrawl = (int)player.distCrawled - player.prevCrawled;
  if ((int)player.distCrawled >= _snakeUnit) {
    if (player.boostTimer > 0) player.boostTimer--;
    player.distCrawled = fmod(player.distCrawled, (float)_snakeUnit);
    toCrawl -= (int)player.distCrawled;
    player.prevCrawled = 0;
    if (toCrawl > 0) _moveSnake(player, toCrawl);
    _foodHandler(player);
    _dropBonusFood();
    _handleMoveInput(player);
  } else {
    if (toCrawl > 0) _moveSnake(player, toCrawl);
    player.prevCrawled += toCrawl;
  }

  glm::ivec2 snakeHead = player.data.bodyParts.front();

  // Players collision
  for (size_t i = 4; i < player.data.bodyParts.size(); i++) {
    glm::ivec2 tmp = snakeHead - player.data.bodyParts[i];
    float distance = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
    if (distance < _snakeUnit / 2) return _killPlayer(player);
  }
  for (size_t i = 0; i < opponent.data.bodyParts.size(); i++) {
    glm::ivec2 tmp = snakeHead - opponent.data.bodyParts[i];
    float distance = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
    if (distance < _snakeUnit / 2) {
      if (i == 0) _killPlayer(opponent);
      return _killPlayer(player);
    }
  }

  // Walls collision
  if (snakeHead.x < 0 || snakeHead.x > (_config.width - 1) * _snakeUnit ||
      snakeHead.y < 0 || snakeHead.y > (_config.height - 1) * _snakeUnit)
    return _killPlayer(player);
  return true;
}

void Snake::_dropBonusFood(void) {
  int tmp = _config.twoPlayers ? 200 : 100;
  if (_meat.coords.x == _config.width * _snakeUnit && (rand() % tmp) == 42) {
    int limit = _diagLength;
    _placeFood(_meat, _apple);
    _meat.lifeTime = _config.twoPlayers ? limit << 1 : limit;
  } else if (_meat.lifeTime > 0)
    _meat.lifeTime--;
  else if (_meat.lifeTime == 0)
    _meat.coords.x = _config.width * _snakeUnit;
}

bool Snake::_killPlayer(Player &player) {
  player.data.bodyParts.clear();
  player.allDirs.clear();
  return true;
}

void Snake::_foodHandler(Player &player) {
  if (player.hasEaten) {
    player.data.bodyParts.push_back(player.newBodyPart);
    player.allDirs.push_back(player.newDirAngle);
    player.hasEaten = false;
  }
  if (player.data.bodyParts.front() == _apple.coords) {
    player.newBodyPart = player.data.bodyParts.back();
    player.newDirAngle = player.allDirs.back();
    player.speed += 0.02f;
    _placeFood(_apple, _meat);
    player.hasEaten = true;
  }
  if (player.data.bodyParts.front() == _meat.coords) {
    player.newBodyPart = player.data.bodyParts.back();
    player.newDirAngle = player.allDirs.back();
    _meat.coords.x = _config.width * _snakeUnit;
    _meat.lifeTime = -1;
    player.speed += 0.04f;
    player.boostTimer += _diagLength >> 1;
    player.hasEaten = true;
  }
}

void Snake::_placeFood(Food &food, Food &otherFood) {
  size_t mapSize = _config.width * _config.height;
  std::vector<glm::ivec2> freePlaces;

  for (size_t i = 0; i < mapSize; i++) {
    int x = i % _config.width;
    int y = i / _config.width;

    bool collision = false;
    for (auto bodyPart : _fstPlayer.data.bodyParts)
      if (x == bodyPart.x / _snakeUnit && y == bodyPart.y / _snakeUnit) {
        collision = true;
        break;
      }
    if (collision) continue;
    for (auto bodyPart : _sndPlayer.data.bodyParts)
      if (x == bodyPart.x / _snakeUnit && y == bodyPart.y / _snakeUnit) {
        collision = true;
        break;
      }
    if (collision) continue;
    if (otherFood.coords.x == x && otherFood.coords.y == y) continue;
    freePlaces.push_back(glm::ivec2(x * _snakeUnit, y * _snakeUnit));
  }

  if (!freePlaces.size())
    throw std::runtime_error("You won! I've never seen that before...");
  food.coords = freePlaces[rand() % freePlaces.size()];
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
  if (isKeyPressed(player.keys.up) && player.data.dirAngle != 180)
    player.data.dirAngle = 0;
  else if (isKeyPressed(player.keys.left) && player.data.dirAngle != 90)
    player.data.dirAngle = 270;
  else if (isKeyPressed(player.keys.down) && player.data.dirAngle != 0)
    player.data.dirAngle = 180;
  else if (isKeyPressed(player.keys.right) && player.data.dirAngle != 270)
    player.data.dirAngle = 90;
  player.allDirs.insert(player.allDirs.begin(), player.data.dirAngle);
  player.allDirs.pop_back();
}

void Snake::_moveSnake(Player &player, int toCrawl) {
  for (size_t idx = 0; idx < player.data.bodyParts.size(); idx++) {
    glm::ivec2 &pos = player.data.bodyParts[idx];
    int dirAngle = player.allDirs[idx];

    if (dirAngle == 0)
      pos.y -= toCrawl;
    else if (dirAngle == 180)
      pos.y += toCrawl;
    else if (dirAngle == 270)
      pos.x -= toCrawl;
    else if (dirAngle == 90)
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
