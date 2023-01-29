#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "abcgOpenGL.hpp"

class Light {
public:
  float *getIa() { return &Ia.x; }
  float *getId() { return &Id.x; }
  float *getIs() { return &Is.x; }
  float *getLightDirection() { return &lightDirection.x; }

  float getLightDiameter() { return lightDiameter; }

  void lightBind();


protected:
  GLint m_IaLocation{};
  glm::vec4 Ia{};

  GLint m_IdLocation{};
  glm::vec4 Id{};

  GLint m_IsLocation{};
  glm::vec4 Is{};

  GLint m_lightDirLocation{};
  glm::vec4 lightDirection{};

  GLint m_lightDiameterLocation{};
  float lightDiameter{1.0f};
};
#endif