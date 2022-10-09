#include "window.hpp"

void Window::onCreate() {
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }
  startGame();
}

void Window::onPaintUI() {

  // Processamento dos estados
  switch (m_gameState) {
      case GameState::Play:
        play();
        break;
      case GameState::Played:
        played();
        break;
      case GameState::Failed:
        failed();
        break;
      case GameState::Win:
       
        break;
      case GameState::Lost:
       
        break;
      case GameState::RestartForPlayer:
        restartForPlayer();
        break;
      case GameState::ResetGame:
        resetGame();
        break;
      }

  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "SquidGame" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("SquidGame", nullptr, flags);

    // Menu
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
          ImGui::MenuItem("Restart", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        startGame();
      }
    }

    ImGui::Spacing();

    //static text showing the answer game
    { 
      std::string aaaaa = concatenation;
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(aaaaa.c_str()).x) / 2);
      ImGui::Text("%s", aaaaa.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();

    //static text showing the history game
    {
      std::string history1 = "Cada nível é um passo que você escolhe andar na ponte.";
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(history1.c_str()).x) / 2);
      ImGui::Text("%s", history1.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();

    //static text showing the continue of the history game
    {
      std::string history2 ="Cuidado para não escolher o lado errado e acabar caindo";
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(history2.c_str()).x) / 2);
      ImGui::Text("%s", history2.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    //static text showing the level game
    {
      auto level = fmt::format("Level {}", std::to_string(m_gameLevel));
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(level.c_str()).x) / 2);
      ImGui::Text("%s", level.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = "Choose your left ou right to live or not...";
        break;
      case GameState::Played:
        text = "You hit! Try to pass the next step!";
        break;
      case GameState::Failed:
        text = "Oh no, you failed! We have to start over!";
        break;
      case GameState::Win:
        text = "Congratulations! You Win!";
        break;
      case GameState::Lost:
        text = "You looooooost :(";
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();

    // Game board
    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (c_N * 10) - 60};
      auto const buttonHeight{gridHeight / c_N};

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Game board", c_N)) {
        for (auto i : iter::range(l_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(c_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * c_N + j};

            // Get current character
            auto ch{m_board.at(offset)};

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (ch == 0) {
              ch = ' ';
            }

            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Play && ch == ' ') {
                m_board.at(offset) = 'X';
                
                m_playerChoice = j == 0 ? PlayerChoice::Left : PlayerChoice::Right;                
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();

    // "Restart game" button
    {
      if (ImGui::Button("New Start game", ImVec2(-1, 50))) {
        startGame();
      }
    }

    ImGui::End();
  }
  m_gameState =  m_futureGameState; 
}

// Generate an random array with 0 or 1 
void Window::generateRandomAnswer(){  
  unsigned seed = time(0);
  srand(seed);

  for (int i=0; i<last_level; i++){
    int x = rand()%2;
    m_answers[i]=x;
    concatenation += std::to_string(x);
  }   
}


bool Window::checkIfAnswerIsCorrect() {
  if (m_playerChoice == PlayerChoice::Left && m_answers[m_gameLevel] == 0){
    return true;
  } else if (m_playerChoice == PlayerChoice::Left && m_answers[m_gameLevel] == 1) {
    return false;
  } else if (m_playerChoice == PlayerChoice::Right && m_answers[m_gameLevel] == 0) {
    return false;
  } else if (m_playerChoice == PlayerChoice::Right && m_answers[m_gameLevel] == 1) {
    return true;
  }
}

void Window::resetGame() {
  m_board.fill('\0');
  m_gameLevel = 0;
  m_playerChoice = PlayerChoice::NotPlayed;
  m_futureGameState = GameState::Play;
}

void Window::restartForPlayer() {
  if (m_gameLevel<last_level-1){
    m_board.fill('\0');
    m_gameLevel++;
    m_futureGameState = GameState::Play;
  } else{
    m_futureGameState = GameState::Win;
  }
}

void Window::startGame() {
  m_gameState = GameState::StartGame;
  m_playerChoice = PlayerChoice::NotPlayed;
  m_board.fill('\0');
  m_gameLevel = 0;
  generateRandomAnswer();

  m_futureGameState = GameState::Play;
}

void Window::play(){
  if(m_playerChoice != PlayerChoice::NotPlayed){
    m_futureGameState = GameState::Played;
  }
}

void Window:: played(){
  if (checkIfAnswerIsCorrect()){
    m_futureGameState = GameState::RestartForPlayer;
  } else {
    m_futureGameState = GameState::Failed;
  }

  m_playerChoice = PlayerChoice::NotPlayed;
}

void Window::failed() {
  /* if(vida == 0 ){
        m_futureGameState = GameState::Lost;
      }else{
        m_futureGameState = GameState::ResetGame;
      }*/
}