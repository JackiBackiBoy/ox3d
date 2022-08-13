#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

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

layout (push_constant) uniform Push {
  vec4 position;
  vec4 color;
  float radius;
} push;

void main() {
  fragOffset = OFFSETS[gl_VertexIndex];

  // World space coordinates
  vec3 cameraRight = { ubo.view[0][0], ubo.view[1][0], ubo.view[2][0] };
  vec3 cameraUp = { ubo.view[0][1], ubo.view[1][1], ubo.view[2][1] };

  vec3 worldPosition = push.position.xyz +
    push.radius * fragOffset.x * cameraRight +
    push.radius * fragOffset.y * cameraUp;

  gl_Position = ubo.projection * ubo.view * vec4(worldPosition, 1.0);
}
