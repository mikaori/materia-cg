#include "moon.hpp"

std::array<glm::vec4, 3> Moon::lightProperties(){
  return {m_Ia, m_Id, m_Is};
}