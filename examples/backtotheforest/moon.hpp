#ifndef MOON_HPP_
#define MOON_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "light.hpp"

// define a classe Moon, que herda propriedades de Light
class Moon : public Light {
public:
  // função que inicializa as váriaveis de iluminação da lua
  void create();
  // função para carregar a localização das variáveis uniformes da lua
  void loadLocation(GLint);

private:
};

#endif