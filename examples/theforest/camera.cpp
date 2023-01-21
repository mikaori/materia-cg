#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

// calcula uma matriz de projeção perspectiva
void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 5.0f);
}


// computa as alterações realizadas em m_eye e m_at
void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}


// desloca os pontos m_eye e m_at para frente ou para trás na direção de visão
void Camera::dolly(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}


// desloca os pontos m_eye e m_at nas laterais de acordo com a direção do vetor left
void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};
  
  // Compute vector to the left
  auto const left{glm::cross(m_up, forward)};

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

// movimento de girar a câmera em torno do eixo y
void Camera::pan(float speed) {
  glm::mat4 transform{1.0f};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

// retorna a posição da câmera
glm::vec3 Camera::getCameraPosition(){
  return m_eye;
}