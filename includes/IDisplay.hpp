#pragma once

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

class IDisplay {
 public:
  virtual bool windowIsOpen(void) const = 0;
  virtual void renderScene(std::vector<glm::vec2> *snakeBodyCoords) = 0;
  virtual void setEvents(std::unordered_map<int, bool> *inputsPressed) = 0;
};
