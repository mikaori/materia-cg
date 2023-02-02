#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "abcgOpenGL.hpp"

class Light {
public:

  // função que retorna a componente de reflexão ambiente
  float *getIa() { return &Ia.x; }

  // função que retorna a componente de reflexão difusa
  float *getId() { return &Id.x; }

  // função que retorna a componente de reflexão especular
  float *getIs() { return &Is.x; }

  // função que retorna a direção da luz
  float *getLightDirection() { return &lightDirection.x; }

  // função que retorna o diametro da luz
  float getLightDiameter() { return lightDiameter; }

  // realiza o bind das variaveis uniformes da luz
  void lightBind();


protected:
  // variáveis para tratamento da reflexão ambiente
  GLint m_IaLocation{};
  glm::vec4 Ia{};

  // variáveis para tratamento da reflexão difusa
  GLint m_IdLocation{};
  glm::vec4 Id{};

  // variáveis para tratamento da reflexão especular
  GLint m_IsLocation{};
  glm::vec4 Is{};

  // variáveis para tratamento da direção da luz
  GLint m_lightDirLocation{};
  glm::vec4 lightDirection{};

  // variáveis para tratamento do diametro da luz
  GLint m_lightDiameterLocation{};
  float lightDiameter{1.0f};
};
#endif