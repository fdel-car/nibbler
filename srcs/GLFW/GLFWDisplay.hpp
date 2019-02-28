#pragma once

#include <glad/glad.h>
// Important, the formatter is dumb
#include <GLFW/glfw3.h>

#define WIDTH 1280
#define HEIGHT 720

#include "IDisplay.hpp"

class GLFWDisplay : public IDisplay {
 public:
  GLFWDisplay(void);
  virtual ~GLFWDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(void) const;

 private:
  int _width, _height;
  GLFWwindow *_window = nullptr;

  GLFWDisplay(GLFWDisplay const &src);

  void _centerWindow(GLFWmonitor *monitor);

  GLFWDisplay &operator=(GLFWDisplay const &rhs);
};

extern "C" {
GLFWDisplay *createDisplay(void);
void deleteDisplay(GLFWDisplay *display);
}
