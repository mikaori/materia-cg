#ifndef BALLS_HPP_
#define BALLS_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "dog.hpp"
#include "gamedata.hpp"

class Balls {
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();

  struct Ball {
    GLuint m_VAO{};
    GLuint m_VBO{};

    glm::vec4 m_color{1};
    int m_polygonSides{20};
    float m_scale{};
    glm::vec2 m_translation{};
    bool m_catch{};
  };

  std::list<Ball> m_balls;

  Ball makeBall(glm::vec2 translation = {});
  bool isAllBallsCatch();

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-0.9f, 0.9f};
};

#endif