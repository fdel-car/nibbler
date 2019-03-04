#pragma once

#include <dlfcn.h>
#include <vector>
#include "IDisplay.hpp"

class Snake {
 public:
  Snake(void);
  virtual ~Snake(void);

  void runLoop(void);

 private:
  size_t _dylibIdx;
  void *_handle = nullptr;
  IDisplay *(*_displayCreator)(void);
  void (*_displayDestructor)(IDisplay *);
  IDisplay *_display = nullptr;
  std::map<std::string, KeyState> _keyMap;

  static std::vector<std::string> _dylibsPaths;

  Snake(Snake const &src);

  void _dlerrorWrapper(void);

  static std::vector<std::string> _initDylibsPaths(void);

  Snake &operator=(Snake const &rhs);
};
