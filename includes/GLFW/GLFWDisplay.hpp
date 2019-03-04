#pragma once

#include <vector>

#include "GLFW/Headers.hpp"
#include "GLFW/ShaderProgram.hpp"

class GLFWDisplay : public IDisplay {
 public:
  GLFWDisplay(void);
  virtual ~GLFWDisplay(void);

  bool windowIsOpen(void) const;
  void pollEvent(std::map<std::string, KeyState> &keyMap);
  void renderScene(void);

 private:
  int _width, _height;
  GLFWwindow *_window = nullptr;
  ShaderProgram *_shaderProgram = nullptr;
  // glm::mat4 _projectionMatrix, viewMatrix;

  static std::vector<std::string> _keyPressed;
  static std::vector<std::string> _keyReleased;
  static std::map<ushort, std::string> _keyMap;

  GLFWDisplay(GLFWDisplay const &src);

  void _initContext(void);
  void _centerWindow(GLFWmonitor *monitor);

  static void _keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static std::map<ushort, std::string> _initKeyMap(void);

  GLFWDisplay &operator=(GLFWDisplay const &rhs);
};

extern "C" {
GLFWDisplay *createDisplay(void);
void deleteDisplay(GLFWDisplay *display);
}
