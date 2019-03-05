#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
 public:
  SFMLDisplay(int w, int h);
  virtual ~SFMLDisplay(void);

  bool windowIsOpen(void) const;
  void pollEvent(std::map<std::string, KeyState> &keyMap);
  void renderScene(std::vector<glm::vec2> const &fstCoords,
                   std::vector<glm::vec2> const &sndCoords);

 private:
  sf::RenderWindow _window;
  sf::Event _event;
  std::vector<sf::CircleShape> _fstBody;
  std::vector<sf::CircleShape> _sndBody;

  static std::map<ushort, std::string> _keyMap;

  SFMLDisplay(void);
  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);

  void _drawSnake(std::vector<glm::vec2> const &snakeCoords,
                  std::vector<sf::CircleShape> *bodySnake);

  static std::map<ushort, std::string> _initKeyMap(void);
};

extern "C" {
SFMLDisplay *createDisplay(int w, int h);
void deleteDisplay(SFMLDisplay *display);
}
