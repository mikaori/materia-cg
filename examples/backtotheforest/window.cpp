#include "window.hpp"

#include <unordered_map>

void Window::onCreate() {

  // definindo o estado do game data para Playing 
  m_gameData.m_state = State::Playing;
  
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // habilita depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // criação do program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/theforest.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/theforest.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // inicializa o VAO/VBO do chão
  m_ground.create(m_program); 

  // inicializa o VAO/VBO da shuriken
  m_skull.create(m_program);

  // inicializa o VAO/VBO da floresta
  m_forest.create(m_program);

  // pega a localização das variáveis uniformes
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  
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

  // Desenha o chão
  m_ground.paint(m_camera);

  // Desenha a shuriken
  m_skull.paint(m_camera);

  // Desenha as árvores
  m_forest.paint(m_camera);

  // definindo a orientação da superfície para indicar que o lado da frente tem vértices no sentido horário
  abcg::glFrontFace(GL_CW);

  // desabilita o programa de shader
  abcg::glUseProgram(0);
}

void Window::onPaintUI() { 
  // pega a largura atual da janela da aplicação através da função
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};

  { 
    // janela terá o tamanho 60
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, 100));

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

  // Update shuriken position and rotation
  m_skull.update(deltaTime, m_camera);

  // Update tree position and rotation
  m_forest.update(deltaTime);

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