#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"

class Material : public Model {
public:
  float *getKa() { return &Ka.x; }

  float *getKd() { return &Kd.x; }

  float *getKs() { return &Ks.x; }

  float getShininess() { return shininess; }

  int getMappingMode() { return mappingMode; }

protected:
  // variaveis alocadas para a constante de reflexão ambiente 
  GLint m_KaLocation{};
  glm::vec4 Ka{};

  // variaveis alocadas para a constante de reflexão difusa
  GLint m_KdLocation{};
  glm::vec4 Kd{};

  // variaveis alocadas para a constante de reflexão especular 
  GLint m_KsLocation{};
  glm::vec4 Ks{};

  // variaveis alocadas e definidas para a constante α do termo especular da equação
  GLint m_shininessLocation{};
  float shininess = 0.0f;

  // variaveis alocadas para o modo de mapping definido
  GLint m_mappingModeLocation{};
  int mappingMode = 3;

  // variaveis alocadas para o recurso de textura difusa do modelo
  GLint m_diffuseTexLocation{};

  // recupera a localização das variáveis uniformes no shader 
  void materialLoadLocation();

  // define os valores das variáveis uniformes no shader
  void materialBindLocation();
  
  // carrega as propriedades do material 
  void materialLoadProperties(const std::string, tinyobj::ObjReader);
  
  // carrega a textura difusa
  void materialLoadDiffuseTexture(std::string_view);
  
  // realiza o bind e o mapeamento da textura
  void materialBindTexture();
  
  // realiza o carregamento do material
  void materialLoadModelFromFile(std::string_view, bool standardize = true);
};

#endif