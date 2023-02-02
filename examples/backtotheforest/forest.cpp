#include "forest.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

// inicializa a floresta
void Forest::create() {

  // propriedades do material
  Ka = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  Kd = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  Ks = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  shininess = 1.0f;

  // contem o caminho atual do subdiretório
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  // armazena os recursos alocados (shaders, arranjo de vértices)
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/forest.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/forest.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
  
  // gera para cada uma das árvores uma posição e um size aleatório
  for (auto &tree : m_tree) {
    randomizeTree(tree);
  }

  // carrega o arquivo OBJ da tree
  materialLoadModelFromFile(assetsPath + "/obj/tree/Tree.obj", false);

  // cria os VBO, EBO e VAO
  modelCreateBuffers();

  // obtem os atributos de cada vértice (posição, vetor normal e coordenada das texturas)
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

  // fim do bind
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // fim do bind para o atual VAO
  abcg::glBindVertexArray(0);

  // obtem a localização de variáveis uniformes para o modelMatrix e o normalMatrix
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_normalMatrixLocation =
      abcg::glGetUniformLocation(m_program, "normalMatrix");

  // carrega a localização de variáveis uniformes dos materiais
  materialLoadLocation(); 
}

// função de desenho das árvores
void Forest::paint(Camera m_camera, Moon m_moon, Skull m_skull) {
  
  // começa a usar o programa de shader
  abcg::glUseProgram(m_program);
  // começa a usar o VAO
  abcg::glBindVertexArray(m_VAO);

  // realiza o bind das texturas
  materialBindTexture();

  // carrega a localização de variáveis uniformes da câmera
  m_camera.loadLocation(m_program);
  // realiza o bind das variáveis uniformes para viewMatrix e projMatrix da camera
  m_camera.bind();

  // carrega a localização de variáveis uniformes da lua
  m_moon.loadLocation(m_program);
  // realiza o bind das variáveis uniformes da lua
  m_moon.lightBind();

  // carrega a localização de variáveis uniformes da Skuriken
  m_skull.loadLocation(m_program);
  // realiza o bind das variáveis uniformes da Shuriken
  m_skull.lightBind();

  // realiza o bind das variáveis uniformes do material
  materialBindLocation();

  // Configura a renderização de cada uma das árvores
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

// função para randomizar a posição da árvore a cada 10 segundos
void Forest::update(float deltaTime) {
  timeToChangeTree -= deltaTime;

  if (timeToChangeTree < 0.0f) {
    for (auto &tree : m_tree) {
      // a cada 5 arvores, uma troca de lugar
      if (rand() % 5 == 0) {
        randomizeTree(tree);
      }
    }
    // define o "tempo" para as árvores trocarem de lugar
    timeToChangeTree = 10.0f;
  }
}

// função que gera para posição e size aleatório para a árvore
void Forest::randomizeTree(Tree &tree) {

  // posição aleatória: x e y dentro do intervalo [-20, 20)
  std::uniform_real_distribution<float> distPosXZ(-20.0f, 20.0f);

  // define a posição da árvore usando a posição aleatória implementada acima
  tree.m_position =
      glm::vec3(distPosXZ(m_randomEngine), 0, distPosXZ(m_randomEngine));

  // posição aleatória: x e y dentro do intervalo [0.50, 2.0)
  std::uniform_real_distribution<float> distSizeXZ(0.50f, 2.0f);

  // define o tamanho aleatório de acordo com a variável aleátoria implementada acima
  tree.m_size = glm::vec3(distSizeXZ(m_randomEngine));
}

// libera os recursos do OpenGL que foram alocados para a floresta
void Forest::destroy() {
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}