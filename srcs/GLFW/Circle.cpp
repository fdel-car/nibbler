#include "GLFW/Circle.hpp"

Circle::Circle(float radius) : _radius(radius), _offset(glm::vec2(radius)) {
  _initVertices();
  _initBuffers();
}

Circle::Circle(float radius, glm::vec2 const &offset)
    : _radius(radius), _offset(offset) {
  _initVertices();
  _initBuffers();
}

Circle::Circle(Circle const &src) : _offset(src.getOffset()) { *this = src; }

Circle &Circle::operator=(Circle const &rhs) {
  _radius = rhs.getRadius();
  _modelMatrix = rhs.getModelMatrix();
  _vertices = rhs.getVertices();
  _color = rhs.getColor();
  _initBuffers();
  return *this;
}

Circle::~Circle(void) {
  glDeleteBuffers(1, &_VBO);
  glDeleteVertexArrays(1, &_VAO);
}

void Circle::_initVertices(void) {
  _vertices = {glm::vec2(_offset)};
  for (size_t idx = 0; idx <= 360; idx += 10) {
    float theta = glm::radians((float)idx);
    _vertices.push_back(glm::vec2(_radius * cos(theta), _radius * sin(theta)) +
                        _vertices.front());
  }
}

void Circle::_initBuffers(void) {
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec2),
               &_vertices.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

float Circle::getRadius(void) const { return _radius; }

glm::vec2 const &Circle::getOffset(void) const { return _offset; }

glm::vec3 const &Circle::getColor(void) const { return _color; }

glm::mat4 const &Circle::getModelMatrix(void) const { return _modelMatrix; }

std::vector<glm::vec2> const &Circle::getVertices(void) const {
  return _vertices;
}

void Circle::rotate(float angle) {
  _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle),
                                glm::vec3(0.f, 0.f, 1.f));
  _updateModelMatrix();
}

glm::ivec2 const &Circle::getPosition(void) const { return _position; }

void Circle::setPosition(glm::ivec2 const &pos) {
  _position = pos;
  _translationMatrix[3][0] = (float)_position.x;
  _translationMatrix[3][1] = (float)_position.y;
  _updateModelMatrix();
}

void Circle::setColor(glm::vec3 const &color) { _color = color; }

void Circle::render(ShaderProgram const &shaderProgram) {
  glUseProgram(shaderProgram.getID());
  shaderProgram.setMat4("M", _modelMatrix);
  shaderProgram.setVec3("color", _color);
  glBindVertexArray(_VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices.size());
}

void Circle::_updateModelMatrix(void) {
  _modelMatrix = _translationMatrix * _rotationMatrix;
}
