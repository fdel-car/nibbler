#include "SFML/SFMLDisplay.hpp"

SFMLDisplay::SFMLDisplay(void)
    : _window(sf::VideoMode(WIDTH, HEIGHT), "Nibbler - SFML",
              sf::Style::Close) {}

SFMLDisplay::~SFMLDisplay(void) {}

bool SFMLDisplay::windowIsOpen(void) const { return _window.isOpen(); }

void SFMLDisplay::pollEvent(std::map<std::string, KeyState> &keyMap) {
  while (_window.pollEvent(_event)) {
    if (_event.type == sf::Event::KeyPressed)
      keyMap[_keyMap[_event.key.code]].currFrame = true;
    if (_event.type == sf::Event::KeyReleased)
      keyMap[_keyMap[_event.key.code]].currFrame = false;

    // TODO: Remove
    if (_event.type == sf::Event::Closed ||
        _event.key.code == sf::Keyboard::Escape) {
      _window.close();
    }
  }
}

void SFMLDisplay::_drawSnake(std::vector<glm::vec2> const &snakeCoords,
                             std::vector<sf::CircleShape> *bodySnake) {
  size_t bodySize = bodySnake->size();
  for (size_t i = 0; i < snakeCoords.size() - bodySize; i++) {
    bodySnake->push_back(sf::CircleShape(5.f));
  }
  for (size_t i = 0; i < snakeCoords.size(); i++) {
    bodySnake->at(i).setPosition(snakeCoords.at(i).x, snakeCoords.at(i).y);
    _window.draw(bodySnake->at(i));
  }
}

void SFMLDisplay::renderScene(std::vector<glm::vec2> const &fstCoords,
                              std::vector<glm::vec2> const &sndCoords) {
  _window.clear(sf::Color::Black);
  _drawSnake(fstCoords, &_fstBody);
  if (sndCoords.size() != 0) _drawSnake(sndCoords, &_sndBody);
  _window.display();
}

std::map<ushort, std::string> SFMLDisplay::_initKeyMap(void) {
  std::map<ushort, std::string> keyMap;

  keyMap[sf::Keyboard::W] = "W";
  keyMap[sf::Keyboard::A] = "A";
  keyMap[sf::Keyboard::S] = "S";
  keyMap[sf::Keyboard::D] = "D";

  return keyMap;
}

std::map<ushort, std::string> SFMLDisplay::_keyMap = _initKeyMap();

SFMLDisplay *createDisplay(void) { return new SFMLDisplay(); }

void deleteDisplay(SFMLDisplay *display) { delete display; }
