#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"

//dedine os atributos que compõe um vértice
struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  float m_angle{}; // ângulo de rotação que será enviado à variável uniforme do vertex shader
  int m_verticesToDraw{}; // qtd de vértices do VBO que será processada pela função de rederização

  std::vector<Vertex> m_vertices; // arranjo de vértices lidos do arquivo OBJ
  std::vector<GLuint> m_indices; // arranjo de índices lidos do arquivo OBJ

  void loadModelFromFile(std::string_view path); // carrega o arquivo OBJ
  void standardize(); // centraliza o modelo na origem e normalia as coordenadas de todos os vértices 
};

#endif