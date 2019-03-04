#include "GLFW/GLFWDisplay.hpp"
#include "GLFW/Circle.hpp"

GLFWDisplay::GLFWDisplay(void) {
  _initContext();
  _shaderProgram = new ShaderProgram("./srcs/GLFW/shaders/default.vs",
                                     "./srcs/GLFW/shaders/default.fs");

  glm::mat4 projectionMatrix =
      glm::ortho<float>(0, WIDTH, HEIGHT, 0, 0.1f, 100);
  glm::mat4 viewMatrix =
      glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -10.f));

  glUseProgram(_shaderProgram->getID());
  _shaderProgram->setMat4("VP", projectionMatrix * viewMatrix);
}

GLFWDisplay::~GLFWDisplay(void) {
  if (_shaderProgram) delete _shaderProgram;
  glfwTerminate();
}

void GLFWDisplay::_initContext(void) {
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
  _window = glfwCreateWindow(WIDTH, HEIGHT, "Nibbler - GLFW", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create windows GLFW");
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
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
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
  for (auto key : _keyPressed) keyMap[key].currFrame = true;
  for (auto key : _keyReleased) keyMap[key].currFrame = false;
  _keyPressed.clear();
  _keyReleased.clear();
}

void GLFWDisplay::renderScene(std::vector<glm::vec2> const &fstCoords,
                              std::vector<glm::vec2> const &sndCoords) {
  glClearColor(0.2f, 0.2f, 0.2f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO: Avoid that reinitialization every frame
  for (auto coords : fstCoords) {
    Circle bodyPart(coords);
    bodyPart.render(*_shaderProgram);
  }

  // for (auto coords : sndCoords) {
  //   Circle bodyPart(coords);
  //   bodyPart.render(*_shaderProgram);
  // }

  glfwSwapBuffers(_window);
  (void)sndCoords;
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

  keyMap[GLFW_KEY_W] = "W";
  keyMap[GLFW_KEY_A] = "A";
  keyMap[GLFW_KEY_S] = "S";
  keyMap[GLFW_KEY_D] = "D";

  return keyMap;
}

std::map<ushort, std::string> GLFWDisplay::_keyMap = _initKeyMap();

GLFWDisplay *createDisplay(void) { return new GLFWDisplay(); }

void deleteDisplay(GLFWDisplay *display) { delete display; }
