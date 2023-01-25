#ifndef TREE_HPP_
#define TREE_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "model.hpp"
#include "camera.hpp"

// variável com o número de árvores no cenário
#define N_TREES 1500

class Forest {
public:
  void create(GLuint program);
  void paint(Camera m_camera);
  void destroy();
  void update(float deltaTime);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  
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

};

#endif