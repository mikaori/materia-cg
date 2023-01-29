#ifndef TREE_HPP_
#define TREE_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "model.hpp"
#include "moon.hpp"
#include "skull.hpp"

// variável com o número de árvores no cenário
#define N_TREES 1500

class Forest : public Material {
public:
  void create();
  void paint(Camera m_camera, Moon m_moon, Skull m_skull);
  void destroy();
  void update(float deltaTime);
  float m_shininess = 25.0f;
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};

private:
  GLint m_modelMatrixLocation{};
  GLint m_normalMatrixLocation{};

  std::default_random_engine m_randomEngine;

  float timeToChangeTree = 10.0;

  struct Tree {
    glm::vec3 m_position{};
    glm::vec3 m_size{};
  };

  std::array<Tree, N_TREES> m_tree;

  void randomizeTree(Tree &tree);
};

#endif