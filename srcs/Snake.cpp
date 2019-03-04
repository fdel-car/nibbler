#include "Snake.hpp"

Snake::Snake(void) : _dylibIdx(rand() % _dylibsPaths.size()) {
  _handle = dlopen(_dylibsPaths[_dylibIdx].c_str(), RTLD_LAZY);
  if (!_handle) _dlerrorWrapper();

  _displayCreator = (IDisplay * (*)(void)) dlsym(_handle, "createDisplay");
  if (!_displayCreator) _dlerrorWrapper();

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
  while (_display->windowIsOpen()) {
    _display->renderScene();
  }
}

void Snake::_dlerrorWrapper(void) { throw std::runtime_error(dlerror()); }

std::vector<std::string> Snake::_initDylibsPaths(void) {
  std::vector<std::string> vector;

  vector.push_back("./dylibs/GLFWDisplay.so");
  // vector.push_back("./dylibs/SFMLDisplay.so");

  return vector;
}

std::vector<std::string> Snake::_dylibsPaths = _initDylibsPaths();
