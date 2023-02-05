#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"

// define a struct para o vértice
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  // verifica se dois vértices são iguais
  friend bool operator==(Vertex const &, Vertex const &) = default;
};

// explicita a especialização de std::hash para Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

// classe responsável pelo carregamento do arquivo OBJ e gerenciamento dos vértices, VBO, EBO e VAO
class Model {
public:
  // função que calcula os vetores normais do modelo
  void modelComputeNormals();

  // função que centraliza o modelo na origem e normaliza o tamanho
  void modelStandardize();

  // função que carrega o objeto através do arquivo OBJ
  void modelLoadModelFromFile(
      tinyobj::ObjReader reader,
      bool standardize = true);

protected:
  // definição das variaveis utilizadas para identificar os recursos alocados 
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};
  GLuint m_diffuseTexture{};

  std::vector<Vertex> m_vertex; // vertices lidos do arquivo OBJ
  std::vector<GLuint> m_index;  // indices lidos do arquivo OBJ

  // variaveis para identificar se foi feito o calculo dos vetores normais e se possui coordenadas de textura
  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  // realiza a criação e configuração do VBO, EBO e VAO
  void modelCreateBuffers();
};

#endif