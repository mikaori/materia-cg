#include "window.hpp"

#include <unordered_map>

void Window::onCreate() {

  // definindo o estado do game data para Playing 
  m_gameData.m_state = State::Playing;
  
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Setup tree
  for (auto &tree : m_tree) {
    randomizeTree(tree);
  }

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "theforest.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "theforest.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // inicializa o VAO/VBO do chão
  m_ground.create(m_program); 

  // inicializa o VAO/VBO da shuriken
  m_skull.create(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

  // Load model
  loadModelFromFileTree(assetsPath + "Tree1.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_tree_vertex.at(0)) * m_tree_vertex.size(),
                     m_tree_vertex.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_tree_index.at(0)) * m_tree_index.size(),
                     m_tree_index.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}


void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  // Configuração de renderização de cada uma das árvores
  for (auto &tree : m_tree)
  {
        glm::mat4 model{1.0f};
        // concatenação de transformações que forma a matriz de modelo 
        model = glm::translate(model, tree.m_position);
        model = glm::scale(model, tree.m_size);

        // envia a matriz modelo para a variável m_modelMatrix no vertex shader
        abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
        
        // a variável uniforme color é definida como (0.33f, 0.21f, 0.18f, 1.0f) (marrom) no vertex shader
        abcg::glUniform4f(m_colorLocation, 0.33f, 0.21f, 0.18f, 1.0f);
        
        // comando de renderização
        abcg::glDrawElements(GL_TRIANGLES, m_tree_index.size(), GL_UNSIGNED_INT,
                     nullptr);
  }
    
  abcg::glBindVertexArray(0); 

  // Desenha o chão
  m_ground.paint();

  // Desenha a shuriken
  m_skull.paint();

  // definindo a orientação da superfície para indicar que o lado da frente tem vértices no sentido horário
  abcg::glFrontFace(GL_CW);

  // desabilita o programa de shader
  abcg::glUseProgram(0);
}

// função que gera para posição e size aleatório para a árvore 
void Window::randomizeTree(Tree &tree) {
  
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

void Window::onPaintUI() { 
  // pega a largura atual da janela da aplicação através da função
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};

  { 
    // janela terá o tamanho 60
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, 60));

    // janela deverá estar na coordenada (0,0) da janela da aplicação 
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    // implementação da janela de mensagem
    ImGui::Begin("MESSAGE", nullptr);

    if(m_gameData.m_state == State::Playing){
      ImGui::TextColored(ImVec4(1,1,0,1), "JUST RUN!!!");
    } else {
      ImGui::TextColored(ImVec4(1,1,0,1), "OH NO, YOU ARE CAPTURE!!! he reached you!! CLOSING IN 10s...");
    }
    
    ImGui::EndChild();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();
  m_skull.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  
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

  // Update shuriken position and rotation
  m_skull.update(deltaTime, m_camera);

  // Verify if the shuriken have "touch" the camera
  if(m_skull.touch(m_camera.getCameraPosition())){
    m_gameData.m_state = State::GameOver;
  }

  // Exit the execution if the state is GameOver
  if(m_gameData.m_state == State::GameOver){
    timeToEndGame -= deltaTime;
    if( timeToEndGame < 0.0f){
      exit(0);
    }
  }

}

void Window::loadModelFromFileTree(std::string_view path) {
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

  m_tree_vertex.clear();
  m_tree_index.clear();

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
        hash[vertex] = m_tree_vertex.size();
        // Add this vertex
        m_tree_vertex.push_back(vertex);
      }

      m_tree_index.push_back(hash[vertex]);
    }
  }
}

void Window::onEvent(SDL_Event const &event) {

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }

  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }

}