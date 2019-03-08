#include "GLFW/GLFWDisplay.hpp"
#include "GLFW/Circle.hpp"

GLFWDisplay::GLFWDisplay(int w, int h) {
  _initContext(w, h);
  _shaderProgram = new ShaderProgram("./srcs/GLFW/shaders/default.vs",
                                     "./srcs/GLFW/shaders/default.fs");

  glm::mat4 projectionMatrix = glm::ortho<float>(0, w, h, 0, 0.1f, 100);
  glm::mat4 viewMatrix =
      glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -10.f));

  glUseProgram(_shaderProgram->getID());
  _shaderProgram->setMat4("VP", projectionMatrix * viewMatrix);

  _food = new Circle(__GAME_LENGTH_UNIT__ / 2.f);
}

GLFWDisplay::~GLFWDisplay(void) {
  _fstBody.clear();
  _sndBody.clear();
  if (_shaderProgram) delete _shaderProgram;
  if (_food) delete _food;
  glfwTerminate();
}

void GLFWDisplay::_initContext(int w, int h) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  _window = glfwCreateWindow(w, h, "Nibbler - GLFW", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create the GLFW window");
  }

  // Discreetly center window
  glfwHideWindow(_window);
  _centerWindow(glfwGetPrimaryMonitor());
  glfwShowWindow(_window);

  glfwGetFramebufferSize(_window, &_width, &_height);
  glfwMakeContextCurrent(_window);

  glfwSetKeyCallback(_window, _keyCallback);
  // glfwSetCursorPosCallback(_window, _cursorCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");
  // printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  glViewport(0, 0, _width, _height);
}

// https://vallentin.io/2014/02/07/glfw-center-window
void GLFWDisplay::_centerWindow(GLFWmonitor *monitor) {
  if (!monitor) return;
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if (!mode) return;
  int monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);
  int windowWidth, windowHeight;
  glfwGetWindowSize(_window, &windowWidth, &windowHeight);
  glfwSetWindowPos(_window, monitorX + (mode->width - windowWidth) / 2,
                   monitorY + (mode->height - windowHeight) / 2);
}

bool GLFWDisplay::windowIsOpen(void) const {
  return !glfwWindowShouldClose(_window);
}

void GLFWDisplay::pollEvent(std::map<std::string, KeyState> &keyMap) {
  glfwPollEvents();
  for (auto key : _keyPressed) {
    if (key == "ESC") glfwSetWindowShouldClose(_window, GL_TRUE);
    keyMap[key].currFrame = true;
  }
  for (auto key : _keyReleased) keyMap[key].currFrame = false;
  _keyPressed.clear();
  _keyReleased.clear();
}

void GLFWDisplay::_drawSnake(SharedData const &data,
                             std::vector<Circle> &bodySnake) {
  size_t bodySize = bodySnake.size();
  for (size_t i = 0; i < data.bodyParts.size() - bodySize; i++) {
    bodySnake.emplace_back(__GAME_LENGTH_UNIT__ / 2.f);
    bodySnake[i + bodySize].setColor(glm::vec3(0.4f, 0.5f, 0.3f));
  }
  for (size_t i = 0; i < data.bodyParts.size(); i++) {
    bodySnake[i].setPosition(data.bodyParts[i]);
    bodySnake[i].render(*_shaderProgram);
  }
  _drawEyes(bodySnake.front(), data.dirAngle);
}

void GLFWDisplay::_drawFood(glm::ivec2 const &appleCoords,
                            glm::vec3 const color) {
  _food->setPosition(appleCoords);
  _food->setColor(color);
  _food->render(*_shaderProgram);
}

// Should be initialized before, so that we don't create a new Circle on the
// stack each frame
void GLFWDisplay::_drawEyes(Circle const &snakeHead, int const dirAngle) {
  Circle eyeL(
      __GAME_LENGTH_UNIT__ / 5.f,
      glm::vec2(-__GAME_LENGTH_UNIT__ / 6.f, -__GAME_LENGTH_UNIT__ / 7.f));
  eyeL.rotate(dirAngle);
  eyeL.setPosition(snakeHead.getPosition() + glm::ivec2(snakeHead.getRadius()));
  eyeL.render(*_shaderProgram);

  Circle eyeR(
      __GAME_LENGTH_UNIT__ / 5.f,
      glm::vec2(__GAME_LENGTH_UNIT__ / 6.f, -__GAME_LENGTH_UNIT__ / 7.f));
  eyeR.rotate(dirAngle);
  eyeR.setPosition(snakeHead.getPosition() + glm::ivec2(snakeHead.getRadius()));
  eyeR.render(*_shaderProgram);

  Circle irisL(
      __GAME_LENGTH_UNIT__ / 12.f,
      glm::vec2(-__GAME_LENGTH_UNIT__ / 6.f, -__GAME_LENGTH_UNIT__ / 4.f));
  irisL.rotate(dirAngle);
  irisL.setPosition(snakeHead.getPosition() +
                    glm::ivec2(snakeHead.getRadius()));
  irisL.setColor(glm::vec3(0.f));
  irisL.render(*_shaderProgram);

  Circle irisR(
      __GAME_LENGTH_UNIT__ / 12.f,
      glm::vec2(__GAME_LENGTH_UNIT__ / 6.f, -__GAME_LENGTH_UNIT__ / 4.f));
  irisR.rotate(dirAngle);
  irisR.setPosition(snakeHead.getPosition() +
                    glm::ivec2(snakeHead.getRadius()));
  irisR.setColor(glm::vec3(0.f));
  irisR.render(*_shaderProgram);
}

void GLFWDisplay::renderScene(glm::ivec2 const &appleCoords,
                              glm::ivec2 const &meatCoords,
                              SharedData const &fstData,
                              SharedData const &sndData) {
  _displayScore(fstData, sndData);
  glClearColor(0.2f, 0.2f, 0.2f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  _drawFood(appleCoords, glm::vec3(0.8f, 0.f, 0.f));
  _drawFood(meatCoords, glm::vec3(1.f, 1.f, 0.f));
  if (fstData.bodyParts.size() != 0) _drawSnake(fstData, _fstBody);
  if (sndData.bodyParts.size() != 0) _drawSnake(sndData, _sndBody);

  glfwSwapBuffers(_window);
}

void GLFWDisplay::_displayScore(SharedData const &fstData,
                                SharedData const &sndData) {
  std::string score = "Nibbler - GLFW | Score: " + fstData.score +
                      (sndData.score.size() ? " - " + sndData.score : "");
  glfwSetWindowTitle(_window, score.c_str());
}

std::vector<std::string> GLFWDisplay::_keyPressed = std::vector<std::string>();

std::vector<std::string> GLFWDisplay::_keyReleased = std::vector<std::string>();

void GLFWDisplay::_keyCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
  auto it = _keyMap.find(key);
  if (it == _keyMap.end()) return;

  if (action == GLFW_PRESS)
    _keyPressed.push_back(it->second);
  else if (action == GLFW_RELEASE)
    _keyReleased.push_back(it->second);

  (void)scancode;
  (void)window;
  (void)mods;
}

std::map<ushort, std::string> GLFWDisplay::_initKeyMap(void) {
  std::map<ushort, std::string> keyMap;

  keyMap[GLFW_KEY_ESCAPE] = "ESC";
  keyMap[GLFW_KEY_W] = "W";
  keyMap[GLFW_KEY_A] = "A";
  keyMap[GLFW_KEY_S] = "S";
  keyMap[GLFW_KEY_D] = "D";
  keyMap[GLFW_KEY_UP] = "UP";
  keyMap[GLFW_KEY_LEFT] = "LEFT";
  keyMap[GLFW_KEY_DOWN] = "DOWN";
  keyMap[GLFW_KEY_RIGHT] = "RIGHT";
  keyMap[GLFW_KEY_1] = "1";
  keyMap[GLFW_KEY_2] = "2";
  keyMap[GLFW_KEY_3] = "3";

  return keyMap;
}

std::map<ushort, std::string> GLFWDisplay::_keyMap = _initKeyMap();

GLFWDisplay *createDisplay(int w, int h) { return new GLFWDisplay(w, h); }

void deleteDisplay(GLFWDisplay *display) { delete display; }
