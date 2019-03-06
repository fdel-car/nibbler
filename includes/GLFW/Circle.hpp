#pragma once

#include <vector>
#include "GLFW/Headers.hpp"
#include "GLFW/ShaderProgram.hpp"

class Circle {
 public:
  Circle(float radius);
  Circle(Circle const &src);

  virtual ~Circle(void);

  void render(ShaderProgram const &shaderProgram);
  void setPosition(glm::ivec2 const &pos);
  void setColor(glm::vec3 const &color);
  float getRadius(void) const;
  glm::mat4 const &getModelMatrix(void) const;
  std::vector<glm::vec3> const &getVertices(void) const;

  Circle &operator=(Circle const &rhs);

 private:
  GLuint _VAO, _VBO;
  float _radius;
  std::vector<glm::vec3> _vertices;
  glm::vec3 _color = glm::vec3(1.f);
  glm::mat4 _modelMatrix = glm::mat4(1.f);

  Circle(void);

  void _initBuffers(void);
};
