#include "SDL/SDLDisplay.hpp"

SDLDisplay::SDLDisplay(int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw std::runtime_error(std::string("Failed to initialize the SDL, ") +
                             SDL_GetError());
  _window = SDL_CreateWindow("Nibbler - SDL", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  if (!_window)
    throw std::runtime_error(std::string("Failed to create the SDL window, ") +
                             SDL_GetError());

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (!_renderer)
    throw std::runtime_error(
        std::string("Failed to initialize the SDL renderer, ") +
        SDL_GetError());
}

SDLDisplay::~SDLDisplay(void) {
  if (_renderer) SDL_DestroyRenderer(_renderer);
  if (_window) SDL_DestroyWindow(_window);
  SDL_Quit();
}

bool SDLDisplay::windowIsOpen(void) const { return _isOpen; }

void SDLDisplay::pollEvent(std::map<std::string, KeyState> &keyMap) {
  while (SDL_PollEvent(&_event)) {
    if (_event.type == SDL_QUIT) _isOpen = false;

    if (_event.type == SDL_KEYDOWN || _event.type == SDL_KEYUP) {
      auto it = _keyMap.find(_event.key.keysym.sym);
      if (it == _keyMap.end()) continue;
      if (_event.type == SDL_KEYDOWN) {
        if (it->second == "ESC") _isOpen = false;
        keyMap[it->second].currFrame = true;
      } else
        keyMap[it->second].currFrame = false;
    }
  }
}

void SDLDisplay::_drawCircle(int _x, int _y, int radius) {
  int x = radius - 1;
  int y = 0;
  int tx = 1;
  int ty = 1;
  int err = tx - (radius << 1);  // Shifting bits left by 1 effectively
                                 // doubles the value. == tx - diameter
  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(_renderer, (_x + radius) + x, (_y + radius) - y);
    SDL_RenderDrawPoint(_renderer, (_x + radius) + x, (_y + radius) + y);
    SDL_RenderDrawPoint(_renderer, (_x + radius) - x, (_y + radius) - y);
    SDL_RenderDrawPoint(_renderer, (_x + radius) - x, (_y + radius) + y);
    SDL_RenderDrawPoint(_renderer, (_x + radius) + y, (_y + radius) - x);
    SDL_RenderDrawPoint(_renderer, (_x + radius) + y, (_y + radius) + x);
    SDL_RenderDrawPoint(_renderer, (_x + radius) - y, (_y + radius) - x);
    SDL_RenderDrawPoint(_renderer, (_x + radius) - y, (_y + radius) + x);
    if (err <= 0) {
      y++;
      err += ty;
      ty += 2;
    }
    if (err > 0) {
      x--;
      tx += 2;
      err += tx - (radius << 1);
    }
  }
}

void SDLDisplay::_drawSnake(std::vector<glm::ivec2> const &snakeCoords) {
  for (size_t i = 0; i < snakeCoords.size(); i++) {
    _drawCircle(snakeCoords.at(i).x, snakeCoords.at(i).y,
                __GAME_LENGTH_UNIT__ / 2.f);
  }
}

void SDLDisplay::_drawFood(glm::ivec2 const &appleCoords) {
  _drawCircle(appleCoords.x, appleCoords.y, __GAME_LENGTH_UNIT__ / 2.f);
}

void SDLDisplay::renderScene(glm::ivec2 const &appleCoords,
                             glm::ivec2 const &meatCoords,
                             SharedData const &fstData,
                             SharedData const &sndData) {
  SDL_RenderClear(_renderer);
  SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
  _drawFood(appleCoords);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
  _drawFood(meatCoords);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
  if (fstData.bodyParts.size() != 0) _drawSnake(fstData.bodyParts);
  if (sndData.bodyParts.size() != 0) _drawSnake(sndData.bodyParts);
  SDL_SetRenderDrawColor(_renderer, 51, 51, 51, 255);
  SDL_RenderPresent(_renderer);
}

std::map<ushort, std::string> SDLDisplay::_initKeyMap(void) {
  std::map<ushort, std::string> keyMap;

  keyMap[SDLK_ESCAPE] = "ESC";
  keyMap[SDLK_w] = "W";
  keyMap[SDLK_a] = "A";
  keyMap[SDLK_s] = "S";
  keyMap[SDLK_d] = "D";
  keyMap[SDL_SCANCODE_UP] = "UP";
  keyMap[SDL_SCANCODE_LEFT] = "LEFT";
  keyMap[SDL_SCANCODE_DOWN] = "DOWN";
  keyMap[SDL_SCANCODE_RIGHT] = "RIGHT";
  keyMap[SDLK_1] = "1";
  keyMap[SDLK_2] = "2";
  keyMap[SDLK_3] = "3";

  return keyMap;
}

std::map<ushort, std::string> SDLDisplay::_keyMap = _initKeyMap();

SDLDisplay *createDisplay(int w, int h) { return new SDLDisplay(w, h); }

void deleteDisplay(SDLDisplay *display) { delete display; }
