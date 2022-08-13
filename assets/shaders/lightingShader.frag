#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 fragColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projection;
  mat4 view;
  mat4 inverseView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[16];
  int numLights;
} ubo;

layout (set = 1, binding = 0) uniform sampler2D diffuseMap;
layout (set = 1, binding = 1) uniform sampler2D normalMap;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
  vec3 specularLight = vec3(0.0);
  vec3 surfaceNormal = normalize(fragNormalWorld);

  vec3 cameraWorldPos = ubo.inverseView[3].xyz;
  vec3 viewDir = normalize(cameraWorldPos - fragPosWorld);

  for (int i = 0; i < ubo.numLights; i++) {
    PointLight light = ubo.pointLights[i];
    vec3 lightDir = light.position.xyz - fragPosWorld;
    float attenuation = 1.0 / dot(lightDir, lightDir); // distance squared
    lightDir = normalize(lightDir);

    float cosAngIncidence = max(dot(surfaceNormal, lightDir), 0);
    vec3 intensity = light.color.xyz * light.color.w * attenuation;

    diffuseLight += intensity * cosAngIncidence;

    // Specular lighting
    vec3 halfAngle = normalize(lightDir + viewDir);
    float blinnTerm = dot(surfaceNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = pow(blinnTerm, 64.0); // high value -> sharper highlight
    specularLight += intensity * blinnTerm;
  }

  vec3 texColor = vec3(texture(diffuseMap, texCoord));
  fragColor = vec4(diffuseLight * texColor + specularLight * texColor, 1.0);
}
