#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include "IDisplay.hpp"

class SDLDisplay : public IDisplay {
 public:
  SDLDisplay(int w, int h);
  virtual ~SDLDisplay(void);

  bool windowIsOpen(void) const;
  void pollEvent(std::map<std::string, KeyState> &keyMap);
  void renderScene(glm::ivec2 apple,
	  				std::vector<glm::ivec2> const &fstCoords,
                    std::vector<glm::ivec2> const &sndCoords);

 private:
  SDL_Window *_window = NULL;
  SDL_Event _event;
  bool _isOpen = true;
  SDL_Renderer *_renderer = NULL;

  static std::map<ushort, std::string> _keyMap;

  SDLDisplay(void);
  SDLDisplay(SDLDisplay const &src);

  SDLDisplay &operator=(SDLDisplay const &rhs);

  void _drawSnake(std::vector<glm::ivec2> const &snakeCoords);
  void _drawCircle(int _x, int _y, int radius);
  void _drawApple(glm::ivec2 apple);

  static std::map<ushort, std::string> _initKeyMap(void);
};

extern "C" {
SDLDisplay *createDisplay(int w, int h);
void deleteDisplay(SDLDisplay *display);
}
