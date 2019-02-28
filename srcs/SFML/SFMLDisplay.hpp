#pragma once

#define WIDTH 1280
#define HEIGHT 720

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
 public:
  SFMLDisplay(void);
  virtual ~SFMLDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(void);

 private:
  int _width, _height;
  sf::RenderWindow _window;
  sf::Event _event;

  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);
};

extern "C" {
SFMLDisplay *createDisplay(void);
void deleteDisplay(SFMLDisplay *display);
}
