#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "abcgOpenGL.hpp"

class Light {
public:
    float * getIa(){
        return &Ia.x;
    }
    float * getId(){
        return &Id.x;
    }
    float * getIs(){
        return &Is.x;
    }
    float * getLightDirection(){
        return &lightDirection.x;
    }
    
    float getLightDiameter(){
        return lightDiameter;
    }

protected:
    glm::vec4 Ia{};
    glm::vec4 Id{};
    glm::vec4 Is{};
    glm::vec4 lightDirection{};
    float lightDiameter{1.0f};
};
#endif