#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projectionViewMatrix;
  vec3 directionToLight;
} ubo;

layout (set = 1, binding = 0) uniform sampler2D diffuseTexture;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

const float AMBIENCE = 0.02;

void main() {
  vec3 texColor = vec3(texture(diffuseTexture, texCoord));
  vec3 lightDir = normalize(ubo.directionToLight);

  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 ambientLight = vec3(1.0, 1.0, 1.0) * AMBIENCE;
  vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), lightDir), 0);

  vec3 result = (diffuseLight + ambientLight) * texColor;
  fragColor = vec4(result, 1.0);
}
