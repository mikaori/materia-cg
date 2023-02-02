#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

// calcula uma matriz de projeção perspectiva
void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 8.0f);
}

// computa a matriz de visão a partir das alterações realizadas em m_eye e m_at
void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

// desloca os pontos m_eye e m_at para frente ou para trás na direção de visão
void Camera::dolly(float speed) {
  
  // computa vetor de avanço (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};

  // move o eye e o centro pra frente (speed>0) ou pra trás (speed<0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

// desloca os pontos m_eye e m_at nas laterais de acordo com a direção do vetor
void Camera::truck(float speed) {

  // computa o vetor de avanço (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};

  // computa vetor para a esquerda
  auto const left{glm::cross(m_up, forward)};

  // move o eye e o centro para a esquerda (speed<0) ou para a direita (speed>0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

// realiza o movimento de girar a câmera em torno do eixo y
void Camera::pan(float speed) {
  glm::mat4 transform{1.0f};

  // faz o rotate da camera em torno do seu eixo y local
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

// retorna a posição da câmera
glm::vec3 Camera::getCameraPosition() { return m_eye; }

// carrega as matrizes de localização de variáveis uniformes para viewMatrix e projMatrix
void Camera::loadLocation(GLint m_program){
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
}

// define variáveis uniformes para viewMatrix e projMatrix
void Camera::bind(){
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &getProjMatrix()[0][0]);
}