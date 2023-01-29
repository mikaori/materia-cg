#version 300 es

precision mediump float;

// atributo de entrada que representa a posição do vértice
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

// variável para definir a cor do objeto
uniform vec4 color;
uniform vec4 lightDirWorldSpace;

uniform mat3 normalMatrix;

// matrizes de transformação geométrica (matriz de modelo, visão e projeção)
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix; // fornece efeito de perspectiva e dá a sensação de que a câmera é o player




// atributo de saída que é uma cor RGBA
out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec4 fragColor;
out float vis;

const float gradient = 2.0;
const float density = 0.25;

void main() {
  // posição do vértice no espaço da câmera
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;
  vec3 L = -(viewMatrix * lightDirWorldSpace).xyz;

  float dis = length(posEyeSpace.xyz);
  vis = exp(-pow(dis*density, gradient));
  vis = clamp(vis, 0.0, 1.0);

  // altera o atributo de acordo com a intensidade definida em i
  fragColor = color;
  fragL = L;
  fragV = -P;
  fragN = N;

  // converte o espaço da câmera para o espaço de recorte
  gl_Position = projMatrix * posEyeSpace;
}