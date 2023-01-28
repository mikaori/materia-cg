#include "forest.hpp"

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>

void Forest::create() {

  auto const &assetsPath{abcg::Application::getAssetsPath()};
  
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/forest.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/forest.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
  for (auto &tree : m_tree) {
    randomizeTree(tree);
  }

  loadModelFromFileTree(assetsPath + "Tree1.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertex.at(0)) * m_vertex.size(),
                     m_vertex.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_index.at(0)) * m_index.size(),
                     m_index.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

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
  
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_lightDirLocation = abcg::glGetUniformLocation(m_program, "lightDirWorldSpace");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  m_normalMatrixLocation = abcg::glGetUniformLocation(m_program, "normalMatrix");

}

void Forest::paint(Camera m_camera, Moon m_moon){
    abcg::glBindVertexArray(m_VAO);

 
    // a variável uniforme color é definida como (0.33f, 0.21f, 0.18f, 1.0f) (marrom) no vertex shader
    abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
    abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);
    abcg::glUniform4fv(m_lightDirLocation, 1, &m_moon.m_lightDir.x);

    abcg::glUniform4f(m_colorLocation, 0.33f, 0.21f, 0.18f, 1.0f);

    // Configuração de renderização de cada uma das árvores       
    for (auto &tree : m_tree)
    {
        glm::mat4 model{1.0f};
        // concatenação de transformações que forma a matriz de modelo 
        model = glm::translate(model, tree.m_position);
        model = glm::scale(model, tree.m_size);

        // envia a matriz modelo para a variável m_modelMatrix no vertex shader
        abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
        
        // comando de renderização
        abcg::glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT,
                     nullptr);
    }

    abcg::glBindVertexArray(0);
}

void Forest::update(float deltaTime){
    // Randomize Tree position 
    timeToChangeTree-= deltaTime;

    if(timeToChangeTree<0.0f){
        for (auto &tree : m_tree) {
            if(rand() % 5 == 0){
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
      glm::vec3(distPosXZ(m_randomEngine), 0,
                distPosXZ(m_randomEngine));

  // Random size: x and y in [-0.5, 2), z in [-100, 0)
  std::uniform_real_distribution<float> distSizeXZ(0.50f, 2.0f);
  
  // define o tamanho aleatório de acordo com a variável aleátoria implementada acima
  tree.m_size = glm::vec3(distSizeXZ(m_randomEngine));
}

void Forest::loadModelFromFileTree(std::string_view path) {
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

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertex.clear();
  m_index.clear();

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
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

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
}

/// @brief liberar os recursos do OpenGL que foram alocados em onCreate ou durante a aplicação
void Forest::destroy() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

std::array<glm::vec4, 3> Forest::lightProperties(){
  return {m_Ia, m_Id, m_Is};
}

std::array<glm::vec4, 3> Forest::materialProperties(){
  return {m_Ka, m_Kd, m_Ks};
}

