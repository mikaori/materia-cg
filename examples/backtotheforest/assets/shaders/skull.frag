#version 300 es

precision mediump float;

in float vis;
in vec3 fragN;
in vec3 fragLMoon;
in vec3 fragLSkull;
in vec3 fragV;
in vec2 fragTexCoord;
in vec3 fragPObj;
in vec3 fragNObj;

// Light properties
uniform vec4 IaMoon, IdMoon, IsMoon;
uniform vec4 lightDirWorldSpaceMoon;
uniform float lightDiameterMoon;

uniform vec4 IaSkull, IdSkull, IsSkull;
uniform vec4 lightDirWorldSpaceSkull;
uniform float lightDiameterSkull;


// Material properties
uniform vec4 Ka, Kd, Ks;
uniform float shininess;

// Diffuse texture sampler
uniform sampler2D diffuseTex;

// Mapping mode
// 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
uniform int mappingMode;

out vec4 outColor;

float IntensityLight(vec3 fragL, vec4 lightDirWorldSpace, float lightDiameter ){
  float intensityLight = lightDiameter/length(fragL-lightDirWorldSpace.xyz);
  if(intensityLight < 0.0){
    intensityLight = -intensityLight;
  }
  return intensityLight > 1.0 ? 1.0 : intensityLight;
}

float IntensityLight2(vec3 fragL, vec4 lightDirWorldSpace, float lightDiameter ){
  float intensityLight = lightDiameter/pow(length(fragL-lightDirWorldSpace.xyz),2.0);
  if(intensityLight < 0.0){
    intensityLight = -intensityLight;
  }
  return intensityLight > 1.0 ? 1.0 : intensityLight;
}

vec4 Phong(vec3 N, vec3 L, vec3 V, vec2 texCoord, vec4 Ia, vec4 Id, vec4 Is) {
  
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

  vec4 map_Kd = texture(diffuseTex, texCoord);
  vec4 map_Ka = map_Kd;

  vec4 diffuseColor = map_Kd * Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = map_Ka * Ka * Ia;

  
  return (ambientColor + diffuseColor + specularColor);
}

// Planar mapping
vec2 PlanarMappingX(vec3 P) { return vec2(1.0 - P.z, P.y); }
vec2 PlanarMappingY(vec3 P) { return vec2(P.x, 1.0 - P.z); }
vec2 PlanarMappingZ(vec3 P) { return P.xy; }

#define PI 3.14159265358979323846

// Cylindrical mapping
vec2 CylindricalMapping(vec3 P) {
  float longitude = atan(P.x, P.z);
  float height = P.y;

  float u = longitude / (2.0 * PI) + 0.5;  // From [-pi, pi] to [0, 1]
  float v = height - 0.5;                  // Base at y = -0.5

  return vec2(u, v);
}

// Spherical mapping
vec2 SphericalMapping(vec3 P) {
  float longitude = atan(P.x, P.z);
  float latitude = asin(P.y / length(P));

  float u = longitude / (2.0 * PI) + 0.5;  // From [-pi, pi] to [0, 1]
  float v = latitude / PI + 0.5;           // From [-pi/2, pi/2] to [0, 1]

  return vec2(u, v);
}

vec2 TextCoordinate(){
  if (mappingMode == 1) { // Cylindrical mapping
    return CylindricalMapping(fragPObj);
  } else if (mappingMode == 2) { // Spherical mapping
    return SphericalMapping(fragPObj);
  } else {// From mesh
    return fragTexCoord;
  }
}

void main() {

  vec2 textCord = TextCoordinate();
  vec4 colorMoon = Phong(fragN, fragLMoon, fragV, textCord, IaMoon, IdMoon, IsMoon) * IntensityLight(fragLMoon, lightDirWorldSpaceMoon, lightDiameterMoon);
  vec4 colorSkull = Phong(fragN, fragLSkull, fragV, textCord, IaSkull, IdSkull, IsSkull) * IntensityLight2(fragLSkull, lightDirWorldSpaceSkull, lightDiameterSkull);

  vec4 color = colorSkull + colorSkull;

  // define a cor final do fragmento
  
  outColor = vec4(vis, vis, vis, 1) * color;
  
}