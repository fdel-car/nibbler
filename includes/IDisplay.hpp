#pragma once

#define __GAME_LENGTH_UNIT__ 30

#include <iostream>
#include <map>
#include <vector>

// GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct KeyState {
  bool currFrame;
  bool prevFrame;
};

struct SharedData {
  int dirAngle = 180;
  std::vector<glm::ivec2> bodyParts;
};

class IDisplay {
 public:
  virtual bool windowIsOpen(void) const = 0;
  virtual void pollEvent(std::map<std::string, KeyState> &keyMap) = 0;
  virtual void renderScene(glm::ivec2 const &appleCoords,
                           glm::ivec2 const &meatCoords,
                           SharedData const &fstData,
                           SharedData const &sndData) = 0;

  virtual ~IDisplay(){};
};
