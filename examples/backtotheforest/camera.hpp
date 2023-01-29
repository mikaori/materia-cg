#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "abcgOpenGL.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);

  // funções que modificam as variáveis m_eye e m_at
  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);

  // funções de acesso à matriz de visão e projeção
  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }

  // pega a posição da câmera
  glm::vec3 getCameraPosition();

  void loadLocation(GLint);
  void bind();

private:
  glm::vec3 m_eye{0.0f, 0.5f, 2.5f}; // Camera position
  glm::vec3 m_at{0.0f, 0.5f, 0.0f};  // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

  // Matrix to change from world space to camera space
  GLint m_viewMatrixLocation{};
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  GLint m_projMatrixLocation{};
  glm::mat4 m_projMatrix;
};

#endif