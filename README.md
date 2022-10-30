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

