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
  void renderScene(glm::ivec2 const &appleCoords, SharedData const &fstData,
                   SharedData const &sndData);

 private:
  sf::RenderWindow _window;
  sf::Event _event;
  std::vector<sf::CircleShape> _fstBody;
  std::vector<sf::CircleShape> _sndBody;
  sf::CircleShape _apple;

  static std::map<ushort, std::string> _keyMap;

  SFMLDisplay(void);
  SFMLDisplay(SFMLDisplay const &src);

  SFMLDisplay &operator=(SFMLDisplay const &rhs);

  void _drawSnake(std::vector<glm::ivec2> const &snakeCoords,
                  std::vector<sf::CircleShape> *bodySnake);
  void _drawApple(glm::ivec2 const &appleCoords);
  static std::map<ushort, std::string> _initKeyMap(void);
};

extern "C" {
SFMLDisplay *createDisplay(int w, int h);
void deleteDisplay(SFMLDisplay *display);
}
