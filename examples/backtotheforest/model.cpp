#include "model.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

// centraliza o modelo na origem e normaliza o tamanho
void Model::modelStandardize() {

  // Get bounds
  glm::vec3 max(std::numeric_limits<float>::lowest());
  glm::vec3 min(std::numeric_limits<float>::max());
  for (auto const &vertex : m_vertex) {
    max = glm::max(max, vertex.position);
    min = glm::min(min, vertex.position);
  }

  // Center and scale
  auto const center{(min + max) / 2.0f};
  auto const scaling{2.0f / glm::length(max - min)};
  for (auto &vertex : m_vertex) {
    vertex.position = (vertex.position - center) * scaling;
  }
}

// calcula os vetores normais do modelo
void Model::modelComputeNormals() {

  // verifica se o modelo vem com os vetores de normal calculado
  if (!m_hasNormals) {
    return;
  }

  // Limpa vetores normais anteriores
  for (auto &vertex : m_vertex) {
    vertex.normal = glm::vec3(0.0f); // define vetores como (0.0,0.0,0.0)
  }

  // Calcula o normal da face
  for (auto const offset : iter::range(0UL, m_index.size(), 3UL)) {
    // obtem os vértices da face
    auto &a{m_vertex.at(m_index.at(offset + 0))};
    auto &b{m_vertex.at(m_index.at(offset + 1))};
    auto &c{m_vertex.at(m_index.at(offset + 2))};

    // Calcula a normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Acumula nos vértices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // normaliza o atributo normal 
  for (auto &vertex : m_vertex) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

// calcula o modelo por meio do arquivo .obj
void Model::modelLoadModelFromFile(tinyobj::ObjReader reader,
                                   bool standardize) {

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertex.clear();
  m_index.clear();

  m_hasNormals = false;
  m_hasTexCoords = false;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // percorre as formas
  for (auto const &shape : shapes) {
    // percorre os indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // acessa os vértices
      auto const index{shape.mesh.indices.at(offset)};

      // posição dos vértices
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // vértice normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      // coordenada da textura
      glm::vec2 texCoord{};
      if (index.texcoord_index >= 0) {
        m_hasTexCoords = true;
        auto const texCoordsStartIndex{2 * index.texcoord_index};
        texCoord = {attrib.texcoords.at(texCoordsStartIndex + 0),
                    attrib.texcoords.at(texCoordsStartIndex + 1)};
      }

      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};

      // se o map não possui esse vértice
      if (!hash.contains(vertex)) {
        // adiciona esse index (tamanho do m_vertices)
        hash[vertex] = m_vertex.size();
        // adiciona esse vértice
        m_vertex.push_back(vertex);
      }

      m_index.push_back(hash[vertex]);
    }
  }

  // verifica se é necessário centralizar na origem e normalizar o tamanho do modelo
  if (standardize) {
    modelStandardize();
  }

  // calcula os vetores normais do modelo
  modelComputeNormals();
}

// realiza a criação e configuração do VBO, EBO e VAO
void Model::modelCreateBuffers() {

  // gera um novo VBO
  abcg::glGenBuffers(1, &m_VBO);
  // realiza o bind para usar ele
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  // faz o upload dos dados para o VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex.at(0)) * m_vertex.size(),
                     m_vertex.data(), GL_STATIC_DRAW);
  // permite a desvinculação do VBO
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // gera o EBO (buffer de índices do VBO)
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_index.at(0)) * m_index.size(), m_index.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // libera o VAO anterior
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // gera VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // vincula atributos de vértice ao VAO atual
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}