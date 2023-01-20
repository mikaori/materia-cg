#version 300 es

precision mediump float;

in vec4 fragColor;

out vec4 outColor;

void main() {
  float i = 1.0 - gl_FragCoord.z;
  
  if (gl_FrontFacing) {
    outColor = fragColor;
  } else {
    outColor = fragColor * 0.5;
  }
}