#include "GLFW/Circle.hpp"

Circle::Circle(glm::vec2 const &pos) {
  _vertices = {glm::vec3(pos.x, pos.y, 0.f)};
  for (size_t idx = 0; idx < 180; idx++) {
    float theta = 180 / (float)idx;
    _vertices.push_back(
        glm::vec3(_radius * cos(theta), _radius * sin(theta), 0.f) +
        _vertices.front());
  }
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3),
               &_vertices.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Circle::~Circle(void) {}

void Circle::render(ShaderProgram const &shaderProgram) {
  glUseProgram(shaderProgram.getID());
  glBindVertexArray(_VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices.size());
}

const float Circle::_radius = 5.f;
