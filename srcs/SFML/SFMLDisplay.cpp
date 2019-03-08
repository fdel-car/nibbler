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
                             std::vector<sf::CircleShape> &bodySnake) {
  size_t bodySize = bodySnake.size();
  if (bodySize > snakeCoords.size()) {
    bodySnake.clear();
    bodySize = 0;
  }
  for (size_t i = 0; i < snakeCoords.size() - bodySize; i++) {
    bodySnake.emplace_back(__GAME_LENGTH_UNIT__ / 2.f);
    bodySnake[i + bodySize].setFillColor(sf::Color(100, 128, 80));
    if (bodySize == 0 && i == 0) {
      bodySnake[i].setOutlineThickness(__GAME_LENGTH_UNIT__ / 20.f);
      bodySnake[i].setOutlineColor(sf::Color::Yellow);
    }
  }
  for (size_t i = 0; i < snakeCoords.size(); i++) {
    bodySnake[i].setPosition(snakeCoords[i].x, snakeCoords[i].y);
    _window.draw(bodySnake[i]);
  }
}

void SFMLDisplay::_drawFood(glm::ivec2 const &appleCoords,
                            sf::Color const color, bool thickness) {
  _food.setPosition(appleCoords.x, appleCoords.y);
  _food.setFillColor(color);
  if (thickness)
    _food.setOutlineThickness(-__GAME_LENGTH_UNIT__ / 12.f);
  else
    _food.setOutlineThickness(0.f);
  _window.draw(_food);
}

void SFMLDisplay::_drawObstacles(std::vector<glm::ivec2> const &obstacles) {
  size_t size = _obstacles.size();
  for (size_t i = 0; i < obstacles.size() - size; i++) {
    _obstacles.push_back(sf::CircleShape(__GAME_LENGTH_UNIT__ / 2.f));
  }
  for (size_t i = 0; i < obstacles.size(); i++) {
    _obstacles[i].setPosition(obstacles[i].x, obstacles[i].y);
    _obstacles[i].setFillColor(sf::Color(128, 128, 128));
    _window.draw(_obstacles[i]);
  }
}

void SFMLDisplay::renderScene(glm::ivec2 const &appleCoords,
                              glm::ivec2 const &meatCoords,
                              SharedData const &fstData,
                              SharedData const &sndData,
                              std::vector<glm::ivec2> const &obstacles) {
  _displayScore(fstData, sndData);
  _window.clear(sf::Color(51, 51, 51, 255));
  _drawObstacles(obstacles);
  _drawFood(appleCoords, sf::Color(202, 10, 0));
  _drawFood(meatCoords, sf::Color(144, 64, 0), true);
  if (fstData.bodyParts.size() != 0) _drawSnake(fstData.bodyParts, _fstBody);
  if (sndData.bodyParts.size() != 0) _drawSnake(sndData.bodyParts, _sndBody);
  _window.display();
}

void SFMLDisplay::_displayScore(SharedData const &fstData,
                                SharedData const &sndData) {
  std::string score = "Nibbler - SFML | Score: " + fstData.score +
                      (sndData.score.size() ? " - " + sndData.score : "");
  _window.setTitle(score);
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
  keyMap[sf::Keyboard::R] = "R";

  return keyMap;
}

std::map<ushort, std::string> SFMLDisplay::_keyMap = _initKeyMap();

SFMLDisplay *createDisplay(int w, int h) { return new SFMLDisplay(w, h); }

void deleteDisplay(SFMLDisplay *display) { delete display; }
