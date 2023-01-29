#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

class Model {
public:
  void modelComputeNormals();
  void modelStandardize();
  void modelLoadModelFromFile(
      tinyobj::ObjReader reader,
      bool standardize = true); // carregamento do arquivo OBJ
protected:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};
  GLuint m_diffuseTexture{};

  std::vector<Vertex> m_vertex; // vertices lidos do arquivo OBJ
  std::vector<GLuint> m_index;  // indices lidos do arquivo OBJ

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void modelCreateBuffers();
};

#endif