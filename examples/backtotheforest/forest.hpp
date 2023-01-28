#ifndef TREE_HPP_
#define TREE_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "moon.hpp"

// variável com o número de árvores no cenário
#define N_TREES 1500

class Forest {
public:
  void create();
  void paint(Camera m_camera, Moon m_moon);
  void destroy();
  void update(float deltaTime);
  std::array<glm::vec4, 3> lightProperties();
  std::array<glm::vec4, 3> materialProperties();
  float m_shininess = 25.0f;
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_lightDirLocation{};
  GLint m_colorLocation{};
  GLint m_normalMatrixLocation{};
  
  std::vector<Vertex> m_vertex; // vertices lidos do arquivo OBJ
  std::vector<GLuint> m_index; // indices lidos do arquivo OBJ

  std::default_random_engine m_randomEngine;

  float timeToChangeTree = 10.0;

  struct Tree {
    glm::vec3 m_position{};
    glm::vec3 m_size{};
  };

  std::array<Tree, N_TREES> m_tree;

  void loadModelFromFileTree(std::string_view path); //carregamento do arquivo OBJ

  void randomizeTree(Tree &tree);

  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{1.0f};

};

#endif