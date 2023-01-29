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
  GLint m_KaLocation{};
  glm::vec4 Ka{};

  GLint m_KdLocation{};
  glm::vec4 Kd{};

  GLint m_KsLocation{};
  glm::vec4 Ks{};

  GLint m_shininessLocation{};
  float shininess = 0.0f;

  GLint m_mappingModeLocation{};
  int mappingMode = 3;

  GLint m_diffuseTexLocation{};

  void materialLoadLocation();
  void materialBindLocation();
  void materialLoadProperties(const std::string, tinyobj::ObjReader);
  void materialLoadDiffuseTexture(std::string_view);
  void materialBindTexture();
  void materialLoadModelFromFile(std::string_view, bool standardize = true);
};

#endif