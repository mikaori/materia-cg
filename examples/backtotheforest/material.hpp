#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "abcgOpenGL.hpp"

class Material {
public:
    float * getKa(){
        return &Ka.x;
    }
    float * getKd(){
        return &Kd.x;
    }
    float * getKs(){
        return &Ks.x;
    }

    float getShininess(){
        return shininess;
    }

protected:
    glm::vec4 Ka{};
    glm::vec4 Kd{};
    glm::vec4 Ks{};
    float shininess = 0.0f;
};

#endif