#pragma once

#define WIDTH 1280
#define HEIGHT 720

#include <iostream>
#include <map>
#include <unordered_map>

// GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct KeyState {
  bool currFrame;
  bool prevFrame;
};

class IDisplay {
 public:
  virtual bool windowIsOpen(void) const = 0;
  virtual void pollEvent(std::map<std::string, KeyState> &keyMap) = 0;
  virtual void renderScene(glm::ivec2 const &appleCoords,
                           std::vector<glm::ivec2> const &fstCoords,
                           std::vector<glm::ivec2> const &sndCoords) = 0;

  virtual ~IDisplay(){};
};
