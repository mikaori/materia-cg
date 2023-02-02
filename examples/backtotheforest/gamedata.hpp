#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

// classe contendo os estados do player
enum class State { Playing, GameOver };

// struct do GameData definindo o status inicial 
struct GameData {
  State m_state{State::Playing};
};

#endif