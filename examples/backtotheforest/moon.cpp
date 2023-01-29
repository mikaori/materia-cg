#include "moon.hpp"

void Moon::create() {
  Ia = glm::vec4{10.3f};
  Is = glm::vec4{10.3f};
  Id = glm::vec4{10.3f};
  lightDirection = glm::vec4{0.0f, 100.0f, 0.0f, 1.0f};
  lightDiameter = 1000.0f;
}

void Moon::loadLocation(GLint m_program){
  m_IaLocation = abcg::glGetUniformLocation(m_program, "Ia");
  m_IdLocation = abcg::glGetUniformLocation(m_program, "Id");
  m_IsLocation = abcg::glGetUniformLocation(m_program, "Is");
  m_lightDiameterLocation =
      abcg::glGetUniformLocation(m_program, "lightDiameter");
  m_lightDirLocation =
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace");
}