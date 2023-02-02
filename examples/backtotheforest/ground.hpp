#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"

class Ground {
public:
  // inicializa a floresta
  void create(GLuint program);
  // função de desenho do chão
  void paint(Camera m_camera);
  // libera os recursos do OpenGL que foram alocados para o chão
  void destroy();

private:
// alocação de variáveis para o VAO e VBO
  GLuint m_VAO{};
  GLuint m_VBO{};

  // alocação de variáveis para a declaração das matrizes 
  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  float SIZE_OF_SIDE = 0.1f; // tamanho dos lados do quadrado
  int NUMBER_SIDE = 200; // tamanho do chão
};

#endif