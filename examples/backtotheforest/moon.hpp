#ifndef MOON_HPP_
#define MOON_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Moon {
public:
  std::array<glm::vec4, 3> lightProperties();
  float m_shininess = 25.0f;
  glm::vec4 m_lightDir{1.0f, -1.0f, 1.0f, 1.0f};

private:
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

};

#endif