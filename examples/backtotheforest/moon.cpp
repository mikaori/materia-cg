#include "moon.hpp"

void Moon::create() {
  Ia = glm::vec4{0.3f};
  Is = glm::vec4{0.3f};
  Id = glm::vec4{0.3f};
  lightDirection = glm::vec4{0.0f, 100.0f, 0.0f, 1.0f};
  lightDiameter = 1000.0f;
}

void Moon::loadLocation(GLint m_program){
  m_IaLocation = abcg::glGetUniformLocation(m_program, "IaMoon");
  m_IdLocation = abcg::glGetUniformLocation(m_program, "IdMoon");
  m_IsLocation = abcg::glGetUniformLocation(m_program, "IsMoon");
  m_lightDiameterLocation =
      abcg::glGetUniformLocation(m_program, "lightDiameterMoon");
  m_lightDirLocation =
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpaceMoon");
}