#version 300 es

precision mediump float;

in vec4 fragColor;
in float vis;
in vec3 fragN;
in vec3 fragL;
in vec3 fragV;

// Light properties
uniform vec4 Ia, Id, Is;

// Material properties
uniform vec4 Ka, Kd, Ks;
uniform float shininess;
uniform vec4 lightDirWorldSpace;
uniform float lightDiameter;

out vec4 outColor;

float IntensityLight(){
  float intensityLight = lightDiameter/length(fragL-lightDirWorldSpace.xyz);
  if(intensityLight < 0.0){
    intensityLight = -intensityLight;
  }
  return intensityLight > 1.0 ? 1.0 : intensityLight;
}

vec4 Phong(vec3 N, vec3 L, vec3 V) {
  
  N = normalize(N);
  L = normalize(L);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    // vec3 R = normalize(2.0 * dot(N, L) * N - L);
    vec3 R = reflect(-L, N);
    V = normalize(V);
    float angle = max(dot(R, V), 0.0);
    specular = pow(angle, shininess);
  }

  vec4 diffuseColor = Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = Ka * Ia;

  
  return (ambientColor + diffuseColor + specularColor);
}

void main() {

  vec4 color = Phong(fragN, fragL, fragV) * IntensityLight();

  // define a cor final do fragmento
  if (gl_FrontFacing) {
    outColor = vec4(vis, vis, vis, 1) * color;
  } else {
    // se estiver não estiver orientado pra frente da câmera a intensidade é metade da original
    outColor = vec4(vis, vis, vis, 1) * color * 0.5;
  }
}