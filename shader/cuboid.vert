#version 330 core

layout (location = 0) in vec3 iPosition;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(iPosition, 1); 
}