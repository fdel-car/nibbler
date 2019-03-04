#pragma once

#define WIDTH 1280
#define HEIGHT 720

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
 public:
  SFMLDisplay(void);
  virtual ~SFMLDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(std::vector<glm::vec2> *snakeBodyCoords);
  void setEvents(std::unordered_map<int, bool> *inputsPressed);

 private:
  int _width, _height;
  sf::RenderWindow _window;
  sf::Event _event;

  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);

  void _setKeyPressed(std::unordered_map<int, bool> *inputsPressed, bool isPressed);

};

extern "C" {
SFMLDisplay *createDisplay(void);
void deleteDisplay(SFMLDisplay *display);
}
