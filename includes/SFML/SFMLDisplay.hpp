#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
 public:
  SFMLDisplay(void);
  virtual ~SFMLDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(std::vector<glm::vec2> *snakeCoords1, std::vector<glm::vec2> *snakeCoords2);
  void setEvents(std::unordered_map<int, bool> *inputsPressed);

 private:
  int _width, _height;
  sf::RenderWindow _window;
  sf::Event _event;
  std::vector<sf::RectangleShape> _bodySnake1;
  std::vector<sf::RectangleShape> _bodySnake2;

  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);

  void _setKeyPressed(std::unordered_map<int, bool> *inputsPressed, bool isPressed);
  void _drawSnake(std::vector<glm::vec2> *snakeCoords, std::vector<sf::RectangleShape> *bodySnake);
};

extern "C" {
SFMLDisplay *createDisplay(void);
void deleteDisplay(SFMLDisplay *display);
}
