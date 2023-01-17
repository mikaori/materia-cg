#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing};
};

#endif