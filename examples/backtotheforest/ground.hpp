#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"

class Ground {
public:
  void create(GLuint program);
  void paint(Camera m_camera);
  void destroy();

private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  float SIZE_OF_SIDE = 0.1f;
  int NUMBER_SIDE = 200;
};

#endif