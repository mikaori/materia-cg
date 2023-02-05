#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "forest.hpp"
#include "gamedata.hpp"
#include "ground.hpp"
#include "model.hpp"
#include "moon.hpp"
#include "skull.hpp"

class Window : public abcg::OpenGLWindow {
protected:

  // captura os eventos de input do player
  void onEvent(SDL_Event const &event) override;
  
  // comandos de inicialização do estado da janela e do OpenGL
  void onCreate() override;
  
  // função de desenhos
  void onPaint() override;

  // funções de desenho de widgets da ImGui
  void onPaintUI() override;

  // faz o resize dos elementos e objetos sempre que o tamanho da janela é alterado
  void onResize(glm::ivec2 const &size) override;

  // libera os recursos que foram alocados 
  void onDestroy() override;

  // faz o update dos objetos
  void onUpdate() override;

private:
  // variavel utiliaza para definir o tempo de exit do gane
  float timeToEndGame = 10.0;

  // armazena o tamanho da janela da aplicação 
  glm::ivec2 m_viewportSize{};

  // definição das variaveis utilizadas para identificar os recursos alocados 
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  // identificadores das variáveis uniform do vertex shader
  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  // definição de um objeto da classe Camera para controlar a câmera
  Camera m_camera;
  float m_dollySpeed{}; // câmera ir para a frente e para trás ao longo da
                        // direção de visão
  float m_truckSpeed{}; // câmera deslizar para os lados
  float m_panSpeed{};   // câmera girar em torno de seu eixo y

  // definição de um objeto da classe Ground para desenhar o chão
  Ground m_ground;

  // definição de um objeto da classe Skull para controlar a shuriken
  Skull m_skull;

  // definição de um objeto da classe Tree para desenhar e controlar a posição das árvores
  Forest m_forest;

  // definição de um objeto da classe GameData para controlar os estados do player
  GameData m_gameData;

  // definição da iluminação global no cenário, como se fosse a iluminação de uma lua
  Moon m_moon;
};

#endif