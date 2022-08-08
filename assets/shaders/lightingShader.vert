#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projectionViewMatrix;
  vec3 directionToLight;
} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec4 worldPosition = push.modelMatrix * vec4(inPosition, 1.0);

  gl_Position = ubo.projectionViewMatrix * worldPosition;
  
  // Out variables
  texCoord = inTexCoord;
  fragPosWorld = worldPosition.xyz;
  fragNormalWorld = normalize(mat3(push.normalMatrix) * inNormal);
}
