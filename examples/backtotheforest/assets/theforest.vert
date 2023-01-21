#version 300 es

// atributo de entrada que representa a posição do vértice
layout(location = 0) in vec3 inPosition;

// variável para definir a cor do objeto
uniform vec4 color;

// matrizes de transformação geométrica (matriz de modelo, visão e projeção)
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix; // fornece efeito de perspectiva e dá a sensação de que a câmera é o player

// atributo de saída que é uma cor RGBA
out vec4 fragColor;

void main() {
  // posição do vértice no espaço da câmera
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  // intensidade da cor a distancia de 20 unidades da coordenada z do vértice no espaço da câmera
  float i = 1.0 - (-posEyeSpace.z / 20.0);

  // altera o atributo de acordo com a intensidade definida em i
  fragColor = vec4(i, i, i, 1) * color;

  // converte o espaço da câmera para o espaço de recorte
  gl_Position = projMatrix * posEyeSpace;
}