#include "skull.hpp"

#include <unordered_map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <glm/gtx/fast_trigonometry.hpp>


void Skull::create(GLuint program) {

  auto const &assetsPath{abcg::Application::getAssetsPath()};

  loadModelFromFileSkull(assetsPath + "Skull.obj");
  
  randomizeSkull();

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

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_viewMatrixLoc = abcg::glGetUniformLocation(program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(program, "projMatrix");
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Skull::randomizeSkull() {
  // Random position: x and y in [-20, 20), z in [-100, 0)
  int selected_position = rand() % 4;
  
  if( selected_position == 0 ){
    s_position = glm::vec3{15.0f, 0.20f, 15.0f};   
  } else if( selected_position == 1 ){
    s_position = glm::vec3{15.0f, 0.20f, -15.0f};   
  } else if( selected_position == 2 ){
    s_position = glm::vec3{-15.0f, 0.20f, 15.0f};   
  } else {
    s_position = glm::vec3{-15.0f, 0.20f, -15.0f};   
  }
  
  s_rotation = 0.0f;
  s_size = glm::vec3(0.0006f);
}

void Skull::paint(Camera m_camera) {
  abcg::glBindVertexArray(m_VAO);

  // a variável uniforme color é definida como (0.33f, 0.21f, 0.18f, 1.0f) (marrom) no vertex shader
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  // Set model matrix as a translation matrix
  glm::mat4 model{1.0f};
  model = glm::translate(model, s_position);
  model = glm::rotate(model, s_rotation, glm::vec3(0, 1, 0));
  model = glm::scale(model, s_size);
  

  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

/// @brief liberar os recursos do OpenGL que foram alocados em onCreate ou durante a aplicação
void Skull::destroy() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

/// @brief realiza o update da posição e rotação da shuriken
/// @param deltaTime tempo entre os quadros de exibição
/// @param camera recebe a classe responsável pela Câmera
void Skull::update(float deltaTime, Camera camera){
   
  glm::vec3 diffVectorPosition = glm::normalize(camera.getCameraPosition() - s_position); // normaliza a diferença entre os vetores da posição da câmera e a posição da shuriken
  
  s_position += (diffVectorPosition * glm::vec3(1.0f,0.0f, 1.0f)) * deltaTime * 1.5f; // velocidade no qual a shuriren se move

  s_rotation += 20.0f * deltaTime; // velocidade de rotação

}

/// @brief verifica se a shuriken alcançou o player
/// @param position_verify posição da câmera
/// @return posição da shuriken
bool Skull::touch(glm::vec3 position_verify){
  return glm::distance(position_verify, s_position) < 0.5f;
}

/// @brief Carrega o modelo da shuriken - Retirado das notas de aula
/// @param path caminho do asset que contem o arquivo .obj da shuriken
void Skull::loadModelFromFileSkull(std::string_view path) {
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