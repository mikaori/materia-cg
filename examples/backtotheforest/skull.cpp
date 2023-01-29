#include "skull.hpp"

#include <cmath>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

void Skull::create() {
  // Light properties
  Ia = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  Id = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  Is = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  lightDiameter = 1.0f;

  shininess = 1.0f;

  mappingMode = 2;

  auto const &assetsPath{abcg::Application::getAssetsPath()};

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/skull.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/skull.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  randomizeSkull();
  
  materialLoadModelFromFile(assetsPath + "/obj/skull/Skull.obj", true);

  modelCreateBuffers();

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

  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_normalMatrixLocation =
      abcg::glGetUniformLocation(m_program, "normalMatrix");
  materialLoadLocation();
}

void Skull::randomizeSkull() {
  // Random position: x and y in [-20, 20), z in [-100, 0)
  int selected_position = rand() % 4;

  if (selected_position == 0) {
    s_position = glm::vec3{15.0f, 0.20f, 15.0f};
  } else if (selected_position == 1) {
    s_position = glm::vec3{15.0f, 0.20f, -15.0f};
  } else if (selected_position == 2) {
    s_position = glm::vec3{-15.0f, 0.20f, 15.0f};
  } else {
    s_position = glm::vec3{-15.0f, 0.20f, -15.0f};
  }

  s_rotation = 0.0f;
  s_size = glm::vec3(0.5f);
}

void Skull::paint(Camera m_camera, Moon m_moon) {
  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  materialBindTexture();

  m_camera.loadLocation(m_program);
  m_camera.bind();

  m_moon.loadLocation(m_program);
  m_moon.lightBind();

  loadLocation(m_program);
  lightBind();

  materialBindLocation();

  // Set model matrix as a translation matrix
  glm::mat4 model{1.0f};
  model = glm::translate(model, s_position);
  model = glm::rotate(model, s_rotation, glm::vec3(0, 1, 0));
  model = glm::scale(model, s_size);

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

  auto const modelViewMatrix{glm::mat3(m_camera.getViewMatrix() * model)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE,
                           &normalMatrix[0][0]);

  abcg::glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

/// @brief liberar os recursos do OpenGL que foram alocados em onCreate ou
/// durante a aplicação
void Skull::destroy() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

/// @brief realiza o update da posição e rotação da shuriken
/// @param deltaTime tempo entre os quadros de exibição
/// @param camera recebe a classe responsável pela Câmera
void Skull::update(float deltaTime, Camera camera) {

  glm::vec3 diffVectorPosition =
      glm::normalize(camera.getCameraPosition() -
                     s_position); // normaliza a diferença entre os vetores da
                                  // posição da câmera e a posição da shuriken

  s_position += (diffVectorPosition * glm::vec3(1.0f, 0.0f, 1.0f)) * deltaTime *
                1.3f; // velocidade no qual a shuriren se move

  lightDirection = glm::vec4{s_position, 1.0f};

  s_rotation += 20.0f * deltaTime; // velocidade de rotação
}

/// @brief verifica se a shuriken alcançou o player
/// @param position_verify posição da câmera
/// @return posição da shuriken
bool Skull::touch(glm::vec3 position_verify) {
  return glm::distance(position_verify, s_position) < 0.5f;
}

void Skull::loadLocation(GLint m_program) {
  m_IaLocation = abcg::glGetUniformLocation(m_program, "IaSkull");
  m_IdLocation = abcg::glGetUniformLocation(m_program, "IdSkull");
  m_IsLocation = abcg::glGetUniformLocation(m_program, "IsSkull");
  m_lightDiameterLocation =
      abcg::glGetUniformLocation(m_program, "lightDiameterSkull");
  m_lightDirLocation =
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpaceSkull");
}