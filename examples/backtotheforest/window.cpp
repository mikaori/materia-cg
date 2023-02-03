#include "window.hpp"

#include <unordered_map>

// comandos de inicialização do estado da janela e do OpenGL
void Window::onCreate() {

  // definindo o estado do game data para Playing
  m_gameData.m_state = State::Playing;

  // contem o caminho atual do subdiretório
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  // define a cor preta no fundo
  abcg::glClearColor(0, 0, 0, 1);

  // habilita depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // armazena os recursos alocados (shaders, arranjo de vértices)
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/default.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/default.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // inicializa o chão
  m_ground.create(m_program);

  // inicializa a shuriken
  m_skull.create();

  // inicializa a floresta
  m_forest.create();

  // inicializa a lua
  m_moon.create();

  // obtem a localização das variáveis uniformes do viewMatrix, projMatrix e do color
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
}

// função de desenhos
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
  abcg::glUseProgram(m_program);
  m_ground.paint(m_camera);

  // Desenha a shuriken
  abcg::glUseProgram(m_program);
  m_skull.paint(m_camera, m_moon);

  // Desenha as árvores
  m_forest.paint(m_camera, m_moon, m_skull);

  // definindo a orientação da superfície para indicar que o lado da frente tem vértices no sentido horário
  abcg::glFrontFace(GL_CCW);

  // desabilita o programa de shader
  abcg::glUseProgram(0);
}

// funções de desenho de widgets da ImGui
void Window::onPaintUI() {
  // pega a largura atual da janela da aplicação através da função
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};

  {
    // janela terá o tamanho 100
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, 100));

    // janela deverá estar na coordenada (0,0) da janela da aplicação
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    // implementação da janela de mensagem
    ImGui::Begin("MESSAGE", nullptr);

    // altera a mensagem de acordo com o state do jogo
    if (m_gameData.m_state == State::Playing) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "JUST RUN!!!");
    } else {
      ImGui::TextColored(
          ImVec4(1, 1, 0, 1),
          "OH NO, YOU ARE CAPTURE!!! he reached you!! CLOSING IN 10s...");
    }

    ImGui::EndChild();
    ImGui::End();
  }
}

// faz o resize da janela sempre que o tamanho da janela é alterado
void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

// libera os recursos do OpenGL que foram alocados 
void Window::onDestroy() {
  m_ground.destroy();
  m_skull.destroy();
  m_forest.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

// faz o update dos objetos
void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // faz o update da camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);

  // faz o update da posição e rotação da shuriken
  m_skull.update(deltaTime, m_camera);

  // faz o update da posição da floresta
  m_forest.update(deltaTime);

  // verifica se a shuriken "tocou" na camera
  if (m_skull.touch(m_camera.getCameraPosition())) {
    m_gameData.m_state = State::GameOver;
  }

  // finaliza a execução se o status é GameOver
  if (m_gameData.m_state == State::GameOver) {
    timeToEndGame -= deltaTime;
    if (timeToEndGame < 0.0f) {
      exit(0);
    }
  }
}

// captura os eventos de input do player
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