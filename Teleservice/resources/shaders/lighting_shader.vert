#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 ProjectionMatrix;
uniform mat4 RotationMatrix;

out vec3 Normal;

void main()
{
	Normal = aNormal;
	gl_Position = ProjectionMatrix * RotationMatrix * vec4(aPos, 1.0);
}