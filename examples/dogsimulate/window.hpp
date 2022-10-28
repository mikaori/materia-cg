#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "balls.hpp"
#include "dog.hpp"
#include "basket.hpp"

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

  // GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  GameData m_gameData;

  Balls m_balls;
  Basket m_basket;
  Dog m_dog;

  abcg::Timer m_restartWaitTimer;

  ImFont *m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void checkCollisions();
  void checkEndCondition();
};

#endif