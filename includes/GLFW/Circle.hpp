#pragma once

#include <vector>
#include "GLFW/Headers.hpp"
#include "GLFW/ShaderProgram.hpp"

class Circle {
 public:
  Circle(glm::vec2 const &pos);

  virtual ~Circle(void);

  void render(ShaderProgram const &shaderProgram);

 private:
  GLuint _VAO, _VBO;
  std::vector<glm::vec3> _vertices;

  static const float _radius;

  Circle(void);

  Circle(Circle const &src);

  Circle &operator=(Circle const &rhs);
};
