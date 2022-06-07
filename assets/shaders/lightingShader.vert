#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec3 fragColor;

layout(push_constant) uniform Push {
  mat4 transform;
  mat4 modelMatrix;
} push;

const vec3 LIGHT_DIRECTION = normalize(vec3(2.0, 1.0, 3.0));
const float AMBIENCE = 0.02f;

void main() {
  gl_Position = push.transform * vec4(inPosition, 1.0);

  vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * inNormal);
  float lightIntensity = AMBIENCE + max(dot(normalWorldSpace, LIGHT_DIRECTION), 0.0f);

  normal = inNormal;
  fragColor = lightIntensity * inColor;
}
