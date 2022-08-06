#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projectionViewMatrix;
  vec3 directionToLight;
} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

const float AMBIENCE = 0.02;

void main() {
  gl_Position = ubo.projectionViewMatrix * push.modelMatrix * vec4(inPosition, 1.0);

  vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * inNormal);
  float lightIntensity = AMBIENCE + max(dot(normalWorldSpace, ubo.directionToLight), 0.0f);

  normal = inNormal;
  fragColor = lightIntensity * vec3(1.0f, 1.0f, 1.0f);
}
