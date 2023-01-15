#include "skull.hpp"

#include <unordered_map>
#include <cmath>

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
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Skull::randomizeSkull() {
  // Random position: x and y in [-20, 20), z in [-100, 0)
  int selected_position = rand() % 4;
  
  if( selected_position == 0 ){
    s_position = glm::vec3{1.0f, 0.5f, 3.5f};   
  } else if( selected_position == 1 ){
    s_position = glm::vec3{1.0f, 0.5f, 3.5f};   
  } else if( selected_position == 2 ){
    s_position = glm::vec3{1.0f, 0.5f, 3.5f};   
  } else {
    s_position = glm::vec3{1.0f, 0.5f, 3.5f};   
  }
  
  s_rotation = 0.0f;
  s_size = glm::vec3(0.02f);
}

void Skull::paint() {
  abcg::glBindVertexArray(m_VAO);

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

void Skull::destroy() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Skull::update(float deltaTime, Camera camera){
   
  glm::vec3 diffVectorPosition = glm::normalize(camera.getCameraPosition() - s_position);
  
  //s_position += diffVectorPosition * deltaTime * 0.2f;

  s_rotation = glm::acos(glm::dot(diffVectorPosition, glm::vec3(1.0f,0.0f,0.0f)));

  printf("teste\n");
}

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