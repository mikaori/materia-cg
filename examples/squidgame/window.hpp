#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const l_N{1}; // Board size is m_N x m_N
  static int const c_N{2};
  static int const m_N{2};
  static int const last_level{5};
  std::string concatenation;


  int m_gameLevel{0};
  int m_answers[10];

  enum class GameState { Play, Played, Failed, Win, Lost, RestartForPlayer, ResetGame };
  enum class PlayerChoice { Left, Right, NotPlayed };
  GameState m_gameState;
  GameState m_futureGameState;
  PlayerChoice m_playerChoice;

  //bool m_XsTurn{true};
  std::array<char, l_N * c_N> m_board{}; // '\0', 'X' or 'O'

  ImFont *m_font{};

  void checkEndCondition();
  void startGame();
  void generateRandomAnswer();
  bool checkIfAnswerIsCorrect();
  void resetGame();
  void restartForPlayer();
  void failed();
  void play();
  void played();
};

#endif
