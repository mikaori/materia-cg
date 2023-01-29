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

  materialLoadModelFromFile(assetsPath + "/obj/tree/Tree.obj", false);

  modelCreateBuffers();

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


  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_normalMatrixLocation =
      abcg::glGetUniformLocation(m_program, "normalMatrix");

  materialLoadLocation(); 
}

void Forest::paint(Camera m_camera, Moon m_moon) {
  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  materialBindTexture();

  m_camera.loadLocation(m_program);
  m_camera.bind();

  m_moon.loadLocation(m_program);
  m_moon.lightBind();

  materialBindLocation();

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

/// @brief liberar os recursos do OpenGL que foram alocados em onCreate ou
/// durante a aplicação
void Forest::destroy() {
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}