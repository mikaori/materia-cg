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
  



## Atividade 04 - Back To The Forest

[LINK PARA A APLICAÇÃO](https://mikaori.github.io/cg_ufabc/backtotheforest/) 

A aplicação consiste em uma **segunda versão** do simulador de fuga contra a *shuriken* conhecida como **Skull**. 

Nesta versão, a skull continua perseguindo qualquer um que entra na floresta a noite! Mas agora ela tem um novo poder que tem o objetivo de assustar o player ainda mais :japanese_ogre:

Seu novo poder consiste em iluminar a visão na direção que ela está perseguindo. Essa iluminação é assustadoramente vermelha.

Aqui o player continua com **um único objetivo**: TENTAR FUGIR! 

Mas, o mini game continua sem rotas de fuga para o player vencer. A *skuriken* ainda é tão rápida que o player não consegue fugir! :cold_sweat:

A floresta continua nada amigável com o player, ela ainda troca aleatoriamente de lugar a cada 10 segundos para deixar o jogador ainda mais confuso. 

Dessa forma, o **destino para o player** continua o mesmo: a shuriken avança de algum canto do cenário perseguindo-o, quando ela o alcança, o player é atingido e o jogo **encerra sozinho** representando a morte dele.

O game é em primeira pessoa e a câmera representa a visão do player. 

A interação do usuário continua o mesmo, o player se move por meio das teclas A, W, S, D ou  ←, →, ↑ e ↓. As teclas A e D são responsáveis pela movimentação lateral da posição da câmera (movimento conhecido como pan). As teclas W e S realizam a movimentação da posição da câmera para frente e para trás (movimento conhecido também como dolly) .

### **Implementações realizadas:**

### *Interface*
- **Widget:** Mensagem para o jogador indicando o que ele deve fazer. 

- **Cenário:** o background do cenário está mais complexo e cheio de detalhes. Existem shaders de iluminação e textura para as árvores e a skull. Há também uma iluminação global como se houvesse uma lua iluminando levemente a floresta.

- **Câmera:** representa a visão do player no jogo.

- **Personagem:** Shuriken 3D que persegue o player. A skull possui uma iluminação própria que interage com o cenário iluminando com sua luz vermelha as árvores que estão perto dela na direção que ela está perseguindo o player.

### *Implementação do código*

#### **camera.hpp**
A implementação da câmera utilizou o código do projeto lookat visto nas notas de aula. Assim, neste arquivo definimos todos os atributos necessários para criar o quadro da câmera.

#### **camera.cpp**
A implementação da câmera utilizou o código do projeto lookat visto nas notas de aula. Neste arquivo definimos as funções da câmera, ou seja, aqui implementamos o movimento dolly, truck e pan da camera. Essas funções realizam modificações nas variáveis m_eye e m_at tal que a camera passa a alterar a posição e a sua orientação.

`void Camera::computeProjectionMatrix(glm::vec2 const &size)`: calcula a matriz de projeção para obter o efeito de perspectiva.

`void Camera::computeViewMatrix()`: calcula a matriz de visão a partir das alterações que ocorrem em m_eye e m_at.

`void Camera::dolly(float speed)`: modifica as variáveis m_eye e m_at alterando a posição e orientação da camêra fazendo ela ir pra frente ou para trás ao longo da direção de visão. 

`void Camera::truck(float speed)`: modifica as variáveis m_eye e m_at alterando a posição e orientação da camêra fazendo ela andar lateralmente ao longo da direção de visão. 

`void Camera::pan(float speed)`: modifica as variáveis m_eye e m_at alterando a posição e orientação da camêra fazendo ela firar em torno de seu eixo y ao longo da direção de visão. 

`glm::vec3 Camera::getCameraPosition()`: retorna o vetor de posição da camêra (m_eye). 

`void Camera::loadLocation(GLint m_program)`: carrega as matrizes de localização de variáveis uniformes para viewMatrix e projMatrix.

`void Camera::bind()`: define variáveis uniformes para viewMatrix e projMatrix da camera.

#### **gamedata.hpp**
O controle do estado da vida do player foi implementada com base em máquina de estados. Neste arquivo está contido a classe State utilizada para controlar os estados do jogo através da struct GameData. Existem apenas dois estados nesta aplicação: 
1. **Playing**: O estado representa que o jogador está tentando fugir. Esse é o estado inicial.
2. **GameOver**: Preparação para finalização do jogo.

#### **forest.hpp**
Contém a definição da classe Forest que é responsável pelas árvores no cenário. Esta classe herda propriedades e métodos da classe Material.

#### **forest.cpp**
Contém a implementação das funções declaradas na classe Forest.

`void Forest::create()`: inicializa a floresta. Para isso, define as propriedades do material (linha 14 a 17), armazena os shaders e arranjo de vértices alocados ( linha 19 a 27), gera posições e tamanhos aleatórios para cada uma das árvores por meio da função randomizeTree(linha 30 a 32), carrega o arquivo OBJ das árvores chamando a função materialLoadModelFromFile da classe Material e cria o VBO, EBO e VAO. Além disso, ela obtem os atributos de cada vértice (posição, vetor normal e coordenada das texturas) (linha 41 a 66) e obtem também localização de variáveis uniformes para o modelMatrix e o normalMatrix (linha 75 a 77). Por fim, realiza também o carregamento da ocalização de variáveis uniformes dos materiais por meio da função materialLoadLocation da classe Material (linha 80).

`void Forest::paint(Camera m_camera, Moon m_moon, Skull m_skull) `: Esta função realiza o desenho das árvores da floresta. Para isso faz o bind e carrega a localização de variáveis uniformes da câmera, da lua, da skull, da textura e do material (linha 91 a 110). E também, configura a renderização de cada uma das árvores (linha 112 a 130).

`void Forest::update(float deltaTime)`: durante o update da floresta é feito a randomização da posição da árvore a cada 10 segundos. Para o controle do tempo utiliza a variavel timeToChangeTree. O código `if (rand() % 5 == 0)` faz com que apenas uma a cada cinco árvores troquem de lugar. 

`void Forest::randomizeTree(Tree &tree)`: Gera uma posição e size aleatório para a árvore recebida na função. 

`void Forest::destroy()`: libera os recursos do OpenGL gerenciados pela classe Forest.

#### **ground.hpp**
Contém a definição da classe Ground que é responsável pelo desenho do chão. O código utilizado foi visto nas notas de aula. 

#### **ground.cpp**
Contém a implementação das funções da classe Ground. O código utilizado foi visto nas notas de aula. Continua-se utilizando a estrutura de padrão de xadrez para que a ilumição da lua tenha a interação desejada. 

`void Ground::create(GLuint program)`: inicializa o chão. Para isso, define a unidade do quadrado no plano xz (linha 7 a 10) e cria o VBO e VAO (linha 12 a 29). Além disso, ela obtem também localização de variáveis uniformes para o modelMatrix, viewMatrix, projMatrix e color (linha 31 a 35). 

`void Ground::paint(Camera m_camera)`: realiza o desenho do chão. Nesta função é definido as variáveis uniformes para as matrizes e cor do chão. Segue a mesmo raciocinío implementado em que cada quadrilátero é transladado através de uma matriz modelo e posteriormente é desenhado por meio de glDrawArrays utilizando GL_TRIANGLE_STRIP.

`void Ground::destroy()`: libera os recursos do OpenGL gerenciados pela classe Groud.

#### **light.hpp**
Contém a definição da classe Luz que é responsável pela iluminação. Estão definidos aqui as funções para recuperar os valores das componentes da luz do objeto, como as componentes de reflexão, direção e diametro da luz.

#### **light.cpp**
Contém a implementação das funções declaradas na classe Light. É uma função base para as classes que possuem propriedades de Luz.

`void Light::lightBind()`: define os valores das variáveis uniformes relacionadas à luz no shader.

#### **main.cpp**
Define dos pontos de entrada da aplicação. O código usado é o mesmo das notas de aula. Nenhuma modificação foi realizada além do título e tamanho da janela.

#### **material.hpp**
Contém a definição da classe Material que é responsável pelas propriedades dos materiais de cada objeto. Esta classe herda propriedades e métodos da classe Model. Além disso, nela é declarada funções para recuperar os valores das constantes do material (Ka, Kd, Ks, shininess). Há também o modo de mapeamento definido para a textura e o objeto.

#### **material.cpp**
Contém a implementação das funções declaradas na classe Material.

`void Material::materialLoadLocation()`: recupera a localização das variáveis uniformes no shader relacionadas ao material.

`void Material::materialBindLocation()`: define os valores das variáveis uniformes no shader referente ao material.

`void Material::materialLoadProperties(const std::string basePath, tinyobj::ObjReader reader)`: carrega as propriedades do material .mtl do objeto. Verifica se há um material definido no arquivo, caso não haja há valores padrões para serem utilizados.

`void Material::materialLoadDiffuseTexture(std::string_view path)`: realiza o carregamento ad textura difusa do objeto por meio do `loadOpenGLTexture`.

`void Material::materialBindTexture()`: realiza o bind e o mapeamento da textura por meio da configuração dos filtros de textura e definição dos parâmentros de empacotamento da textura

`void Material::materialLoadModelFromFile(std::string_view path, bool standardize)`: realiza a preparação do carregamento do arquivo OBJ, assim nele é  verificado se foi possível carregar o modelo. Dentro desta função é feito também o carregamento do modelo (`modelLoadModelFromFile`) e das propriedades do material do modelo.

#### **model.hpp**
Contém a definição da classe Model que é responsável responsável por gerenciar o VBO, EBO e VAO do modelo geométrico. Esta classe contém propriedades e métodos que serão utilizados por outras classes que precisarem manipular objetos. Suas funções serão usadas por outras classes por meio de herança. Além disso, nela é declarada a struct para cada vértice e contém também a especialização de std::hash para Vertex.


#### **model.cpp**
Contém a implementação das funções declaradas na classe Model.

`void Model::modelStandardize()`: realiza a centralização do modelo na origem e normaliza o tamanho obtendo os limites máximos e mínimos. 

`void Model::modelComputeNormals()`: calcula os vetores normal do modelo se o modelo não possuir os vetores de normal calculado.

`void Model::modelLoadModelFromFile(tinyobj::ObjReader reader, bool standardize)`: carrega o modelo por meio do arquivo OBJ. Verifica também se é necessário centralizar o objeto na origem e normalizar ele, caso seja, é chamado a função de calcular os vetores normais do modelo.

`void Model::modelCreateBuffers()`: quando chamado, esta funçao realiza a criação e configuração de novos VBO, EBO e VAO.

#### **moon.hpp**
Contém a definição da classe Mon que é responsável pela iluminação que representa a luz da lua no cenário. Esta classe herda propriedades e métodos da classe Light.

#### **moon.cpp**
Contém a implementação das funções declaradas na classe Model.
`void Moon::create()`: inicializa as váriaveis de iluminação da lua.

`void Moon::loadLocation(GLint m_program)`: obtém a localização das variáveis uniformes relacionadas a iluminação da lua.

#### **skull.hpp**
Aqui é definido a classe Shuriken, responsável pelo desenho e movimentação da shuriken Skull na aplicação. Esta classe herda propriedades e métodos da classe Material e Light, visto que a skull possui uma iluminação própria.

#### **skull.cpp**
Possui a implementação das definições da classe Shuriken.

`void Skull::create()`: inicializa a skull. Para isso, define inicialmente as propriedades de iluminação e o modo de mapping da skull (linha 11 a 20). 

Em seguida,  realiza o armazenamento dos recursos de shader e arranjo de vértices alocados (linhas 26 a 30). Define-se a posição inicial e o carregamento do OBJ da skull. Obtem-se também os atributos de cada vértice (posição, vetor normal e coordenada das texturas) e a localização das variáveis uniforme para o modelMatrix, o normalMatrix e os materiais da skull. 

`void Skull::randomizeSkull()`: define a posição da skuriken no inicio do jogo, esta posição ocorre de maneira aleatória, podendo surgir nos seguintes pontos: {-15.0f, 0.20f, -15.0f}, {15.0f, 0.20f, 15.0f}, {-15.0f, 0.20f, 15.0f} e {15.0f, 0.20f, -15.0f}. O objetivo desses pontos é o jogador não conseguir identificar de onde ela irá surgir.

`void Skull::paint(Camera m_camera, Moon m_moon)`: realiza o desenho da skull usando o programa de shader para carregar a localização de variáveis uniformes e o bind das texturas, câmera, lua, skull, iluminação da skull e do material dela. Aqui também é definido a matriz modelo como a matriz de translação para ser enviado ao vertex shader. E por fim, ocorre a renderização da skull.

`void Skull::destroy()`: libera os recursos do OpenGL gerenciados pela classe Skull.

`void Skull::update(float deltaTime, Camera camera)`: realiza o update da posição, rotação e iluminação da shuriken que simula o efeito de perseguição em direção ao player. Para realizar este efeito é somado a posição da Skull o resultado da normalização da diferença entre os vetores de posição da shuriken e da câmera. 

`bool Skull::touch(glm::vec3 position_verify)`: verifica se a shuriken encostou em algo. Isso será utilizado para verificar se a Skull tocou a câmera. A forma de implementação é medindo se a distancia entre a Skull e o objeto é menor que 0.5f, caso positivo, a Skull encostou.

`void Skull::loadLocation(GLint m_program)`: obtém a localização das variáveis uniformes relacionadas a iluminação da skull


#### **window.hpp**
Implementação das definições da classe Window. Ela é responsável tanto pelo gerenciamento da janela da aplicação quanto pela lógica do jogo.  

#### **window.cpp**
Implementa as funções declaradas na classe Window. 

`void Window::onCreate()`: realiza os comandos de inicialização do estado da janela e do OpenGL. Define também o estado do game data da aplicação para Playing, o que significa que o jogo já começou e o player deve iniciar a sua fuga (linha 9). Além disso, é definido a cor preta no fundo do jogo (linha 15). Habilita-se, também o depth buffering e armazena os recursos alocados (shaders, arranjo de vértices) (linha 21). Em seguida, inicializa-se os objetos que farão parte da cena: o chão, a shuriken, a floresta e a lua (linha 27 a 36). 

`void Window::onPaint()`: realiza a implementação dos desenhos dos objetos na cena. Inicialmente ela limpa o buffer de cor de depth buffer e define variáveis uniformes para viewMatrix e projMatrix, essas matrizes serão usadas para cada objeto da cena. Em seguida, realiza-se o desenho do chão, da shuriken e das árvores (linha 62 a 71). Define também da orientação da superfície para indicar que o lado da frente tem vértices no sentido horário GL_CCW (linha 74).

`void Window::onPaintUI()`: desenha os widgets da ImGui. Implementação de mensagens no jogo conforme o estado da player. Para isso utilizou-se um widget da ImGui para exibir a mensagem na interface. Os estados apresentam as seguintes mensagens:
  1. **Playing**: "JUST RUN!!!".
  2. **GameOver**: "OH NO, YOU ARE CAPTURE!!! he reached you!! CLOSING IN 10s...".

`void Window::onResize(glm::ivec2 const &size)`: faz o resize dos elementos e objetos sempre que o tamanho da janela é alterado

`void Window::onDestroy()`: libera os recursos do OpenGL gerenciados pela classe Window.

`void Window::onUpdate()`: faz o update dos objetos em razão da variável `deltaTime`.
- Camera: movimentação dolly, truck pan de acordo com o speed de cada uma.
- Skull: realiza o update da posição e rotação da shuriken.
- Floresta: realiza o update da floresta, ou seja, realiza a troca de posição de um quinto das árvores de forma aleatória a cada 10 segundos.

`void Window::onEvent(SDL_Event const &event)`: captura os eventos de input do player, de forma que quando ocorre os eventos pressionamento ou liberação de alguma das teclas (AWSD ou as setas) do teclado, a velocidade de dolly, pan ou truck é alterada para -1, 1 ou 0.

#### **default.vert**
Semelhante ao implementado nas notas de aula, a diferença é a implementação do vertex shader default que é aplicado no chão, foi utilizado como referência um vídeo sobre névoa (https://www.youtube.com/watch?v=qslBNLeSPUc&pp=ugMICgJwdBABGAE%3D). Nele é utilizado a equação exp(-pow(dis*density, gradient)) que cria uma espécie de nevoeiro na cena, essa equação faz com que a visibilidade tenha o seu comportamento afetado de forma mais gradual e suave de acordo com a distância. 

#### **default.frag**
Semelhante ao visto nas notas de aula. A cor de saida do fragmento é de acordo com a orientação da face. Se estiver orientado para frente da câmera a intensidade é 1, caso contrário é a metade da original.

#### **forest.vert**
Semelhante ao implementado nas notas de aula, a diferença é a implementação do vertex shader da floresta foi utilizado como base a névoa comentada anteriormente, para que este efeito também ocorresse com as árvores da floresta. Além disso, neste código passamos a ter também mais um vetor l, dado que temos mais de uma fonte de luz.

#### **forest.frag**
Na implementação do fragment shader da skull foi realizada a iluminação utilizando a técnica de Phong (como visto nas notas de aula). Para a texturização, destá implementado os seguintes modos de mapping: planar, cilíndrico e esférico (como visto nas notas de aula). 

A alteração realizada encontra-se na forma como é realizado a cor final do framegmento do fragment shader: a cor final se dá pela soma das cores dadas pelas iluminações das duas fontes de luz que temos na cena: a skull e a lua. Assim, temos que calculas as cores usando Phong para a lua e também para a skull.

Em especial, para a floresta há mais uma regra no fragment antes de ser definido a cor final do fragmento: se estiver orientado para frente da câmera, a cor é a calculada anteriormente com intensidade 1, caso contrário é a cor é verde. Isso faz com que algumas folhas sejam verdes, e outras "mortas".

#### **skull.vert**
Semelhante ao implementado nas notas de aula, a diferença é a implementação do vertex shader da skull foi utilizado como base a névoa comentada anteriormente, para que este efeito também ocorresse com ela. Além disso, neste código passamos a ter também mais um vetor l, dado que temos mais de uma fonte de luz.

#### **skull.frag**
Na implementação do fragment shader da skull foi realizada a iluminação utilizando a técnica de Phong (como visto nas notas de aula). Para a texturização, destá implementado os seguintes modos de mapping: planar, cilíndrico e esférico (como visto nas notas de aula). 

A alteração realizada encontra-se na forma como é realizado a cor final do framegmento do fragment shader: a cor final se dá pela soma das cores dadas pelas iluminações das duas fontes de luz que temos na cena: a skull e a lua. Assim, temos que calculas as cores usando Phong para a lua e também para a skull.




