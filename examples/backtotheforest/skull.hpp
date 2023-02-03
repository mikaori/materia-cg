#ifndef SKULL_HPP_
#define SKULL_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "moon.hpp"
#include "light.hpp"

// a classe Skull herda da classe Light e Material
class Skull: public Light,Material  {
public:

  // função que inicializa a Skull
  void create();

  // função de desenho da shuriken
  void paint(Camera m_camera, Moon m_moon);

  // função que libera os recursos do OpenGL que foram alocados
  void destroy();

  // função que realiza o update da posição, rotação e iluminação da shuriken
  void update(float deltaTime, Camera camera);

  // função que verifica se a skull "encostou" na posição passada
  bool touch(glm::vec3 position_verify);

  // função que obtem a localização das variáveis uniformes relacionadas a iluminação da skull
  void loadLocation(GLint);

private:
  // variáveis alocadas para a matriz modelo e normal (que transforma um vetor normal do espaço do mundo para um vetor normal do espaço da câmera)
  GLint m_modelMatrixLocation{};
  GLint m_normalMatrixLocation{};
  
  // carregamento do arquivo OBJ da Skull
  void
  loadModelFromFileSkull(std::string_view path); 

  // randomiza a posição inicial da skull
  void randomizeSkull();

  // variáveis que definem as propriedades da skull
  glm::vec3 s_position{};
  float s_rotation{};
  glm::vec3 s_size{};
};

#endif