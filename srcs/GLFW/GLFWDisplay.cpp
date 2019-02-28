#include "GLFWDisplay.hpp"

GLFWDisplay::GLFWDisplay(void) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
  // glfwWindowHint(GLFW_SAMPLES, 4);
  _window =
      glfwCreateWindow(WIDTH, HEIGHT, "Particle system", nullptr, nullptr);
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
  // glfwSwapInterval(0);  // Remove 60 fps limit from glfw

  // glfwSetKeyCallback(_window, _keyCallback);
  // glfwSetCursorPosCallback(_window, _cursorCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  glViewport(0, 0, _width, _height);

  std::cout << "Created :)" << std::endl;
}

GLFWDisplay::~GLFWDisplay(void) {
  std::cout << "Destroyed :(" << std::endl;
  glfwTerminate();
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

void GLFWDisplay::renderScene(void) const {
  glfwPollEvents();
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(_window);
}

GLFWDisplay *createDisplay(void) { return new GLFWDisplay(); }

void deleteDisplay(GLFWDisplay *display) { delete display; }
