#ifndef SKULL_HPP_
#define SKULL_HPP_

#include "abcgOpenGL.hpp"
#include "vertex.hpp"
#include "camera.hpp"

class Skull {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update(float deltaTime, Camera camera);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};


  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  void loadModelFromFileSkull(std::string_view path); //carregamento do arquivo OBJ

  void randomizeSkull();

  glm::vec3 s_position{};
  float s_rotation{};
  glm::vec3 s_size{};
  
  std::vector<Vertex> m_vertex; // vertices lidos do arquivo OBJ
  std::vector<GLuint> m_index; // indices lidos do arquivo OBJ

};

#endif