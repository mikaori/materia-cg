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

  // inicializa a floresta
  void create();

  // função de desenho das árvores
  void paint(Camera m_camera, Moon m_moon, Skull m_skull);

  // liberar os recursos do OpenGL que foram alocados em onCreate ou durante a aplicação
  void destroy();
  
  // função para randomizar a posição da árvore a cada 10 segundos
  void update(float deltaTime);

  float m_shininess = 25.0f; // constante α do termo especular da equação
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f}; // direção da luz

private:
  // variáveis alocadas para a matriz modelo e normal (que transforma um vetor normal do espaço do mundo para um vetor normal do espaço da câmera)
  GLint m_modelMatrixLocation{};
  GLint m_normalMatrixLocation{};

  // variável definida para geração aleátoria de posição e size
  std::default_random_engine m_randomEngine;

  float timeToChangeTree = 10.0; //valor default para as árvores "se moverem"

  // struct para armazenar as posições e size das árvores
  struct Tree {
    glm::vec3 m_position{};
    glm::vec3 m_size{};
  };

  // array para armazenar as árvores
  std::array<Tree, N_TREES> m_tree;

  // função que gera para posição e size aleatório para a árvore
  void randomizeTree(Tree &tree);
};

#endif