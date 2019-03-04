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

void SFMLDisplay::renderScene(void) {
  while (_window.pollEvent(_event)) {
    if (_event.type == sf::Event::Closed) {
      _window.close();
    }
  }
  _window.clear(sf::Color::Black);
  _window.display();
}

SFMLDisplay *createDisplay(void) { return new SFMLDisplay(); }

void deleteDisplay(SFMLDisplay *display) { delete display; }
