#pragma once

#include <glad/glad.h>
// Important, the formatter is dumb
#include <GLFW/glfw3.h>

// GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 1280
#define HEIGHT 720

#include "IDisplay.hpp"

class ShaderProgram;

class GLFWDisplay : public IDisplay {
 public:
  GLFWDisplay(void);
  virtual ~GLFWDisplay(void);

  bool windowIsOpen(void) const;
  void renderScene(void);

 private:
  int _width, _height;
  GLFWwindow *_window = nullptr;
  ShaderProgram *_shaderProgram = nullptr;

  GLFWDisplay(GLFWDisplay const &src);

  void _centerWindow(GLFWmonitor *monitor);

  GLFWDisplay &operator=(GLFWDisplay const &rhs);
};

extern "C" {
GLFWDisplay *createDisplay(void);
void deleteDisplay(GLFWDisplay *display);
}
