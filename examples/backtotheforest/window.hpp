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
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  float timeToEndGame = 10.0;

  glm::ivec2 m_viewportSize{};

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

  // definição de um objeto da classe Tree para desenhar e controlar a posição
  // das árvores
  Forest m_forest;

  // definição de um objeto da classe GameData para controlar os estados do
  // player
  GameData m_gameData;

  // definição da iluminação global no cenário, como se fosse a iluminação de
  // uma lua
  Moon m_moon;
};

#endif