#include "dog.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Dog::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Reset dog attributes
  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  // clang-format off
  std::array positions{
      glm::vec2{-08.0f,  00.0f}, glm::vec2{ 00.0f,  00.0f},  glm::vec2{ 00.0f, -08.0f}, glm::vec2{-08.0f, -08.0f},
      glm::vec2{-04.0f, -08.0f}, glm::vec2{-04.0f, -12.0f},
      glm::vec2{-08.0f, -12.0f}, glm::vec2{-12.0f, +04.0f},
      glm::vec2{-12.0f,  00.0f}, glm::vec2{+08.0f,  00.0f},
      glm::vec2{+08.0f, -08.0f}, glm::vec2{+08.0f, -12.0f},
      glm::vec2{+04.0f, -08.0f}, glm::vec2{+04.0f, -12.0f},
      glm::vec2{+12.0f,  00.0f}, glm::vec2{+12.0f, +04.0f},
      glm::vec2{+08.0f, +04.0f}, glm::vec2{+08.0f, +08.0f},
      glm::vec2{+04.0f, +08.0f}, glm::vec2{ 00.0f, +08.0f}
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array const indices{
                            0, 3, 10,
                            0, 9, 10,
                            3, 4, 5,
                            3, 5, 6,
                            0, 7, 8,
                            10, 11, 12,
                            11, 12, 13,
                            9, 14, 15,
                            9, 15, 16,
                            9, 18, 20,
                            2, 9, 20,  
                            };
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Dog::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  // abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Dog::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Dog::update(GameData const &gameData, float deltaTime) {
  // Rotate
  if (gameData.m_input[gsl::narrow<size_t>(Input::Up)])
    m_translation += glm::vec2(0, 1.0f) * deltaTime;
  if (gameData.m_input[gsl::narrow<size_t>(Input::Down)])
    m_translation -= glm::vec2(0, 1.0f) * deltaTime;
  if (gameData.m_input[gsl::narrow<size_t>(Input::Right)])
    m_translation += glm::vec2(1.0f, 0) * deltaTime;
  if (gameData.m_input[gsl::narrow<size_t>(Input::Left)])
    m_translation -= glm::vec2(1.0f, 0) * deltaTime;

  if (m_translation.x <= -1.0f)
    m_translation.x = -1.0f;
  if (m_translation.x >= +1.0f)
    m_translation.x = +1.0f;
  if (m_translation.y <= -1.0f)
    m_translation.y = -1.0f;
  if (m_translation.y >= +1.0f)
    m_translation.y = +1.0f;
}