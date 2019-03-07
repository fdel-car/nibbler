#pragma once

#include <vector>

#include "GLFW/Circle.hpp"
#include "GLFW/Headers.hpp"
#include "GLFW/ShaderProgram.hpp"

class GLFWDisplay : public IDisplay {
 public:
  GLFWDisplay(int w, int h);
  virtual ~GLFWDisplay(void);

  bool windowIsOpen(void) const;
  void pollEvent(std::map<std::string, KeyState> &keyMap);
  void renderScene(glm::ivec2 const &appleCoords,
	  			   glm::ivec2 const &bonusFoodCoords,
                   std::vector<glm::ivec2> const &fstCoords,
                   std::vector<glm::ivec2> const &sndCoords);

 private:
  int _width, _height;
  GLFWwindow *_window = nullptr;
  ShaderProgram *_shaderProgram = nullptr;
  // glm::mat4 _projectionMatrix, viewMatrix;
  Circle *_food = nullptr;
  std::vector<Circle> _fstBody;
  std::vector<Circle> _sndBody;

  static std::vector<std::string> _keyPressed;
  static std::vector<std::string> _keyReleased;
  static std::map<ushort, std::string> _keyMap;

  GLFWDisplay(void);
  GLFWDisplay(GLFWDisplay const &src);

  void _initContext(int w, int h);
  void _centerWindow(GLFWmonitor *monitor);
  void _drawSnake(std::vector<glm::ivec2> const &snakeCoords,
                  std::vector<Circle> *bodySnake);
 void _drawFood(glm::ivec2 const &appleCoords, glm::vec3 const color);

  static void _keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static std::map<ushort, std::string> _initKeyMap(void);

  GLFWDisplay &operator=(GLFWDisplay const &rhs);
};

extern "C" {
GLFWDisplay *createDisplay(int w, int h);
void deleteDisplay(GLFWDisplay *display);
}
