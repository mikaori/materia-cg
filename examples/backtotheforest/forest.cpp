#include "forest.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

void Forest::create() {

  // Material properties
  Ka = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  Kd = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  Ks = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  shininess = 25.0f;

  auto const &assetsPath{abcg::Application::getAssetsPath()};

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/forest.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/forest.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
  for (auto &tree : m_tree) {
    randomizeTree(tree);
  }

  loadModelFromFileTree(assetsPath + "/obj/tree/Tree.obj", false);

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

  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(m_program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const texCoordAttribute{
      abcg::glGetAttribLocation(m_program, "inTexCoord")};
  if (texCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(texCoordAttribute);
    auto const offset{offsetof(Vertex, texCoord)};
    abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_lightDirLocation =
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  m_normalMatrixLocation =
      abcg::glGetUniformLocation(m_program, "normalMatrix");
  m_IaLocation = abcg::glGetUniformLocation(m_program, "Ia");
  m_IdLocation = abcg::glGetUniformLocation(m_program, "Id");
  m_IsLocation = abcg::glGetUniformLocation(m_program, "Is");
  m_KaLocation = abcg::glGetUniformLocation(m_program, "Ka");
  m_KdLocation = abcg::glGetUniformLocation(m_program, "Kd");
  m_KsLocation = abcg::glGetUniformLocation(m_program, "Ks");
  m_shininessLocation = abcg::glGetUniformLocation(m_program, "shininess");
  m_lightDiameterLocation =
      abcg::glGetUniformLocation(m_program, "lightDiameter");
  m_diffuseTexLocation = abcg::glGetUniformLocation(m_program, "diffuseTex");
  m_mappingModeLocation = abcg::glGetUniformLocation(m_program, "mappingMode");
}

void Forest::paint(Camera m_camera, Moon m_moon) {
  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // a variável uniforme color é definida como (0.33f, 0.21f, 0.18f, 1.0f)
  // (marrom) no vertex shader
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);
  abcg::glUniform1i(m_diffuseTexLocation, 0);
  abcg::glUniform1i(m_mappingModeLocation, getMappingMode());

  abcg::glUniform4fv(m_lightDirLocation, 1, m_moon.getLightDirection());

  abcg::glUniform4f(m_colorLocation, 0.33f, 0.21f, 0.18f, 1.0f);

  abcg::glUniform4fv(m_IaLocation, 1, m_moon.getIa());
  abcg::glUniform4fv(m_IdLocation, 1, m_moon.getId());
  abcg::glUniform4fv(m_IdLocation, 1, m_moon.getIs());
  abcg::glUniform4fv(m_KaLocation, 1, getKa());
  abcg::glUniform4fv(m_KdLocation, 1, getKd());
  abcg::glUniform4fv(m_KdLocation, 1, getKs());
  abcg::glUniform1f(m_shininessLocation, getShininess());
  abcg::glUniform1f(m_lightDiameterLocation, m_moon.getLightDiameter());

  // Configuração de renderização de cada uma das árvores
  for (auto &tree : m_tree) {
    glm::mat4 model{1.0f};
    // concatenação de transformações que forma a matriz de modelo
    model = glm::translate(model, tree.m_position);
    model = glm::scale(model, tree.m_size);

    // envia a matriz modelo para a variável m_modelMatrix no vertex shader
    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

    auto const modelViewMatrix{glm::mat3(m_camera.getViewMatrix() * model)};
    auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE,
                             &normalMatrix[0][0]);

    // comando de renderização
    abcg::glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT,
                         nullptr);
  }

  abcg::glBindVertexArray(0);
}

void Forest::update(float deltaTime) {
  // Randomize Tree position
  timeToChangeTree -= deltaTime;

  if (timeToChangeTree < 0.0f) {
    for (auto &tree : m_tree) {
      if (rand() % 5 == 0) {
        randomizeTree(tree);
      }
    }
    timeToChangeTree = 10.0f;
  }
}

// função que gera para posição e size aleatório para a árvore
void Forest::randomizeTree(Tree &tree) {

  // Random position: x and y in [-20, 20), z in [-100, 0)
  std::uniform_real_distribution<float> distPosXZ(-20.0f, 20.0f);

  // define a posição da árvore usando a posição aleatória implementada acima
  tree.m_position =
      glm::vec3(distPosXZ(m_randomEngine), 0, distPosXZ(m_randomEngine));

  // Random size: x and y in [-0.5, 2), z in [-100, 0)
  std::uniform_real_distribution<float> distSizeXZ(0.50f, 2.0f);

  // define o tamanho aleatório de acordo com a variável aleátoria implementada
  // acima
  tree.m_size = glm::vec3(distSizeXZ(m_randomEngine));
}

void Forest::loadModelFromFileTree(std::string_view path, bool standardize) {
  auto const basePath{std::filesystem::path{path}.parent_path().string() + "/"};

  tinyobj::ObjReaderConfig readerConfig;
  readerConfig.mtl_search_path = basePath; // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};
  auto const &materials{reader.GetMaterials()};

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

  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // First material
    Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    shininess = mat.shininess;

    if (!mat.diffuse_texname.empty()){
      loadDiffuseTexture(basePath + mat.diffuse_texname);
    }
      
  } else {
    // Default values
    Ka = {0.1f, 0.1f, 0.1f, 1.0f};
    Kd = {0.7f, 0.7f, 0.7f, 1.0f};
    Ks = {1.0f, 1.0f, 1.0f, 1.0f};
    shininess = 25.0f;
  }

  if (standardize) {
    Forest::standardize();
  }

  computeNormals();
}

void Forest::loadDiffuseTexture(std::string_view path) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}

/// @brief liberar os recursos do OpenGL que foram alocados em onCreate ou
/// durante a aplicação
void Forest::destroy() {
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Forest::computeNormals() {
  
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

void Forest::standardize() {
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