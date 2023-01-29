#include "model.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

void Model::modelStandardize() {
  // Center to origin and normalize largest bound to [-1, 1]

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

void Model::modelComputeNormals() {

  if (!m_hasNormals) {
    return;
  }

  // Clear previous vertex normals
  for (auto &vertex : m_vertex) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Compute face normals
  for (auto const offset : iter::range(0UL, m_index.size(), 3UL)) {
    // Get face vertices
    auto &a{m_vertex.at(m_index.at(offset + 0))};
    auto &b{m_vertex.at(m_index.at(offset + 1))};
    auto &c{m_vertex.at(m_index.at(offset + 2))};

    // Compute normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Accumulate on vertices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normalize
  for (auto &vertex : m_vertex) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

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

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Vertex normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      // Texture coordinates
      glm::vec2 texCoord{};
      if (index.texcoord_index >= 0) {
        m_hasTexCoords = true;
        auto const texCoordsStartIndex{2 * index.texcoord_index};
        texCoord = {attrib.texcoords.at(texCoordsStartIndex + 0),
                    attrib.texcoords.at(texCoordsStartIndex + 1)};
      }

      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertex.size();
        // Add this vertex
        m_vertex.push_back(vertex);
      }

      m_index.push_back(hash[vertex]);
    }
  }

  if (standardize) {
    modelStandardize();
  }

  modelComputeNormals();
}

void Model::modelCreateBuffers() {
  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex.at(0)) * m_vertex.size(),
                     m_vertex.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_index.at(0)) * m_index.size(), m_index.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}