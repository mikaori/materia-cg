#version 300 es

precision mediump float;

in vec4 fragColor;

out vec4 outColor;

void main() {
  float i = 1.0 - gl_FragCoord.z;
  
  // define a cor final do fragmento
  if (gl_FrontFacing) {
    outColor = fragColor;
  } else {
    // se estiver não estiver orientado pra frente da câmera a intensidade é metade da original
    outColor = fragColor * 0.5;
  }
}