#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "ground.hpp"

#define N_TREES 500

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Window : public abcg::OpenGLWindow {
  protected:
    void onEvent(SDL_Event const &event) override;
    void onCreate() override;
    void onPaint() override;
    void onPaintUI() override;
    void onResize(glm::ivec2 const &size) override;
    void onDestroy() override;
    void onUpdate() override;

  private:
    std::default_random_engine m_randomEngine;

    float timeToChangeTree = 10.0;
    
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
    float m_dollySpeed{}; //câmera ir para a frente e para trás ao longo da direção de visão
    float m_truckSpeed{}; //câmera deslizar para os lados
    float m_panSpeed{}; //câmera girar em torno de seu eixo y

    Ground m_ground;

    struct Tree {
      glm::vec3 m_position{};
      glm::vec3 m_size{};
    };

    std::array<Tree, 500> m_tree;

    std::vector<Vertex> m_vertices; // vertices lidos do arquivo OBJ
    std::vector<GLuint> m_indices; // indices lidos do arquivo OBJ

    void loadModelFromFile(std::string_view path); //carregamento do arquivo OBJ

    void randomizeTree(Tree &tree);

};

#endif