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
  void pollEvent(std::map<std::string, KeyState> &keyMap);
  void renderScene(std::vector<glm::vec2> const &fstCoords,
                   std::vector<glm::vec2> const &sndCoords);

 private:
  int _width, _height;
  sf::RenderWindow _window;
  sf::Event _event;
  std::vector<sf::CircleShape> _fstBody;
  std::vector<sf::CircleShape> _sndBody;

  static std::map<ushort, std::string> _keyMap;

  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);

  void _drawSnake(std::vector<glm::vec2> const &snakeCoords,
                  std::vector<sf::CircleShape> *bodySnake);

  static std::map<ushort, std::string> _initKeyMap(void);
};

extern "C" {
SFMLDisplay *createDisplay(void);
void deleteDisplay(SFMLDisplay *display);
}
