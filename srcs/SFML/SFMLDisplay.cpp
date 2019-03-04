#include "SFML/SFMLDisplay.hpp"

SFMLDisplay::SFMLDisplay(void)
    : _window(sf::VideoMode(800, 600), "Nibbler - SFML", sf::Style::Close) {
  std::cout << "SFML created :)" << std::endl;
}

SFMLDisplay::~SFMLDisplay(void) {
  std::cout << "SFML destroyed :(" << std::endl;
}

bool SFMLDisplay::windowIsOpen(void) const { return _window.isOpen(); }

void SFMLDisplay::renderScene(std::vector<glm::vec2> *snakeBodyCoords) {
	(void)snakeBodyCoords;

  _window.clear(sf::Color::Black);
  _window.display();
}

void SFMLDisplay::setEvents(std::unordered_map<int, bool> *inputsPressed) {
  while (_window.pollEvent(_event)) {
    if (_event.type == sf::Event::KeyPressed)
	  _setKeyPressed(inputsPressed, true);
    if (_event.type == sf::Event::KeyReleased)
  	  _setKeyPressed(inputsPressed, false);
    else
	  std::cout << "unpressed" << std::endl;
    if (_event.type == sf::Event::Closed) {
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
