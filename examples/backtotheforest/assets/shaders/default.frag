#version 300 es

precision mediump float;

in vec4 fragColor;
in float vis;

out vec4 outColor;

void main() {
  // define a cor final do fragmento
  if (gl_FrontFacing) {
    outColor = vec4(vis, vis, vis, 1) * fragColor;
  } else {
    // se estiver não estiver orientado pra frente da câmera a intensidade é metade da original
    outColor = vec4(vis, vis, vis, 1) * fragColor * 0.5;
  }
}