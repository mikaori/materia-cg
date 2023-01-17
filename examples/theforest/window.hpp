#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "ground.hpp"
#include "skull.hpp"
#include "vertex.hpp"
#include "gamedata.hpp"

#define N_TREES 500


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
    float timeToEndGame = 10.0;
    float startingPlayerMov = 10.0;
    
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

    Skull m_skull;

    GameData m_gameData;

    struct Tree {
      glm::vec3 m_position{};
      glm::vec3 m_size{};
    };

    std::array<Tree, 1500> m_tree;

    std::vector<Vertex> m_tree_vertex; // vertices lidos do arquivo OBJ
    std::vector<GLuint> m_tree_index; // indices lidos do arquivo OBJ

    void loadModelFromFileTree(std::string_view path); //carregamento do arquivo OBJ

    void randomizeTree(Tree &tree);
};

#endif