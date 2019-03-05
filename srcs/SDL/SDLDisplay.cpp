#include "SDL/SDLDisplay.hpp"

SDLDisplay::SDLDisplay(int w, int h) {
  /* Initialisation simple */
  std::cout << "SDL start" << std::endl;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Échec de l'initialisation de la SDL " << SDL_GetError()
              << std::endl;
    return;
  }

  _window = SDL_CreateWindow("Nibbler - SDL", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  if (!_window) {
    std::cerr << "Erreur de création de la fenêtre: " << SDL_GetError()
              << std::endl;
    return;
  }
  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (!_renderer) {
    std::cerr << "Erreur de création du renderer: " << SDL_GetError()
              << std::endl;
    return;
  }
}

SDLDisplay::~SDLDisplay(void) {
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

bool SDLDisplay::windowIsOpen(void) const { return _isOpen; }

void SDLDisplay::pollEvent(std::map<std::string, KeyState> &keyMap) {
  while (SDL_PollEvent(&_event)) {
    if (_event.type == SDL_KEYDOWN)
      keyMap[_keyMap[_event.key.keysym.sym]].currFrame = true;
    if (_event.type == SDL_KEYUP)
      keyMap[_keyMap[_event.key.keysym.sym]].currFrame = false;

    // TODO: Remove
    if (_event.type == SDL_QUIT) {
      _isOpen = false;
    }
  }
}

void SDLDisplay::_drawCircle(int _x, int _y, int radius) {
  int x = radius - 1;
  int y = 0;
  int tx = 1;
  int ty = 1;
  int err = tx - (radius << 1);  // shifting bits left by 1 effectively
                                 // doubles the value. == tx - diameter
  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(_renderer, _x + x, _y - y);
    SDL_RenderDrawPoint(_renderer, _x + x, _y + y);
    SDL_RenderDrawPoint(_renderer, _x - x, _y - y);
    SDL_RenderDrawPoint(_renderer, _x - x, _y + y);
    SDL_RenderDrawPoint(_renderer, _x + y, _y - x);
    SDL_RenderDrawPoint(_renderer, _x + y, _y + x);
    SDL_RenderDrawPoint(_renderer, _x - y, _y - x);
    SDL_RenderDrawPoint(_renderer, _x - y, _y + x);
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

void SDLDisplay::_drawSnake(std::vector<glm::vec2> const &snakeCoords) {
  for (size_t i = 0; i < snakeCoords.size(); i++) {
    _drawCircle(snakeCoords.at(i).x, snakeCoords.at(i).y, 5);
  }
}

void SDLDisplay::renderScene(std::vector<glm::vec2> const &fstCoords,
                             std::vector<glm::vec2> const &sndCoords) {
  SDL_RenderClear(_renderer);
  // _drawSnake(fstCoords);
  // if (sndCoords.size() != 0) _drawSnake(sndCoords);
  SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

  _drawCircle(200, 200, 10);
  SDL_RenderPresent(_renderer);
  (void)fstCoords;
  (void)sndCoords;
}

std::map<ushort, std::string> SDLDisplay::_initKeyMap(void) {
  std::map<ushort, std::string> keyMap;

  keyMap[SDLK_w] = "W";
  keyMap[SDLK_a] = "A";
  keyMap[SDLK_s] = "S";
  keyMap[SDLK_d] = "D";

  return keyMap;
}

std::map<ushort, std::string> SDLDisplay::_keyMap = _initKeyMap();

SDLDisplay *createDisplay(int w, int h) { return new SDLDisplay(w, h); }

void deleteDisplay(SDLDisplay *display) { delete display; }
