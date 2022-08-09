#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  vec3 lightPosition;
  vec4 lightColor; // w is intensity
} ubo;

layout (set = 1, binding = 0) uniform sampler2D diffuseMap;
layout (set = 1, binding = 1) uniform sampler2D normalMap;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec3 texColor = vec3(texture(diffuseMap, texCoord));
  vec3 lightDirection = ubo.lightPosition - fragPosWorld;
  float attenuation = 1.0 / dot(lightDirection, lightDirection); // distance squared

  vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
  vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
  vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), lightDirection), 0);

  vec3 result = (diffuseLight + ambientLight) * texColor;
  fragColor = vec4(result, 1.0);
}
