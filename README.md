### Michelle Kaori Hamada RA 21039716

* * *

# Atividades 

## Atividade 01 - SquidGame

[LINK PARA A APLICAÇÃO](https://mikaori.github.io/cg_ufabc/squidgame/) 
 
**Como é a aplicação?**

A aplicação é uma representação da Ponte de Crital, este jogo é apresentado na série Round 6 como o quinto desafio dos participantes. 

Para ver esta cena e conhecer mais sobre a inspiração desse jogo veja o vídeo abaixo clicando na imagem! :grin:

<a href="https://www.youtube.com/watch?v=97Je5DrBWQw" target="_blank"><img src="http://img.youtube.com/vi/97Je5DrBWQw/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

Nesta pequena adaptação o objetivo consistem em sobreviver à Ponte de Cristal para ganhar o dinheiro, assim como ocorre na série. 

O jogador possui três vidas para conseguir passar os levels. Ao todo são cinco níveis (indo do level 0 até o level 4) para conseguir passar pela ponte e ganhar o jogo!

Portanto, temos quatro casos: 
1. Caso o jogador acerte a escolha, ele passa para o próximo level.

2. Caso o jogador consiga passar por todos os levels, ele vence a partida ganhando o jogo. :money_mouth_face: 

3. Caso o jogador erre a escolha, o jogo reinicia e o jogador volta para o nível zero mas dessa vez com uma vida a menos. A sequencia da resposta permanece a mesma, portanto durante a partida o jogador deve lembrar de suas escolhas. A sequencia de resposta só inicia caso o jogador escolha restartar pelo menu ou clique botão New Start game.

4. Caso o jogador perca todas as vidas e não consiga passar, ele perde e o jogo **encerra sozinho** representando a morte final do personagem. :skull_and_crossbones:


**Implementações realizadas:**

*Interface*
- Menu contendo a opção de restart. 

- Duas linhas estáticas na cor branca contendo um resumo da história do jogo.

- Duas linhas vermelhas contendo informações sobre o estado jogo. A primeira linha contém informações sobre a vida do jogador e o level que ele se encontra. Na linha vermelha abaixo tem-se os textos sobre o estado atual jogo.

- Dois botões que são uma abstração de cada uma das vidraças no qual os participantes da série tem que escolher para atravessar a ponte. 

*Implementação do código*
- A estrutura do código foi implementada com base em máquina de estados. Dessa forma, tem-se oito estados: Play, Played, Failed, Win, Lost, RestartForPlayer, ResetGame, StartGame.

        1. **StartGame:** Inicia o jogo, seta os parâmetros defaults necessários e reseta os botões para o 
           jogador iniciar o jogo do zero.
        2. **Play:** Verifica se o jogador clicou em algum dos botões.
        3. **Played:** Verifica se o jogador acertou a resposta.
        4. **Failed:** Verifica se o jogador ainda possui vida para tentar jogar novamente.
        5. **RestartForPlayer:** Verifica se o jogador possui mais um nível para jogar ou se conseguiu passar 
           por todos os níveis.
        6. **ResetGame:** Reseta o jogo durante a partida atual. Ou seja, se o jogador ainda possui vida para
           jogar, este estado atua para resetar o jogo para ele conseguir jogar novamente.
        7. **Win:** Jogador ganhou a partida.
        8. **Lost:** Jogador perdeu a partida. A aplicação foi programada para encerrar sozinha nesse estado. 

- A interação do usuário ocorre por meio de interações com o botão. Quando o usuário seleciona um dos botões o jogo verifica se o usuário selecionou o botão da esquerda ou da direita para posteriormente realizar a checagem da resposta correta. 

- Cada vez que o jogo é iniciado gera-se uma array com respostas aleatórias. Cada valor em uma posição i da array representa a resposta correta no level i. O valor zero representa o botão esquerdo e o valor 1 representa o botão a direita. 

- Utilizou-se a classe [ctime](https://learn.microsoft.com/pt-br/cpp/atl-mfc-shared/reference/ctime-class?view=msvc-170) para controle de tempo entre as passagens de estado do jogo.

## Atividade 02 - DogSimulate

[LINK PARA A APLICAÇÃO](https://mikaori.github.io/cg_ufabc/dogsimulate/) 

**Como é a aplicação?**

Bob a brincadeira acabou???

Esta aplicação é uma sumilação de um **cachorro** (Bob) organizado que coleta seus brinquedos ao parar de brincar. Depois de um longo dia brincando no gramado do quintal, Bob tem o objetivo de organizar as bolas que ele espalhou. 

Desta forma, o personagem principal deve coletar todas as bolas na tela para completar a simulação, e para isso Bob deve ir até cada uma das bolas que aparecem na tela.

O cenário é composto pelos seguintes objetos:
- O Bob, o cachorro;
- As bolas do Bob (em vermelho e azul);
- Uma grama com flores rosas ao fundo.


**Implementações realizadas:**

*Interface*
- Fundo verde para representar o gramado do quintal.

- Flores rosas no gramado que são representados pelos pequenos brilhos rosas

- Bolas vermelhas e Azuis (brinquedos do Bob).

- Bob, o personagem principal. 

*Implementação do código*
- A estrutura do código foi implementada de acordo com as classes dos objetos:

        1. **Dog:** esta classe representa o cachorro (VAO, VBO e seus atributos de translação)
        2. **Balls:** esta classe gerencia as bolas do cachorro. Esta classe contém uma lista de instâncias da estrutura Bola, onde cada Bola possui um VBO e as propriedades de uma bola.
        3. **Flowers:** esta classe são as camadas de estrelas usadas ao fundo verde que representa a grama. Flowers contém um arranjo de objetos flower, onde cada objeto define o VBO de pontos da camada de flores.

- A interação do usuário com o personagem ocorre por meio das setas do teclado (←↑→↓) ou das teclas A W S D. Por meio delas, o usuário consegue mover o Bob pela tela para coletar as bolas.

- O jogo é reiniciado sozinho depois de um tempo que o usuário consegue coletar todas as bolas.


## Atividade 03 - The Forest

[LINK PARA A APLICAÇÃO](https://mikaori.github.io/cg_ufabc/theforest/) 

A aplicação consiste em simular a fuga contra a *shuriken* conhecida como **Skull**. 

O que ela tem de diferente? Bom, ela persegue qualquer um que entrar na floresta na escuridão da noite! 

O nome dela é Skull? Sim, pois quando ela alcança todos que passam pela floresta, estes possuem o mesmo triste fim... :skull_and_crossbones:

Assim, nesta aplicação o player possui **um único objetivo**: TENTAR FUGIR! 

Todavia, tem coisas na vida que não temos como fugir. Neste mini game não há rotas de fuga para o player vencer. O QUE? É isso mesmo??? Sim. A *skuriken* é rápida... mais rápida do que o player consegue fugir! :cold_sweat:

Além disso, a floresta não fornece nenhuma ajuda ao player, muito pelo contrário... a cada 10 segundos, aleatoriamente algumas das árvores trocam de lugar para deixar o jogador ainda mais confuso. 

Assim, existe apenas **um destino para o player**: a shuriken avança de algum canto do cenário perseguindo-o, quando ela o alcança, o player é atingido e o jogo **encerra sozinho** representando a morte dele. 

O game é em primeira pessoa e a câmera representa a visão do player. 

A interação do usuário ocorre por meio das teclas A, W, S, D ou  ←, →, ↑ e ↓. As teclas A e D são responsáveis pela movimentação lateral da posição da câmera (movimento conhecido como pan). As teclas W e S realizam a movimentação da posição da câmera para frente e para trás (movimento conhecido também como dolly) .

### **Implementações realizadas:**

### *Interface*
- Mensagem para o jogador indicando o que ele deve fazer. 

- Cenário: o background do cenário é composto por um fundo preto representando a noite escura, esse background também limita o campo de visão do player. Além disso, existem árvores no cenário para representar uma floresta.

- Câmera: representa a visão do player no jogo.

- Personagem: Shuriken 3D que persegue o player.

### *Implementação do código*

**camera.hpp**
A implementação da câmera utilizou o código do projeto lookat visto nas notas de aula. Assim, neste arquivo definimos todos os atributos necessários para criar o quadro da câmera. 

**camera.cpp**
A implementação da câmera utilizou o código do projeto lookat visto nas notas de aula. Neste arquivo definimos as funções da câmera, ou seja, aqui implementamos o movimento dolly, truck e pan da camera. Essas funções realizam modificações nas variáveis m_eye e m_at tal que a camera passa a alterar a posição e a sua orientação.

**gamedata.hpp**
O controle do estado da vida do player foi implementada com base em máquina de estados. Neste arquivo está contido a classe State utilizada para controlar os estados do jogo através da struct GameData. Existem apenas dois estados nesta aplicação: 
1. **Playing**: O estado representa que o jogador está tentando fugir. Esse é o estado inicial.
2. **GameOver**: Preparação para finalização do jogo.

**ground.hpp**
Contém a definição da classe Ground que é responsável pelo desenho do chão. O código utilizado foi visto nas notas de aula. 

**ground.cpp**
Contém a implementação das funções da classe Ground. O código utilizado foi visto nas notas de aula. Todavia, foi feito uma alteração para ser desenhado um quadriláterio de 20x20 unidades com apenas uma cor (no caso, verde). A cor do chão representa a grama da floresta.

**main.cpp**
Definição os pontos de entrada da aplicação. O código usado é o mesmo das notas de aula. Nenhuma modificação foi realizada além do título da janela.

**skull.hpp**
Aqui é definido a classe Shuriken, responsável pelo desenho da shuriken Skull na aplicação.

**skull.cpp**
Possui a implementação das definições da classe Shuriken, o qual esta classe apresenta quatro responsabilidades. A primeira é a renderização da shuriken. Já a segunda é definir a posição da skuriken no inicio do jogo, esta posição ocorre de maneira aleatória, podendo surgir nos seguintes pontos: {-15.0f, 0.20f, -15.0f}, {15.0f, 0.20f, 15.0f}, {-15.0f, 0.20f, 15.0f} e {15.0f, 0.20f, -15.0f}. O objetivo desses pontos é o jogador não conseguir identificar de onde ela irá surgir. 

A terceira é o movimento da shuriken que simula o efeito de perseguição em direção ao player. Para realizar este efeito é somado a posição da Skull o resultado da normalização da diferença entre os vetores de posição da shuriken e da câmera. 

Por fim, a quarta responsabilidade verifica se a shuriken encostou em algo. Isso será utilizado para verificar se a Skull tocou a câmera. A forma de implementação é medindo se a distancia entre a Skull e o objeto é menor que 0.5f, caso positivo, a Skull encostou.

**vertex.hpp**
Os códigos neste arquivo utilizam os códigos vistos em aula.
Há necessidade de criar este arquivo foi devido a utilização se suas estruturas em mais de uma classe. Dessa forma, foi implementado a struct que define os atributos que compõe um vértice. Cada um dos vértices possuem apenas uma posição definida (x,y,z) e um operador que verifica se dois vértices são iguais. Além disso, tem-se também a especialização explícita de std::hash para a estrutura de Vertex definida.

**window.hpp**
Implementação das definições da classe Window. Ela é responsável tanto pelo gerenciamento da janela da aplicação quanto pela lógica do jogo.  

**window.cpp**
Implementa as funções declaradas na classe Window. Assim, aqui é tratado:
- Os eventos de teclado de forma que quando alguma das teclas (AWSD ou as setas) é pressionada ou liberada, a velocidade de dolly, pan ou truck é alterada para -1, 1 ou 0. 

- É realizado também a rederização do total de 1500 árvore. Cada vez que o jogo inicia, a posição de cada árvore no jogo é gerada de forma aleatória. Além disso, ocorre também a alteração da posição de 1/5 das árvores de forma aleatória.

- Implementação de mensagens no jogo conforme o estado da player. Para isso utilizou-se um widget da ImGui para exibir a mensagem na interface. Os estados apresentam as seguintes mensagens:
  1. **Playing**: "JUST RUN!!!".
  2. **GameOver**: "OH NO, YOU ARE CAPTURE!!! he reached you!! CLOSING IN 10s...".

- Realiza-se também o update da rotação e da posição da shuriken conforme a posição da câmera, o que dá ao player a sensação de estar sendo perseguido pela shuriken. Verifica-se também se a shuriken alcançou o jogador, caso tenha alcançado, o status do jogo é alterado para GameOver. Esta alteração dá sequência a um cronometro de 10s que quando acaba o seu tempo o jogo é encerrado.  
