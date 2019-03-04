#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
 public:
  SFMLDisplay(void);
  virtual ~SFMLDisplay(void);

  bool windowIsOpen(void) const;
  void pollEvent(std::map<std::string, KeyState> &keyMap);
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
