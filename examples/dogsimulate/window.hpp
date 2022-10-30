#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "balls.hpp"
#include "dog.hpp"
#include "flowers.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  GameData m_gameData;

  Dog m_dog;
  Flowers m_flowers;
  Balls m_balls;
  int m_ballsQuantity{20};

  abcg::Timer m_restartWaitTimer;

  ImFont *m_font{};

  std::default_random_engine m_randomEngine;

  std::array<float, 4> m_clearColor{0.0f, 0.7f, 0.0f, 1.0f};

  void restart();
  void checkCollisions();
  void checkEndCondition();
};

#endif