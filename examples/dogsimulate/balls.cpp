#include "balls.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Balls::create(GLuint program, int quantity) {
  destroy();

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");

  // Create balls
  m_balls.clear();
  m_balls.resize(quantity);

  for (auto &ball : m_balls) {
    ball = makeBall();

    // Make sure the ball won't collide with the dog
    do {
      ball.m_translation = {m_randomDist(m_randomEngine),
                            m_randomDist(m_randomEngine)};
    } while (glm::length(ball.m_translation) < 0.01f);
  }
}

void Balls::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &ball : m_balls) {
    if (!ball.m_catch) {
      abcg::glBindVertexArray(ball.m_VAO);
      abcg::glUniform1f(m_scaleLoc, ball.m_scale);
      abcg::glUniform4fv(m_colorLoc, 1, &ball.m_color.r);

      for (auto i : {-2, 0, 2}) {
        for (auto j : {-2, 0, 2}) {
          abcg::glUniform2f(m_translationLoc, ball.m_translation.x + j,
                            ball.m_translation.y + i);

          abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, ball.m_polygonSides + 2);
        }
      }

      abcg::glBindVertexArray(0);
    }
  }

  abcg::glUseProgram(0);
}

void Balls::destroy() {
  for (auto &ball : m_balls) {
    abcg::glDeleteBuffers(1, &ball.m_VBO);
    abcg::glDeleteVertexArrays(1, &ball.m_VAO);
  }
}

Balls::Ball Balls::makeBall(glm::vec2 translation) {
  Ball ball;

  auto &re{m_randomEngine}; // Shortcut

  // Get a random color (actually, a grayscale)
  std::uniform_real_distribution randomIntensity(0.0f, 1.0f);
  
  // Select the color of ball
  auto const colorRandom{randomIntensity(re)};
  if( colorRandom < 0.5f ){
    ball.m_color = glm::vec4(1,0,0, 1);
  }else{
    ball.m_color = glm::vec4(0,0,1, 1);
  }
  
  ball.m_color.a = 1.0f;
  ball.m_scale = 0.05f;
  ball.m_translation = translation;

  // Create geometry data
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / ball.m_polygonSides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &ball.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &ball.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(ball.m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return ball;
}

bool Balls::isAllBallsCatch() {
  for (auto const &ball : m_balls) {
    if (!ball.m_catch) {
      return false;
    }
  }
  return true;
}