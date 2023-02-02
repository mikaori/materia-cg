#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "abcgOpenGL.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:

  // função que computa a matriz de visão
  void computeViewMatrix();

  // função que computa uma matriz de projeção perspectiva 
  void computeProjectionMatrix(glm::vec2 const &size);

  // funções que modificam as variáveis m_eye e m_at
  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);

  // funções de acesso à matriz de visão e projeção
  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }

  // função que retorna a posição da câmera
  glm::vec3 getCameraPosition();

  // função que carrega as matrizes de localização de variáveis uniformes para viewMatriz e projMatrix
  void loadLocation(GLint);

  // função que define variáveis uniformes para viewMatriz e projMatrix
  void bind();

private:
  glm::vec3 m_eye{0.0f, 0.5f, 2.5f}; // Camera position
  glm::vec3 m_at{0.0f, 0.5f, 0.0f};  // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

  // matriz para mudar o espaço do mundo para o espaço da câmera
  GLint m_viewMatrixLocation{};
  glm::mat4 m_viewMatrix;

  // matriz para mudar o espaço da câmera para o espaço do clip
  GLint m_projMatrixLocation{};
  glm::mat4 m_projMatrix;
};

#endif