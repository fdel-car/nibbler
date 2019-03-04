#include "Snake.hpp"

Snake::Snake(void) : _dylibIdx(rand() % _dylibsPaths.size()), _multi(true) {
  _handle = dlopen(_dylibsPaths[_dylibIdx].c_str(), RTLD_LAZY);
  if (!_handle) _dlerrorWrapper();

  _displayCreator = (IDisplay * (*)(void)) dlsym(_handle, "createDisplay");
  if (!_displayCreator) _dlerrorWrapper();

  _P1.snakeBodyCoords.push_back(glm::vec2(100, 100));
  _P1.snakeBodyCoords.push_back(glm::vec2(100, 110));
  _P1.snakeBodyCoords.push_back(glm::vec2(100, 120));
  _P1.snakeBodyCoords.push_back(glm::vec2(100, 130));
  if (_multi) {
	  _P2.id = 2;
	  _P2.snakeBodyCoords.push_back(glm::vec2(130, 100));
	  _P2.snakeBodyCoords.push_back(glm::vec2(120, 100));
	  _P2.snakeBodyCoords.push_back(glm::vec2(110, 100));
	  _P2.snakeBodyCoords.push_back(glm::vec2(100, 100));
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
  clock_t killTime = 0;
  while (_display->windowIsOpen()) {
	if (clock() > killTime) {
		killTime = clock() + (0.05 * CLOCKS_PER_SEC);
		_display->setEvents(&_inputsPressed);
		!_multi ? _display->renderScene(&_P1.snakeBodyCoords) : _display->renderScene(&_P1.snakeBodyCoords, &_P2.snakeBodyCoords);
		_checkInputPressed(&_P1);
		if (_P2.id == 2)
			_checkInputPressed(&_P2);
    }
  }
}

void Snake::_checkInputPressed(Player *player) {
	for (const auto &key : _inputsPressed) {
		if (((player->id == 1 && key.first == LEFT) ||\
				(player->id == 2 && key.first == A)) && key.second == true) {
			player->move = -10;
			player->lastDirectionMove = (player->id == 1 ? LEFT : A);
		}
		if (((player->id == 1 && key.first == RIGHT) ||\
				(player->id == 2 && key.first == D)) && key.second == true) {
			player->move = 10;
			player->lastDirectionMove = (player->id == 1 ? RIGHT : D);
		}
		if (((player->id == 1 && key.first == UP) ||\
				(player->id == 2 && key.first == W)) && key.second == true) {
			player->move = -10;
			player->lastDirectionMove = (player->id == 1 ? UP : W);
		}
		if (((player->id == 1 && key.first == DOWN) ||\
				(player->id == 2 && key.first == S)) && key.second == true) {
			player->move = 10;
			player->lastDirectionMove = (player->id == 1 ? DOWN : S);
		}
	}
	_moveSnake(player);
}

void Snake::_moveSnake(Player *player) {
	glm::vec2 tmp = player->snakeBodyCoords[0];
	if (player->lastDirectionMove == LEFT || player->lastDirectionMove == RIGHT\
			|| player->lastDirectionMove == A || player->lastDirectionMove == D)
		tmp.x += player->move;
	if (player->lastDirectionMove == UP || player->lastDirectionMove == DOWN\
			|| player->lastDirectionMove == W || player->lastDirectionMove == S)
		tmp.y += player->move;
	player->snakeBodyCoords.insert(player->snakeBodyCoords.begin(), tmp);
	player->snakeBodyCoords.pop_back();
}

void Snake::_dlerrorWrapper(void) { throw std::runtime_error(dlerror()); }

std::vector<std::string> Snake::_initDylibsPaths(void) {
  std::vector<std::string> vector;

  vector.push_back("./dylibs/GLFWDisplay.so");
  vector.push_back("./dylibs/SFMLDisplay.so");

  return vector;
}

std::vector<std::string> Snake::_dylibsPaths = _initDylibsPaths();
