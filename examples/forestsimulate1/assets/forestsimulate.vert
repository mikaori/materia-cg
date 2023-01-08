#version 300 es

layout(location = 0) in vec3 inPosition;

uniform float angle;
uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - (-posEyeSpace.z / 5.0);

  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace; //converte as coordenadas no espaço da camera para o espaço de recorte

  float sinAngle = sin(angle);
  float cosAngle = cos(angle);

  gl_Position =
      vec4(inPosition.x * cosAngle + inPosition.z * sinAngle, inPosition.y,
           inPosition.z * cosAngle - inPosition.x * sinAngle, 1.0);
}