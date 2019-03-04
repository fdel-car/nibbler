#pragma once

#define WIDTH 1280
#define HEIGHT 720

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

// GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Player;

// class snake;

class IDisplay {
 public:
  virtual bool windowIsOpen(void) const = 0;
  virtual void renderScene(std::vector<glm::vec2> *snakeCoords1, std::vector<glm::vec2> *snakeCoords2 = nullptr) = 0;
  virtual void setEvents(std::unordered_map<int, bool> *inputsPressed) = 0;

  virtual ~IDisplay(){};
};
