#include "GLFW/Circle.hpp"

Circle::Circle(float radius) : _radius(radius) {
  _vertices = {glm::vec3(_radius, _radius, 0.f)};
  for (size_t idx = 0; idx <= 360; idx++) {
    float theta = glm::radians((float)idx);
    _vertices.push_back(
        glm::vec3(_radius * cos(theta), _radius * sin(theta), 0.f) +
        _vertices.front());
  }
  _initBuffers();
}

Circle::Circle(Circle const &src) { *this = src; }

Circle &Circle::operator=(Circle const &rhs) {
  _radius = rhs.getRadius();
  _modelMatrix = rhs.getModelMatrix();
  _vertices = rhs.getVertices();
  _initBuffers();
  return *this;
}

Circle::~Circle(void) {}

void Circle::_initBuffers(void) {
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

float Circle::getRadius(void) const { return _radius; }

glm::mat4 const &Circle::getModelMatrix(void) const { return _modelMatrix; }

std::vector<glm::vec3> const &Circle::getVertices(void) const {
  return _vertices;
}

void Circle::setPosition(glm::ivec2 const &pos) {
  _modelMatrix[3][0] = (float)pos.x;
  _modelMatrix[3][1] = (float)pos.y;
}

void Circle::render(ShaderProgram const &shaderProgram) {
  glUseProgram(shaderProgram.getID());
  shaderProgram.setMat4("M", _modelMatrix);
  glBindVertexArray(_VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices.size());
}
