#pragma once

#include <vector>
#include "GLFW/ShaderProgram.hpp"

class GLFWDisplay : public IDisplay {
 public:
  GLFWDisplay(void);
  virtual ~GLFWDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(std::vector<glm::vec2> *snakeCoords1, std::vector<glm::vec2> *snakeCoords2);
  void setEvents(std::unordered_map<int, bool> *inputsPressed);

 private:
  int _width, _height;
  GLuint VAO, VBO;
  GLFWwindow *_window = nullptr;
  ShaderProgram *_shaderProgram = nullptr;

  GLFWDisplay(GLFWDisplay const &src);

  void _initContext(void);
  void _centerWindow(GLFWmonitor *monitor);

  GLFWDisplay &operator=(GLFWDisplay const &rhs);
};

extern "C" {
GLFWDisplay *createDisplay(void);
void deleteDisplay(GLFWDisplay *display);
}
