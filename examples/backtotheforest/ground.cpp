#include "ground.hpp"

// inicializa o chão
void Ground::create(GLuint program) {
  
  // unidade do quadrado no plano xz
  std::array<glm::vec3, 4> vertices{{{-SIZE_OF_SIDE, 0.0f, +SIZE_OF_SIDE},
                                     {-SIZE_OF_SIDE, 0.0f, -SIZE_OF_SIDE},
                                     {+SIZE_OF_SIDE, 0.0f, +SIZE_OF_SIDE},
                                     {+SIZE_OF_SIDE, 0.0f, -SIZE_OF_SIDE}}};

   // gera um VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // gera VAO e faz o bind dos atributos dos vértices
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

  // armazena a localização das variáveis uniformes 
  m_viewMatrixLoc = abcg::glGetUniformLocation(program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(program, "projMatrix");
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

// função de desenho do chão
void Ground::paint(Camera m_camera) {

  // vincular atributos de vértice ao VAO atual
  abcg::glBindVertexArray(m_VAO);

  // define variáveis uniformes para viewMatrix e projMatrix do chão
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  // define a matriz modelo como uma matriz de translação
  for (int z : iter::range(-NUMBER_SIDE, NUMBER_SIDE + 1)) {
    for (int x : iter::range(-NUMBER_SIDE, NUMBER_SIDE + 1)) {
      glm::mat4 model{1.0f};
      model = glm::translate(
          model, glm::vec3(x * SIZE_OF_SIDE, 0.0f, z * SIZE_OF_SIDE));
      // define variáveis uniformes para modelMatrix do chão
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      // define variáveis uniformes para a cor do chão
      abcg::glUniform4f(m_colorLoc, 0.16f, 0.32f, 0.12f, 1.0f);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  // fim do bind do VAO atual
  abcg::glBindVertexArray(0);
}

// libera os recursos do OpenGL que foram alocados para o chão
void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}