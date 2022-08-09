#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inTexCoord;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  vec3 lightPosition;
  vec4 lightColor; // w is intensity
} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec4 worldPosition = push.modelMatrix * vec4(inPosition, 1.0);

  gl_Position = ubo.projection * ubo.view * worldPosition;

  vec3 lightDirection = ubo.lightPosition - worldPosition.xyz;
  vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w;
  vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;

  // Out variables
  texCoord = inTexCoord;
  fragPosWorld = worldPosition.xyz;
  fragNormalWorld = normalize(mat3(push.normalMatrix) * inNormal);
}
