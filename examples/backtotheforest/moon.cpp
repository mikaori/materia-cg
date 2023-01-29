#include "moon.hpp"

void Moon::create() {
  Ia = glm::vec4{0.3f};
  Is = glm::vec4{0.3f};
  Id = glm::vec4{0.3f};
  lightDirection = glm::vec4{0.0f, 100.0f, 0.0f, 1.0f};
  lightDiameter = 1000.0f;
}