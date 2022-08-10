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

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  vec3 lightPosition;
  vec4 lightColor; // w is intensity
} ubo;

const float LIGHT_RADIUS = 0.1;

void main() {
  fragOffset = OFFSETS[gl_VertexIndex];

  // World space coordinates
  vec3 cameraRight = { ubo.view[0][0], ubo.view[1][0], ubo.view[2][0] };
  vec3 cameraUp = { ubo.view[0][1], ubo.view[1][1], ubo.view[2][1] };

  vec3 worldPosition = ubo.lightPosition.xyz +
    LIGHT_RADIUS * fragOffset.x * cameraRight +
    LIGHT_RADIUS * fragOffset.y * cameraUp;

  gl_Position = ubo.projection * ubo.view * vec4(worldPosition, 1.0);
}
