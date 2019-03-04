#include "GLFW/GLFWDisplay.hpp"

GLFWDisplay::GLFWDisplay(void) {
  _initContext();
  _shaderProgram = new ShaderProgram("./srcs/GLFW/shaders/default.vs",
                                     "./srcs/GLFW/shaders/default.fs");

  std::vector<float> vertices = {0.5f, -0.5f, 0.0f, 0.5f, 0.5f,
                                 0.0f, -0.5f, 0.5f, 0.0f};

  // GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices.front(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "GLFW created :)" << std::endl;
}

GLFWDisplay::~GLFWDisplay(void) {
  if (_shaderProgram) delete _shaderProgram;
  glfwTerminate();
  std::cout << "GLFW destroyed :(" << std::endl;
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

  // glfwSetKeyCallback(_window, _keyCallback);
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

void GLFWDisplay::renderScene(void) {
  glfwPollEvents();
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(_shaderProgram->getID());
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwSwapBuffers(_window);
}

GLFWDisplay *createDisplay(void) { return new GLFWDisplay(); }

void deleteDisplay(GLFWDisplay *display) { delete display; }
