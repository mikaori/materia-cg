#include "ground.hpp"

void Ground::create(GLuint program) {
  // Unit quad on the xz plane
  std::array<glm::vec3, 4> vertices{{{-20.0f, 0.0f, +20.0f},
                                     {-20.0f, 0.0f, -20.0f},
                                     {+20.0f, 0.0f, +20.0f},
                                     {+20.0f, 0.0f, -20.0f}}};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paint() {
  abcg::glBindVertexArray(m_VAO);

  // Draw a grid of 2N+1 x 2N+1 tiles on the xz plane, centered around the
  // origin

  // Set model matrix as a translation matrix
  glm::mat4 model{1.0f};
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

  abcg::glUniform4f(m_colorLoc, 0.16f, 0.32f, 0.12f, 1.0f);

  abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  abcg::glBindVertexArray(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}