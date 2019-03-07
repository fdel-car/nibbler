#include "SFML/SFMLDisplay.hpp"

SFMLDisplay::SFMLDisplay(int w, int h)
    : _window(sf::VideoMode(w, h), "Nibbler - SFML", sf::Style::Close) {
  // Center window
  _window.setPosition(sf::Vector2i(
      sf::VideoMode::getDesktopMode().width * 0.5f - _window.getSize().x * 0.5f,
      sf::VideoMode::getDesktopMode().height * 0.5f -
          _window.getSize().y * 0.5f));
  _window.setFramerateLimit(60);

  _food = sf::CircleShape(__GAME_LENGTH_UNIT__ / 2.f);
}

SFMLDisplay::~SFMLDisplay(void) {}

bool SFMLDisplay::windowIsOpen(void) const { return _window.isOpen(); }

void SFMLDisplay::pollEvent(std::map<std::string, KeyState> &keyMap) {
  while (_window.pollEvent(_event)) {
    if (_event.type == sf::Event::Closed) _window.close();

    if (_event.type == sf::Event::KeyPressed ||
        _event.type == sf::Event::KeyReleased) {
      auto it = _keyMap.find(_event.key.code);
      if (it == _keyMap.end()) continue;
      if (_event.type == sf::Event::KeyReleased)
        keyMap[it->second].currFrame = false;
      else {
        if (it->second == "ESC") _window.close();
        keyMap[it->second].currFrame = true;
      }
    }
  }
}

void SFMLDisplay::_drawSnake(std::vector<glm::ivec2> const &snakeCoords,
                             std::vector<sf::CircleShape> *bodySnake) {
  size_t bodySize = bodySnake->size();
  for (size_t i = 0; i < snakeCoords.size() - bodySize; i++) {
    bodySnake->push_back(sf::CircleShape(__GAME_LENGTH_UNIT__ / 2.f));
  }
  for (size_t i = 0; i < snakeCoords.size(); i++) {
    bodySnake->at(i).setPosition(snakeCoords.at(i).x, snakeCoords.at(i).y);
    _window.draw(bodySnake->at(i));
  }
}

void SFMLDisplay::_drawFood(glm::ivec2 const &appleCoords, sf::Color const color) {
  _food.setPosition(appleCoords.x, appleCoords.y);
  _food.setFillColor(color);
  _window.draw(_food);
}

void SFMLDisplay::renderScene(glm::ivec2 const &appleCoords,
							  glm::ivec2 const &bonusFoodCoords,
                              SharedData const &fstData,
                              SharedData const &sndData) {
  _window.clear(sf::Color(51, 51, 51, 255));
  _drawFood(appleCoords, sf::Color(255, 0, 0));
  _drawFood(bonusFoodCoords, sf::Color(255, 255, 0));
  if (fstData.bodyParts.size() != 0) _drawSnake(fstData.bodyParts, &_fstBody);
  if (sndData.bodyParts.size() != 0) _drawSnake(sndData.bodyParts, &_sndBody);
  _window.display();
}

std::map<ushort, std::string> SFMLDisplay::_initKeyMap(void) {
  std::map<ushort, std::string> keyMap;

  keyMap[sf::Keyboard::Escape] = "ESC";
  keyMap[sf::Keyboard::W] = "W";
  keyMap[sf::Keyboard::A] = "A";
  keyMap[sf::Keyboard::S] = "S";
  keyMap[sf::Keyboard::D] = "D";
  keyMap[sf::Keyboard::Up] = "UP";
  keyMap[sf::Keyboard::Left] = "LEFT";
  keyMap[sf::Keyboard::Down] = "DOWN";
  keyMap[sf::Keyboard::Right] = "RIGHT";
  keyMap[sf::Keyboard::Num1] = "1";
  keyMap[sf::Keyboard::Num2] = "2";
  keyMap[sf::Keyboard::Num3] = "3";

  return keyMap;
}

std::map<ushort, std::string> SFMLDisplay::_keyMap = _initKeyMap();

SFMLDisplay *createDisplay(int w, int h) { return new SFMLDisplay(w, h); }

void deleteDisplay(SFMLDisplay *display) { delete display; }
