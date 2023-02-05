#include "skull.hpp"

#include <cmath>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <unordered_map>

// inicializa a skull
void Skull::create() {
  // seta as propriedades de iluminação da skull
  Ia = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  Id = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  Is = glm::vec4{0.3f, 0.05f, 0.05f, 1.0f};
  lightDiameter = 1.0f;

  shininess = 1.0f;

  // define o modo de mapping da textura
  mappingMode = 2;

  // carrega o caminho atual do subdiretório
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  // armazena os recursos alocados (shaders, arranjo de vértices)
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/skull.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/skull.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // randomiza a posição inicial da skull
  randomizeSkull();
  
  // carrega o OBJ da skull
  materialLoadModelFromFile(assetsPath + "/obj/skull/Skull.obj", true);

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

  // fim do bind do VAO atual
  abcg::glBindVertexArray(0);

  // obtem a localização de variáveis uniformes para o modelMatrix e o normalMatrix
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_normalMatrixLocation =
      abcg::glGetUniformLocation(m_program, "normalMatrix");
  
  // carrega a localização de variáveis uniformes dos materiais
  materialLoadLocation();
}

// randomiza a posição inicial da skull
void Skull::randomizeSkull() {
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

// função de desenho da shuriken
void Skull::paint(Camera m_camera, Moon m_moon) {

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

  // carrega a localização de variáveis uniformes da skull
  loadLocation(m_program);
  // realiza o bind das variáveis uniformes da skull
  lightBind();

  // realiza o bind das variáveis uniformes do material
  materialBindLocation();

  // define a matriz modelo como a matriz de translação
  glm::mat4 model{1.0f};
  model = glm::translate(model, s_position);
  model = glm::rotate(model, s_rotation, glm::vec3(0, 1, 0));
  model = glm::scale(model, s_size);

  // envia a matriz modelo para a variável m_modelMatrix no vertex shader
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

  auto const modelViewMatrix{glm::mat3(m_camera.getViewMatrix() * model)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE,
                           &normalMatrix[0][0]);

  // comando de renderização
  abcg::glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

// liberar os recursos do OpenGL que foram alocados
void Skull::destroy() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

// realiza o update da posição, rotação e iluminação da shuriken
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

// verifica se a shuriken alcançou o player
bool Skull::touch(glm::vec3 position_verify) {
  return glm::distance(position_verify, s_position) < 0.5f;
}

// obtem a localização das variáveis uniformes relacionadas a iluminação da skull
void Skull::loadLocation(GLint m_program) {
  m_IaLocation = abcg::glGetUniformLocation(m_program, "IaSkull");
  m_IdLocation = abcg::glGetUniformLocation(m_program, "IdSkull");
  m_IsLocation = abcg::glGetUniformLocation(m_program, "IsSkull");
  m_lightDiameterLocation =
      abcg::glGetUniformLocation(m_program, "lightDiameterSkull");
  m_lightDirLocation =
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpaceSkull");
}