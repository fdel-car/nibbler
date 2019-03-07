#pragma once

#include <vector>
#include "GLFW/Headers.hpp"
#include "GLFW/ShaderProgram.hpp"

class Circle {
 public:
  Circle(float radius);
  Circle(float radius, glm::vec2 const &offset);
  Circle(Circle const &src);

  virtual ~Circle(void);

  void render(ShaderProgram const &shaderProgram);
  void setPosition(glm::ivec2 const &pos);
  void setColor(glm::vec3 const &color);
  float getRadius(void) const;
  glm::vec2 const &getOffset(void) const;
  glm::ivec2 const &getPosition(void) const;
  glm::vec3 const &getColor(void) const;
  glm::mat4 const &getModelMatrix(void) const;
  std::vector<glm::vec2> const &getVertices(void) const;
  void rotate(float angle);

  Circle &operator=(Circle const &rhs);

 private:
  GLuint _VAO, _VBO;
  float _radius;
  glm::vec2 const _offset;
  std::vector<glm::vec2> _vertices;
  glm::ivec2 _position;
  glm::vec3 _color = glm::vec3(1.f);
  glm::mat4 _translationMatrix = glm::mat4(1.f);
  glm::mat4 _rotationMatrix = glm::mat4(1.f);
  glm::mat4 _modelMatrix = glm::mat4(1.f);

  Circle(void);

  void _initVertices(void);
  void _initBuffers(void);
  void _updateModelMatrix(void);
};
