#ifndef MOON_HPP_
#define MOON_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "light.hpp"

class Moon : public Light {
public:
  void create();
  void loadLocation(GLint);

private:
};

#endif