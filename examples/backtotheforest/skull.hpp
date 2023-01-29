#ifndef SKULL_HPP_
#define SKULL_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "moon.hpp"

class Skull: public Material {
public:
  void create();
  void paint(Camera m_camera, Moon m_moon);
  void destroy();
  void update(float deltaTime, Camera camera);
  bool touch(glm::vec3 position_verify);

private:
  GLint m_modelMatrixLocation{};
  GLint m_normalMatrixLocation{};
  
  void
  loadModelFromFileSkull(std::string_view path); // carregamento do arquivo OBJ

  void randomizeSkull();

  glm::vec3 s_position{};
  float s_rotation{};
  glm::vec3 s_size{};
};

#endif