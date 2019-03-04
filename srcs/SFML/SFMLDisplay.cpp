#include "SFML/SFMLDisplay.hpp"

SFMLDisplay::SFMLDisplay(void)
    : _window(sf::VideoMode(WIDTH, HEIGHT), "Nibbler - SFML",
              sf::Style::Close) {
  std::cout << "SFML created :)" << std::endl;
}

SFMLDisplay::~SFMLDisplay(void) {
  std::cout << "SFML destroyed :(" << std::endl;
}

bool SFMLDisplay::windowIsOpen(void) const { return _window.isOpen(); }

void SFMLDisplay::renderScene(std::vector<glm::vec2> *snakeCoords1, std::vector<glm::vec2> *snakeCoords2) {
  _window.clear(sf::Color::Black);
  _drawSnake(snakeCoords1, &_bodySnake1);
  if (snakeCoords2 != nullptr)
  	_drawSnake(snakeCoords2, &_bodySnake2);
  _window.display();
}

void SFMLDisplay::_drawSnake(std::vector<glm::vec2> *snakeCoords, std::vector<sf::RectangleShape> *bodySnake) {
	size_t bodySize = bodySnake->size();
	for (size_t i = 0; i < snakeCoords->size() - bodySize; i++) {
  	  bodySnake->push_back(sf::RectangleShape(sf::Vector2f(50.f, 50.f)));
    }
    for (size_t i = 0; i < snakeCoords->size(); i++) {
  	  bodySnake->at(i).setPosition(snakeCoords->at(i).x, snakeCoords->at(i).y);
  	  _window.draw(bodySnake->at(i));
    }
}

void SFMLDisplay::setEvents(std::unordered_map<int, bool> *inputsPressed) {
  while (_window.pollEvent(_event)) {
	if (_event.type == sf::Event::KeyPressed)
	  _setKeyPressed(inputsPressed, true);
    if (_event.type == sf::Event::KeyReleased)
  	  _setKeyPressed(inputsPressed, false);
    if (_event.type == sf::Event::Closed || _event.key.code == sf::Keyboard::Escape) {
	  _window.close();
    }
  }
}

void SFMLDisplay::_setKeyPressed(std::unordered_map<int, bool> *inputsPressed, bool isPressed) {
  int keyValue = static_cast<int>(_event.text.unicode);

  if (inputsPressed->find(keyValue) == inputsPressed->end())
    inputsPressed->insert(std::pair<int, bool>(keyValue, isPressed));
  else
    inputsPressed->at(keyValue) = isPressed;
  std::cout << "ASCII character typed: " << keyValue << " " << isPressed << std::endl;
}

SFMLDisplay *createDisplay(void) { return new SFMLDisplay(); }

void deleteDisplay(SFMLDisplay *display) { delete display; }
