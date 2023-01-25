#include "ground.hpp"

void Ground::create(GLuint program) {
  // Unit quad on the xz plane
  std::array<glm::vec3, 4> vertices{{{-SIZE_OF_SIDE, 0.0f, +SIZE_OF_SIDE},
                                     {-SIZE_OF_SIDE, 0.0f, -SIZE_OF_SIDE},
                                     {+SIZE_OF_SIDE, 0.0f, +SIZE_OF_SIDE},
                                     {+SIZE_OF_SIDE, 0.0f, -SIZE_OF_SIDE}}};

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
  m_viewMatrixLoc = abcg::glGetUniformLocation(program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(program, "projMatrix");
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paint(Camera m_camera) {
  abcg::glBindVertexArray(m_VAO);

    // a variável uniforme color é definida como (0.33f, 0.21f, 0.18f, 1.0f) (marrom) no vertex shader
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  // Set model matrix as a translation matrix
  for (int z : iter::range(-NUMBER_SIDE, NUMBER_SIDE + 1)) {
    for (int x : iter::range(-NUMBER_SIDE, NUMBER_SIDE + 1)) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x*SIZE_OF_SIDE, 0.0f, z*SIZE_OF_SIDE));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      abcg::glUniform4f(m_colorLoc, 0.16f, 0.32f, 0.12f, 1.0f);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}